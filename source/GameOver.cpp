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

#include "GameOver.h"
#include "HighScores.h"
#include "About.h"
#include "Game.h"
#include "MainMenu.h"
#include "Square.h"
#include "draw.h"

#define BTN_W 300

CGameOver::CGameOver() : 
	doneBtn(CVector((CGame::GetInstance()->screenWidth - BTN_W)/2, 100), 
	"done", CGame::GetInstance()->mainMenuBtnFont, BTN_W, 50),
	goTxt("Game Over", CGame::GetInstance()->headLineFont, 0xffffffff),
	enterName("Enter a sequence of characters:", CGame::GetInstance()->creditsFont, 0xffffffff),
	userStr(""),
	playerTxt(NULL)
{
	
	userTxtHeight = CGame::GetInstance()->screenHeight/2 - 70;
	blinkDiv.lowerLeft = CVector(CGame::GetInstance()->screenWidth/2, userTxtHeight);
	
}

CGameOver* CGameOver::GetInstance()
{
	static CGameOver* instance = NULL;

	if(instance == NULL)
		instance = new CGameOver();

	return instance;
}

void CGameOver::Draw()
{
	CSquare screenRect = CGame::GetInstance()->screenRect;

	screenRect.downLeft.y += CGame::GetInstance()->screenHeight*0.83;
	screenRect.CalculateWH();
	goTxt.DrawInRectCenter(screenRect);
	screenRect.downLeft.y -= 150;
	screenRect.CalculateWH();
	youScored->DrawInRectCenter(screenRect);
	screenRect.downLeft.y -= 100;
	screenRect.CalculateWH();


	

	doneBtn.Draw();

	if(scoreGoodEnough)
	{
		enterName.DrawInRectCenter(screenRect);

		if(playerTxt!=NULL)
			playerTxt->Draw(CVector((CGame::GetInstance()->screenWidth - playerTxt->width)/2, userTxtHeight + playerTxt->height));

		blinkDiv.Draw(5, 80);
	}
}

void CGameOver::RegisterEvents()
{
	doneBtn.mMouseClick = false;

	if( event.type == SDL_KEYDOWN )
	{
		if( event.key.keysym.sym == SDLK_RETURN)
		{
			doneBtn.mMouseClick = true;
		}
	}

	if( event.type == SDL_MOUSEMOTION )
	{
		doneBtn.MouseMotionEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		doneBtn.MouseClickEventHandling();
	}

	if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();

	if(scoreGoodEnough)
	RegisterStringInput();
}

#include <sstream>


void CGameOver::Initialize()
{
	scoreGoodEnough = CHighScores::GetInstance()->Placable(score);


	std::stringstream ss;
	ss << score;

	youScored = new CImage("You scored: " + ss.str(), CGame::GetInstance()->mainMenuBtnFont, 0xffffffff);
	doneBtn.ResetButton();
}

void CGameOver::Logic()
{
	if(scoreGoodEnough && userStr.empty())
		return;

	if(doneBtn.mMouseClick)
	{
		CGame::GetInstance()->nextState = CHighScores::GetInstance();

		if(scoreGoodEnough)
			CHighScores::GetInstance()->AddNewEntry(new CHSEntry(userStr, score, level, CGame::GetInstance()->matrixSize));
	}
}

void CGameOver::SetScore(int _score, int _level)
{
	score = _score;
	level = _level;
}

void CGameOver::RegisterStringInput()
{
	bool change = false;

	if( event.type == SDL_TEXTINPUT ) {
		if( userStr.length() <= CGame::GetInstance()->maxNameSize )
		{
			userStr += event.text.text;
			change = true;
		}
	}

	if( event.type == SDL_KEYDOWN )
	{
		if( event.key.keysym.sym == SDLK_BACKSPACE && !userStr.empty())
		{
			userStr.erase( userStr.length() - 1 );
			change = true;
		}
	}

	if(change)
	{
		if(playerTxt!=NULL)
			delete playerTxt;

		if(userStr.empty())
		{
			playerTxt = NULL;
			blinkDiv.lowerLeft = CVector(CGame::GetInstance()->screenWidth/2, userTxtHeight);
			return;
		}

		playerTxt = new CImage(userStr, CGame::GetInstance()->headLineFont, CGame::GetInstance()->color2);

		blinkDiv.lowerLeft = CVector((CGame::GetInstance()->screenWidth - playerTxt->width)/2 + playerTxt->width, userTxtHeight);
	}
}


void CBlinkingDivider::Draw( double width, double height )
{
	currentAlpha+=alphaVel*alphaDir*CGame::GetInstance()->deltaTime;

	if(alphaDir==1)
	{
		if(currentAlpha>1)
			alphaDir = -1;
	}

	if(alphaDir==-1)
		if(currentAlpha<0)
			alphaDir = 1;

	glColor4f(1,1,1,currentAlpha);
	Draw::Rect(lowerLeft + CVector(0, height), width, height);
	glColor3f(1,1,1);

	
}
