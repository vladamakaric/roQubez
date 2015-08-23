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

#include "About.h"
#include "Game.h"
#include "MainMenu.h"
#include "Square.h"

#define BTN_W 300
CAbout::CAbout() : 
	mainMenu(CVector((CGame::GetInstance()->screenWidth - BTN_W)/2, 100), 
	"main menu", CGame::GetInstance()->mainMenuBtnFont, BTN_W, 50),
	text("programmed by Vladimir Makaric, vladaserbia@gmail.com", CGame::GetInstance()->creditsFont, 0xffffffff)
{

}

CAbout* CAbout::GetInstance()
{
	static CAbout* instance = NULL;

	if(instance == NULL)
		instance = new CAbout();

	return instance;
}

void CAbout::Draw()
{
	text.DrawInRectCenter(CSquare(CVector(0,0), CVector(CGame::GetInstance()->screenWidth, CGame::GetInstance()->screenHeight)));
	mainMenu.Draw();
}

void CAbout::RegisterEvents()
{
	if( event.type == SDL_MOUSEMOTION )
	{
			mainMenu.MouseMotionEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		mainMenu.MouseClickEventHandling();
	}

	if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();
}

void CAbout::Initialize()
{
	mainMenu.ResetButton();
}

void CAbout::Logic()
{
	if(mainMenu.mMouseClick)
		CGame::GetInstance()->nextState = CMainMenu::GetInstance();
}