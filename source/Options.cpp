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

#include "Optinos.h"
#include "Game.h"
#include "MainMenu.h"
#include "Square.h"
#include <sstream>

#define BTN_W 250

COptions::COptions() : 
	mainMenu(CVector((CGame::GetInstance()->screenWidth - BTN_W)/2, 35), 
	"main menu", CGame::GetInstance()->mainMenuBtnFont, BTN_W, 50),
	instructionLine1("LEFT/RIGHT: horizontal tetrimino movement", CGame::GetInstance()->creditsFont, 0xffffffff),
	instructionLine2("UP: CCW 90 degree tetrimino rotation", CGame::GetInstance()->creditsFont, 0xffffffff),
	instructionLine3("DOWN: accelerate tetrimino free fall", CGame::GetInstance()->creditsFont, 0xffffffff),
	instructionLine4("Q/W: CW/CCW 90 degree level rotation", CGame::GetInstance()->creditsFont, 0xffffffff),
	controlsTxt("CONTROLS:", CGame::GetInstance()->mainMenuBtnFont, 0xffffffff),
	levelMatrixSizeTxt("Change level matrix size:", CGame::GetInstance()->mainMenuBtnFont, 0xffffffff),
	matrixSizeSl(CVector(67, 410), 270, 40, 23, 6, 9, 19, 4, CGame::GetInstance()->matrixSize)
{
	stringstream ss;
	ss << matrixSizeSl.currentValue;
	matrixSizeSlTxt = new CImage(ss.str(), CGame::GetInstance()->headLineFont, CGame::GetInstance()->color2);
}

COptions* COptions::GetInstance()
{
	static COptions* instance = NULL;

	if(instance == NULL)
		instance = new COptions();

	return instance;
}

void COptions::Draw()
{
	glColor3f(1,1,1);
	CSquare screenRect(CVector(0,0), CVector(CGame::GetInstance()->screenWidth, CGame::GetInstance()->screenHeight));
	controlsTxt.DrawInRectCenter(screenRect);


	screenRect.upRight.y -= 50; 
	instructionLine1.DrawInRectCenter(screenRect);
	screenRect.upRight.y -= 40; 
	instructionLine2.DrawInRectCenter(screenRect);
	screenRect.upRight.y -= 40; 
	instructionLine3.DrawInRectCenter(screenRect);
	screenRect.upRight.y -= 40; 
	instructionLine4.DrawInRectCenter(screenRect);

	screenRect.upRight.y =  CGame::GetInstance()->screenHeight; 
	screenRect.downLeft.y =  CGame::GetInstance()->screenHeight-100;
	screenRect.CalculateWH();
	levelMatrixSizeTxt.DrawInRectCenter(screenRect);
	mainMenu.Draw();

	matrixSizeSl.Draw();

	screenRect.downLeft = CVector(CGame::GetInstance()->screenWidth/2 + 20, CGame::GetInstance()->screenHeight/2 - 50);
	screenRect.CalculateWH();


	glColor3f(1,1,1);
	matrixSizeSlTxt->DrawInRectCenter(screenRect);
}

void COptions::RegisterEvents()
{
	if( event.type == SDL_MOUSEMOTION )
	{
		mainMenu.MouseMotionEventHandling();
		matrixSizeSl.MouseMotionEventHandling();

	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		mainMenu.MouseClickEventHandling();
		matrixSizeSl.MouseDownEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONUP)
	{
		matrixSizeSl.MouseUpEventHandling();
	}

	if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();
}

void COptions::Initialize()
{
	mainMenu.ResetButton();
	matrixSizeSl.Reset();
}

void COptions::Logic()
{
	if(CGame::GetInstance()->matrixSize!=matrixSizeSl.currentValue)
	{
		CGame::GetInstance()->matrixSize = matrixSizeSl.currentValue;

		stringstream ss;
		ss << matrixSizeSl.currentValue;
		delete matrixSizeSlTxt;
		matrixSizeSlTxt = new CImage(ss.str(), CGame::GetInstance()->headLineFont, CGame::GetInstance()->color2);
	}

	if(mainMenu.mMouseClick)
		CGame::GetInstance()->nextState = CMainMenu::GetInstance();
}