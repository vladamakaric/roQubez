/*******************************************************************
Copyright 2013 Vladimir Makaric, email: vladaserbia@gmail.com

This file is part of RoQubez. RoQubez is a Tetris clone, with some
new features added, such as level rotation and interaction with falling
tetriminos. In RoQubez the user has the ability to edit, modify, 
remove, add, or delete tetriminos from the falling tetrimino set.

RoQubez is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RoQubez is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RoQubez.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************/

#include "Level.h"
#include "f_misc.h"
#include "MainMenu.h"
#include "global.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <sstream> 
#include "draw.h"
#include "GameOver.h"


using namespace std;

#include "TetriminoManager.h"

CLevel::CLevel(double _inset, int _matrixDimension) : 
	LM(CVector(_inset,_inset), _matrixDimension, (CGame::GetInstance()->screenHeight - 7*_inset)/double(_matrixDimension)), 
	mainMenu(CVector(LM.dimension*LM.tileSize + _inset*2, _inset), "menu", CGame::GetInstance()->smallBtnFont, CGame::GetInstance()->screenWidth - LM.dimension*LM.tileSize - 3*_inset, 30), 
	cwBtn(mainMenu.mRect.downLeft + CVector(0, _inset + mainMenu.mRect.height), "CW", CGame::GetInstance()->mainMenuBtnFont, mainMenu.mRect.width, mainMenu.mRect.width*0.8),
	ccwBtn(cwBtn.mRect.downLeft + CVector(0, _inset + cwBtn.mRect.height), "CCW", CGame::GetInstance()->mainMenuBtnFont, cwBtn.mRect.width, cwBtn.mRect.height),
	nextTetriminoTxt("Next: ", CGame::GetInstance()->scoreFont, 0xffffffff ),
	tileProgress(0), 
	focusTTileProgress(0),
	matrixSnap(false),
	MOVE_LEFT(false),
	updateDynamicT(true),
	updateFocusT(true),
	MOVE_RIGHT(false),
	ROTATE_TETRIMINO(false),
	SPEED_UP(false),
	sidewaysKeysTime(0),
	maxSMovementPeriod(0.055),
	sidewaysMovementPeriod(0.055),  //u sekundama
	rotateKeyTime(0),
	rotatePeriod(0.07),
	gameOver(false),
	points(0),
	currentRotAngle(0),
	gameOverAlpha(0),
	inset(_inset)
{

	previewTileSize = ccwBtn.mRect.width/T_MATRIX_SIZE;
	level = 1;
	levelDuration = 20;
	currentLevelTime = 0;
	pixelsPerSecundPerLevel = 30;
	pixelsPerSecond = pixelsPerSecundPerLevel*level;

	degreesPerSecund = 90*pixelsPerSecond/double(LM.tileSize);
	ROTATE_LEVEL = ROTATE_LEVELCCW = false;
	rotateState = NONE;
	pointsTxt = new CImage("Points: 0", CGame::GetInstance()->scoreFont, 0xffffffff);
	levelTxt = new CImage("Level: 1", CGame::GetInstance()->scoreFont, 0xffffffff);

	levelTxtPos = CVector(_inset*2 + LM.dimension*LM.tileSize, CGame::GetInstance()->screenHeight-_inset);
	pointsTxtPos = levelTxtPos - CVector(0, _inset);

	nextTetrimino =	rand()%CTetriminoManager::types.size();
	AddNewFallingBlock();

	rotationDir = 1;
	THAW = false;
	dbgDrop = false;
	DEBUG_angle = 0;
}

void CLevel::Initialize()
{
	mainMenu.ResetButton();
}

CLevel* CLevel::GetInstance() 
{
	static CLevel* instance = NULL;

	if(instance != NULL)
		delete instance;

	instance = new CLevel(30, CGame::GetInstance()->matrixSize);

	return instance;
}

void CLevel::RegisterEvents()
{
	if( event.type == SDL_KEYDOWN && rotateState == NONE)
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_q: ROTATE_LEVEL = true; rotationDir = 1; break;
			case SDLK_w: ROTATE_LEVEL = true; rotationDir = -1; break;
			case SDLK_UP: ROTATE_TETRIMINO = true;  break;
			case SDLK_DOWN: SPEED_UP = true; break;
			case SDLK_LEFT: MOVE_LEFT = true; break;
			case SDLK_RIGHT: MOVE_RIGHT = true; break;
		}
	}

	if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_DOWN: SPEED_UP = false; break;
			case SDLK_LEFT: MOVE_LEFT = false; break;
			case SDLK_RIGHT: MOVE_RIGHT = false; break;
		}
	}

	if( event.type == SDL_MOUSEMOTION )
	{
		mainMenu.MouseMotionEventHandling();
		ccwBtn.MouseMotionEventHandling();
		cwBtn.MouseMotionEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		mainMenu.MouseClickEventHandling();
		ccwBtn.MouseClickEventHandling();
		cwBtn.MouseClickEventHandling();
	}

    if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();
}

using namespace std;

void CLevel::Logic()
{
	if(gameOverAlpha==1)
	{
		CGameOver::GetInstance()->SetScore(points, level);
		CGame::GetInstance()->nextState = CGameOver::GetInstance();
		return;
	}

	if(mainMenu.mMouseClick)
	{
		CGame::GetInstance()->nextState = CMainMenu::GetInstance();
		return;
	}

	if(gameOver)
		return;

	currentLevelTime+=CGame::GetInstance()->deltaTime;

	if(currentLevelTime>levelDuration)
	{
		currentLevelTime = 0;
		level++;
		pixelsPerSecond += pixelsPerSecundPerLevel;
		pixelsPerSecundPerLevel*=0.9;
		levelDuration = levelDuration*1.3;

		stringstream ss;
		ss << level;

		delete levelTxt;
		levelTxt = new CImage("Level: " + ss.str(), CGame::GetInstance()->scoreFont, 0xffffffff);
	}

	if(cwBtn.mMouseClick)
	{
		cwBtn.mMouseClick = false;

		if(rotateState==NONE)
		{
			ROTATE_LEVEL = true;
			rotationDir = 1;
		}
	}
	
	if(ccwBtn.mMouseClick)
	{

		ccwBtn.mMouseClick = false;

		if(rotateState==NONE)
		{
			ROTATE_LEVEL = true;
			rotationDir = -1;
		}
	}



	deltaTime = CGame::GetInstance()->deltaTime;

	if(ROTATE_LEVEL)
	{
		ROTATE_LEVEL = false;
		if(rotateState == NONE) 
		{
			if(!FocusTetriminoCutOff())
				rotateState = WAITING_FOR_SNAP;
		}
	}

	if(rotateState!=NONE)
	{
		double angleUpdate = deltaTime*degreesPerSecund;
		currentRotAngle+= angleUpdate;
		F::MISC::CutOffValue(currentRotAngle, 90.0);

		if(currentRotAngle==90.0 && rotateState == ROTATING)
			FinishRotate();

		if(gameOver)
			return;
	}

	if(rotateState == ROTATING)
		return;

	if(updateDynamicT) DynamicTetriminosLogic();
	if(updateFocusT) FocusTetriminoLogic();	

	if(gameOver)
		return;

	if(rotateState == WAITING_FOR_SNAP)
	{
		if(!updateFocusT) FTUpdateAmount = 0;
		if(!updateDynamicT) DTUpdateAmount = 0;

		if(matrixSnap)
		{
			DTUpdateAmount = tileProgress;
			updateDynamicT = false;
			tileProgress = 0;
		}

		if(focusTMatrixSnap || focusTTileProgress == 0)
		{
			FTUpdateAmount = focusTTileProgress;
			focusTTileProgress = 0;
			updateFocusT = false;
		}
	}

	UpdateDynamicTetriminoPositions(DTUpdateAmount);

	if(focusTetrimino!=NULL)
	focusTetrimino->UpdatePosition( CVector(0, -FTUpdateAmount));

	if(matrixSnap && !pendingThawTetriminos.empty())
		ThawStaticTetriminos();

	LM.HandleFullRows();
	HandleSlicingTetriminos();

	if(!updateFocusT && !updateDynamicT)
		rotateState = ROTATING;
}

void CLevel::FinishRotate()
{
	Rotate(rotationDir);
	updateFocusT = updateDynamicT = true;
	currentRotAngle = 0;
	rotateState = NONE;

	if(focusTetrimino==NULL)
		AddNewFallingBlock();
}

void CLevel::FocusTetriminoLogic()
{
	if(!focusClicked && rotateState == NONE)
	focusTetrimino->FTD->UpdateSpeed(deltaTime, SPEED_UP, pixelsPerSecond);
	else
		focusTetrimino->FTD->pixelsPerSecund = pixelsPerSecond;

	FTUpdateAmount = focusTetrimino->FTD->pixelsPerSecund*deltaTime;

	F::MISC::CutOffValue(FTUpdateAmount, double(LM.tileSize*0.5));

	if(!FTUpdateAmount)
		return;

	focusTetrimino->FTD->framesActive++;

	focusTTileProgress += FTUpdateAmount;

	if(focusTTileProgress >= LM.tileSize)
	{
		focusTTileProgress-=LM.tileSize;
		focusTMatrixSnap = true;
	}
	else
		focusTMatrixSnap = false;

	if(focusTMatrixSnap)
		LM.UpdateTetrimino(focusTetrimino);

	if(!focusClicked)
	if(FTBottomCollisionWithDT())
		ClickFTWithDynamicTs();

	if(focusTetrimino->GetLowestApsoluteJCoord()<LM.dimension+2 && rotateState == NONE)
	RegulateKeyPressTime();

	if(focusTMatrixSnap)
	if(LM.BlockedFromBellowByNonDynamicTCubes(focusTetrimino))
	{
		tetriminos.push_back(focusTetrimino);
		RecursiveFreeze(focusTetrimino);

		if(LM.tetriminoOutOfMatrixFreeze)
		{
			gameOver = true;
			return;
		}

		if(rotateState!=NONE)
		focusTetrimino = NULL;
		else
			AddNewFallingBlock();

		FTUpdateAmount = 0;
	}
}

void CLevel::UpdateLevelMatrix()
{
	LM.ClearDynamicTetriminos();

	if(focusTetrimino!=NULL)
	LM.AddDynamicTetrimino(focusTetrimino);

	vector<CTetrimino*>::iterator it = tetriminos.begin();
	for(; it != tetriminos.end(); it++)
	{
		if(!(*it)->dynamic)
			continue;

		(*it)->j--;

		LM.AddDynamicTetrimino(*it);
	}
}

void CLevel::Draw()
{



	glPushMatrix();

	glTranslatef(LM.matrixCenter.x, LM.matrixCenter.y, 0);
	glRotatef(-currentRotAngle*rotationDir,0,0,1);

	glTranslatef(-LM.matrixCenter.x, -LM.matrixCenter.y, 0);

	LM.Draw();	

	if(focusTetrimino!=NULL)
	focusTetrimino->Draw();

	if(!tetriminos.empty())
	for (vector<CTetrimino*>::iterator tetrimino = tetriminos.begin();
	tetrimino != tetriminos.end(); ++tetrimino) 
		(*tetrimino)->Draw();



	glTranslatef(LM.matrixCenter.x, LM.matrixCenter.y, 0);
	

	glPopMatrix();

	DrawNextTetrimino();

	mainMenu.Draw();
	ccwBtn.Draw();
	cwBtn.Draw();

	levelTxt->Draw(levelTxtPos);
	pointsTxt->Draw(pointsTxtPos);
	nextTetriminoTxt.Draw(pointsTxtPos - CVector(0, inset));

	if(!gameOver)
		return;
	////////////////////////////////////////////////////////
	gameOverAlpha+=CGame::GetInstance()->deltaTime;

	if(gameOverAlpha>1)
		gameOverAlpha = 1;
	
	glColor4f(0,0,0,gameOverAlpha);
	Draw::Rect(CVector(0, CGame::GetInstance()->screenHeight), CGame::GetInstance()->screenWidth, CGame::GetInstance()->screenHeight);
}

bool CLevel::MoveTetriminoSideways( int dir )
{
	if(focusTetrimino->OnEdge(dir))
		return false;

	vector<CTCube> apsoluteSidewaysFeelers;
	
	if(dir == CTetrimino::LEFT_DIR)
	apsoluteSidewaysFeelers = *focusTetrimino->FTD->leftCollisionFeelers;
	else
		apsoluteSidewaysFeelers = *focusTetrimino->FTD->rightCollisionFeelers;

	focusTetrimino->ChangeToApsoluteCoords(apsoluteSidewaysFeelers);

	if(LM.TCubesIntersectTetriminos(apsoluteSidewaysFeelers))
		return false;


	vector<CTCube> apsoluteBottomSidewaysFeelers = (dir == CTetrimino::LEFT_DIR) ?  
	*focusTetrimino->FTD->BottomLeftCollisionFeelers : *focusTetrimino->FTD->BottomRightCollisionFeelers;
	focusTetrimino->ChangeToApsoluteCoords(apsoluteBottomSidewaysFeelers);


	if(!focusTMatrixSnap)
	if(LM.TCubesIntersectStaticTetriminos(apsoluteBottomSidewaysFeelers))
		return false;


	//dynamic check
	//////////////////////////////////////////////////////////////

	vector<CTCube> apsoluteUpperSidewaysFeelers = (dir == CTetrimino::LEFT_DIR) ?  
	*focusTetrimino->FTD->UpperLeftCollisionFeelers : *focusTetrimino->FTD->UpperRightCollisionFeelers;
	focusTetrimino->ChangeToApsoluteCoords(apsoluteUpperSidewaysFeelers);


	bool upperIntersect = LM.TCubesIntersectDynamicTetriminos(apsoluteUpperSidewaysFeelers);
	bool bottomIntersect = LM.TCubesIntersectDynamicTetriminos(apsoluteBottomSidewaysFeelers);

	if(upperIntersect && bottomIntersect)
	{
		if(!F::MISC::DoublesEqual(tileProgress, focusTTileProgress, 4))
			return false;

		if(!focusClicked)
		ClickFTWithDynamicTs();
	}
	else
	{
		if(upperIntersect && tileProgress>focusTTileProgress)
			return false;

		if(bottomIntersect && focusTTileProgress>tileProgress)
			return false;
	}




	///////////////////////////////////////////////////////////////

	LM.ClearTetrimino(focusTetrimino);
	focusTetrimino->Move(dir);
	LM.AddDynamicTetrimino(focusTetrimino);

	return true;
}

void CLevel::RotateTetriminoCW()
{
	int lowestJ, leftMostI, rightMostI; 
	focusTetrimino->GetRotatedBounds(1, leftMostI, rightMostI, lowestJ);
	bool rotatedTInBounds = LM.ColumnInBounds(leftMostI) && LM.ColumnInBounds(rightMostI) && LM.RowInBoundsFromBelow(lowestJ); 

	if(!rotatedTInBounds)
		return;

	int rotatedFace = focusTetrimino->GetRotatedFace(1);
	vector<CTCube> testRotatedT = focusTetrimino->type->tCubes[rotatedFace];
	focusTetrimino->ChangeToApsoluteCoords(testRotatedT);

	LM.RemoveAllTCubesUpwardOfMatrix(testRotatedT);
	LM.ClearTetrimino(focusTetrimino);


	vector<CTCube> upperRotatedT, bottomRotatedT;
	bool bottomIntersect, upperIntersect;

	if(LM.TCubesInMatrixCollisionWithTetriminos(testRotatedT))
		goto doneRotating;

	bottomRotatedT = focusTetrimino->type->bottomCollisionFeelers[rotatedFace];
	focusTetrimino->ChangeToApsoluteCoords(bottomRotatedT);
	LM.RemoveAllTCubesUpwardOfMatrix(bottomRotatedT);


	if(!LM.ValidTCubesPlacement(bottomRotatedT))
		goto doneRotating;

	if(LM.TCubesIntersectStaticTetriminos(bottomRotatedT))
		goto doneRotating;

	bottomIntersect = LM.TCubesIntersectTetriminos(bottomRotatedT);

	upperRotatedT = focusTetrimino->type->uppperCollisionFeelers[rotatedFace];
	focusTetrimino->ChangeToApsoluteCoords(upperRotatedT);
	LM.RemoveAllTCubesUpwardOfMatrix(upperRotatedT);

	upperIntersect = LM.TCubesIntersectTetriminos(upperRotatedT);

	if(upperIntersect && bottomIntersect)
	{
		if(!F::MISC::DoublesEqual(tileProgress, focusTTileProgress, 4))
			goto doneRotating;

		if(!focusClicked)
			ClickFTWithDynamicTs();
	}
	else
	{
		if(upperIntersect && tileProgress>focusTTileProgress)
			goto doneRotating;

		if(bottomIntersect && focusTTileProgress>tileProgress)
			goto doneRotating;
	}

	focusTetrimino->Rotate(true);

	doneRotating:
	LM.AddDynamicTetrimino(focusTetrimino);

}

void CLevel::DetectDynamicTetriminoDownwardFreeze()
{
	vector<CTetrimino*>::iterator it = tetriminos.begin();
	for(; it != tetriminos.end(); it++)
	{
		if(!(*it)->dynamic)
			continue;

		RecursiveFreeze(*it);
	}
}

void CLevel::AddNewFallingBlock()
{
	focusTetrimino = new CTetrimino(&LM, CTetriminoManager::types[nextTetrimino]);
	nextTetrimino =	rand()%CTetriminoManager::types.size();
	focusTetrimino->FTD->pixelsPerSecund = pixelsPerSecond;

	focusClicked = false;
	focusTTileProgress = 0;
	focusTMatrixSnap = false;

  	if(LM.BlockedFromBellowByNonDynamicTCubes(focusTetrimino))
 		gameOver=true;
}

void CLevel::RegulateKeyPressTime()
{
	bool attemptMove = false;


	if(MOVE_LEFT || MOVE_RIGHT)
	{
		sidewaysKeysTime += CGame::GetInstance()->deltaTime;
	}
	else
	{
		sidewaysMovementPeriod = 0;
		sidewaysKeysTime = 0;
	}

	if(sidewaysKeysTime>sidewaysMovementPeriod)
	{
		bool prevClicked = focusClicked;
		bool moved = false;
		if(MOVE_LEFT)
			moved = MoveTetriminoSideways(CTetrimino::LEFT_DIR);
		else if(MOVE_RIGHT)
			moved = MoveTetriminoSideways(CTetrimino::RIGHT_DIR);

		if(moved)
		{


			sidewaysKeysTime=0;
		
			sidewaysMovementPeriod*=0.85;

			if(sidewaysMovementPeriod==0)
				sidewaysMovementPeriod =maxSMovementPeriod;

			if (prevClicked) 
				focusClicked = FTBottomCollisionWithDT();
		}
		
	}

	if(ROTATE_TETRIMINO)
	{
		ROTATE_TETRIMINO = false;
		RotateTetriminoCW();
	}


}


void CLevel::AddNewDynamicBlock( int _i, int _j )
{
	CTetrimino* newDBlock = new CTetrimino(&LM, _i, _j, CTetriminoManager::types[1], true);
	tetriminos.push_back(newDBlock);
}

void CLevel::ClearTetriminos()
{
	vector<CTetrimino*>::iterator it = tetriminos.begin();
	for(; it != tetriminos.end(); it++)
		delete *it;

	tetriminos.clear();
	LM.Clear();
}

void CLevel::UpdateDynamicTetriminoPositions( double _amount )
{
	vector<CTetrimino*>::iterator it = tetriminos.begin();
	for(; it != tetriminos.end(); it++)
	{
		if(!(*it)->dynamic)
			continue;

		(*it)->UpdatePosition( CVector(0, -_amount));
	}
}

void CLevel::DynamicTetriminosLogic()
{
	DTUpdateAmount = pixelsPerSecond*deltaTime;

	F::MISC::CutOffValue(DTUpdateAmount, double(LM.tileSize*0.5));

	tileProgress+=DTUpdateAmount;

	if(!DTUpdateAmount)
		return;

	if(tileProgress >= LM.tileSize)
	{
		tileProgress -= LM.tileSize;
		matrixSnap = true;
	}
	else 
		matrixSnap = false;

	if(matrixSnap)
	{
		UpdateLevelMatrix();
		DetectDynamicTetriminoDownwardFreeze();
	}
}

void CLevel::ClickFTWithDynamicTs()
{
	focusClicked = true;
	focusTetrimino->FTD->pixelsPerSecund = pixelsPerSecond;

	double tProgressDiff = tileProgress - focusTTileProgress;
	double offset = -tProgressDiff; 

	focusTetrimino->position.y += offset;
	focusTTileProgress = tileProgress;
}

bool CLevel::FTBottomCollisionWithDT()
{
	focusTetrimino->ChangeToApsoluteCoords(*focusTetrimino->DTD->bottomCollisionFeelers);

	if(LM.TCubesIntersectDynamicTetriminos(*focusTetrimino->DTD->bottomCollisionFeelers))
	{

		focusTetrimino->ChangeToRelativeCoords(*focusTetrimino->DTD->bottomCollisionFeelers);

		if(focusTTileProgress + FTUpdateAmount > tileProgress )
			return true;
	}
	else
	focusTetrimino->ChangeToRelativeCoords(*focusTetrimino->DTD->bottomCollisionFeelers);

	return false;
}

void CLevel::RecursiveFreeze( CTetrimino* _tetrimino )
{
	if(!LM.BlockedFromBellowByNonDynamicTCubes(_tetrimino))
		return;

	_tetrimino->Freeze();

	if(LM.tetriminoOutOfMatrixFreeze)
		return;

	vector<CTCube>::iterator it = _tetrimino->DTD->upperCollisionFeelers->begin();

	for(; it != _tetrimino->DTD->upperCollisionFeelers->end(); it++)
	{
		if(!LM.TCubeInBounds(_tetrimino->GetApsoluteTCube(*it)))
			continue;

		if(LM.GetTetriminoAtTCube(_tetrimino->GetApsoluteTCube(*it)) == NULL)
			continue;

		RecursiveFreeze(LM.GetTetriminoAtTCube(_tetrimino->GetApsoluteTCube(*it)));
	}
}

void CLevel::RecursiveThaw( CTetrimino* _tetrimino )
{
	if(_tetrimino->dynamic)
		return;

	if(LM.BlockedFromBellowByNonDynamicTCubes(_tetrimino))
		return;

	_tetrimino->Thaw();

	vector<CTCube>::iterator it = _tetrimino->DTD->upperCollisionFeelers->begin();
	for(; it != _tetrimino->DTD->upperCollisionFeelers->end(); it++)
	{
		if(!LM.TCubeInBounds(_tetrimino->GetApsoluteTCube(*it)))
			continue;

		if(LM.GetTetriminoAtTCube(_tetrimino->GetApsoluteTCube(*it)) == NULL)
			continue;

		if(LM.GetTetriminoAtTCube(_tetrimino->GetApsoluteTCube(*it))->dynamic)
			continue;

		RecursiveThaw(LM.GetTetriminoAtTCube(_tetrimino->GetApsoluteTCube(*it)));
	}
}

void CLevel::HandleSlicingTetriminos()
{
	if(LM.fullStaticRows.empty())
		return;

	points+=LM.fullStaticRows.size()*LM.fullStaticRows.size();

	if(focusTetrimino!=NULL)
	if(focusTetrimino->FTD->framesActive!=0)
		points+=LM.fullStaticRows.size();

	for(std::set<int>::iterator staticRow = LM.fullStaticRows.begin(); 
		staticRow!=LM.fullStaticRows.end(); ++staticRow)
		for(int i=0; i < LM.dimension; i++)
		{
			CTetrimino* t = LM.cubeMatrix[i][*staticRow];

			if(!t->sliced)
			{
				t->sliced = true;
				slicedTetriminos.push_back(t);
			}

			t->MarkApsoluteTCubeForDeletion(CTCube(i, *staticRow));
		}
	
	vector<int> thawFeelerRows;
	int prevRow = *LM.fullStaticRows.begin();
	std::set<int>::iterator it = LM.fullStaticRows.begin();
	it++;
	for (; it!=LM.fullStaticRows.end(); it++) 
	{
		if(*it != prevRow + 1)
			thawFeelerRows.push_back(prevRow + 1);

		prevRow = *it;
	}

	thawFeelerRows.push_back(prevRow + 1);
	LM.fullStaticRows.clear();

	
	/*
	if(!pendingThawTetriminos.empty())
	{
		std::tr1::unordered_set<CTetrimino*>::iterator it = slicedTetriminos.begin();
		for(; it != slicedTetriminos.end(); it++)
			pendingThawTetriminos.erase(*it);
		
		points+=2;
	}*/

	
	for(std::vector<CTetrimino*>::iterator it = slicedTetriminos.begin(); 
		it != slicedTetriminos.end(); it++)
	{
		vector<vector<CTCube> > subTCubeVectors;

		vector<CTCube> newTCubes;
		(*it)->GetRemainingTCubes(newTCubes);

		LM.ClearTetrimino(*it);

		if(newTCubes.empty())
			continue;

		CTetriminoManager::PartitionTCubesIntoClusters(subTCubeVectors, newTCubes);

		 for(vector<vector<CTCube> >::iterator tCubesPartition = subTCubeVectors.begin(); 
			 tCubesPartition != subTCubeVectors.end(); ++tCubesPartition) 
		{
			CTetriminoType* newType = CTetriminoManager::GetNewTypeFromTCubeVector(*tCubesPartition);

			CTetrimino* newT = new CTetrimino(&LM, (*it)->i, (*it)->j, newType, false);  

			tetriminos.push_back(newT);

			newT->color = (*it)->color;

			LM.AddDynamicTetrimino(newT);

			newT->pendingThaw = true;
			pendingThawTetriminos.push_back(newT);
		}
	}

	for (vector<CTetrimino*>::iterator b = tetriminos.begin(); b != tetriminos.end();) 
	{ 
		if ((*b)->sliced) 
		{
			delete *b;
			b = tetriminos.erase( b ); 	
		} else ++b;
	}

	if(tetriminos.empty())
		points+=5;

	for(vector<int>::iterator row = thawFeelerRows.begin(); row!= thawFeelerRows.end(); ++row)
	{
		for(int i=0; i < LM.dimension; i++)
		{
			CTetrimino* t = LM.cubeMatrix[i][*row];
			if(t!=NULL) if(!t->pendingThaw)
			{
				t->pendingThaw = true;
				pendingThawTetriminos.push_back(t);
			}
		}
	}

	slicedTetriminos.clear();

	stringstream ss; ss << points;
	delete pointsTxt;
	pointsTxt = new CImage("Points: " + ss.str(), CGame::GetInstance()->scoreFont, 0xffffffff);
}

void CLevel::ThawStaticTetriminos()
{
	for (vector<CTetrimino*>::iterator b = tetriminos.begin(); b != tetriminos.end(); ++b) 
	{
		(*b)->pendingThaw = false;
		RecursiveThaw(*b);
	}

	pendingThawTetriminos.clear();
}

bool CLevel::FocusTetriminoCutOff()
{
	return focusTetrimino->GetHighestApsoluteJCoord() > LM.dimension;
}

void CLevel::RotateTetriminoWithLevel(CTetrimino* _tetrimino, int dir)
{
	int relativeI = _tetrimino->i - LM.dimension/2;
	int relativeJ = _tetrimino->j - LM.dimension/2;

	if(dir==1)
	F::MISC::RAroundOrigin90CW(relativeI, relativeJ);
	else
	F::MISC::RAroundOrigin90CCW(relativeI, relativeJ);

	_tetrimino->i = relativeI + LM.dimension/2;
	_tetrimino->j = relativeJ + LM.dimension/2;
	_tetrimino->Rotate(dir);

	_tetrimino->AlignPositionWithMatrix();
}

void CLevel::Rotate( int dir )
{
	LM.Clear();

	for (vector<CTetrimino*>::iterator tetrimino = tetriminos.begin();
		tetrimino != tetriminos.end(); ++tetrimino) 
	{
		RotateTetriminoWithLevel(*tetrimino, dir);
		LM.AddDynamicTetrimino(*tetrimino);
		(*tetrimino)->dynamic = false;
	}

	if(focusTetrimino!=NULL)
	{
		if(focusTetrimino->GetHighestApsoluteJCoord()<LM.dimension)
		{
			
			RotateTetriminoWithLevel(focusTetrimino, dir);
			LM.AddDynamicTetrimino(focusTetrimino);
			focusTetrimino->dynamic = false;
			RecursiveThaw(focusTetrimino);
			
			if(LM.BlockedFromBellowByNonDynamicTCubes(focusTetrimino))
			{
				tetriminos.push_back(focusTetrimino);
/*				RecursiveFreeze(focusTetrimino);*/
				AddNewFallingBlock();
				gameOver = false; //nisu jos odmrznuti
			}
				
		}
	}

	for (vector<CTetrimino*>::iterator tetrimino = tetriminos.begin();
		tetrimino != tetriminos.end(); ++tetrimino) 
 		RecursiveThaw(*tetrimino);

	if(focusTetrimino!=NULL)
	if(LM.BlockedFromBellowByNonDynamicTCubes(focusTetrimino))
	{
		gameOver=true;
		return;
	}

	memset(LM.potentialFullRows, 1, LM.dimension*sizeof(bool));		
}

void CLevel::DrawNextTetrimino()
{
	glColor3f(1,1,1);
	CVector bottomLeft( ccwBtn.mRect.downLeft.x, ccwBtn.mRect.upRight.y + inset);


	for (vector<CTCube>::iterator tcube = CTetriminoManager::types[nextTetrimino]->tCubes[0].begin();
		tcube != CTetriminoManager::types[nextTetrimino]->tCubes[0].end(); ++tcube) 
	{
		int i = tcube->i;
		int j = tcube->j;

		CTetriminoManager::MakeRelativeCoordsApsoluteToSquareMatrix(i, j, T_MATRIX_SIZE);

		Draw::Rect(bottomLeft + CVector(i*previewTileSize, previewTileSize*(j+1)), previewTileSize, previewTileSize);
	}

	glLineWidth(1);

	for(int i = 1; i < T_MATRIX_SIZE; i++)
	{
		Draw::Line(CVector(bottomLeft.x + i*previewTileSize, bottomLeft.y), CVector(bottomLeft.x + i*previewTileSize, bottomLeft.y + T_MATRIX_SIZE*previewTileSize));
		Draw::Line(CVector(bottomLeft.x, bottomLeft.y + i*previewTileSize), CVector(bottomLeft.x + T_MATRIX_SIZE*previewTileSize, bottomLeft.y + i*previewTileSize));
	}
}



