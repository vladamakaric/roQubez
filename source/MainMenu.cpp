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

#include "MainMenu.h"
#include "global.h"
#include "Game.h"
#include "Level.h"
#include "f_graphics.h"
#include "About.h"
#include "Optinos.h"
#include "HighScores.h"

#define BTN_WIDTH 300
#define BTN_HEIGHT 50

CMainMenu::CMainMenu() : 
	headLine("RoQubez", CGame::GetInstance()->headLineFont , 0x00ffffff),
	copyrightLine("Copyright© 2013 Vladimir Makaric", CGame::GetInstance()->copyrightFont , 0x00ffffff),
	inset((CGame::GetInstance()->screenWidth - headLine.width - 20)/2)
{
	double vertSpaceBetweenBtns = (CGame::GetInstance()->screenHeight - inset*3 - headLine.height - BTN_HEIGHT*5)/4;
	double btnHorizInset = (CGame::GetInstance()->screenWidth - BTN_WIDTH)/2;

	exit = CRotatingTextButton(CVector(btnHorizInset, inset),"exit", CGame::GetInstance()->mainMenuBtnFont, BTN_WIDTH, BTN_HEIGHT);
	about = CRotatingTextButton(CVector(btnHorizInset, inset + BTN_HEIGHT + vertSpaceBetweenBtns),"about", CGame::GetInstance()->mainMenuBtnFont, BTN_WIDTH, BTN_HEIGHT);
	highScores = CRotatingTextButton(CVector(btnHorizInset, inset + 2*BTN_HEIGHT + 2*vertSpaceBetweenBtns),"high scores", CGame::GetInstance()->mainMenuBtnFont, BTN_WIDTH, BTN_HEIGHT);
	options = CRotatingTextButton(CVector(btnHorizInset, inset + 3*BTN_HEIGHT + 3*vertSpaceBetweenBtns), "options", CGame::GetInstance()->mainMenuBtnFont, BTN_WIDTH, BTN_HEIGHT);
	newGame = CRotatingTextButton(CVector(btnHorizInset, inset + 4*BTN_HEIGHT + 4*vertSpaceBetweenBtns),"new game", CGame::GetInstance()->mainMenuBtnFont, BTN_WIDTH, BTN_HEIGHT);

	buttons.push_back(&exit);
	buttons.push_back(&about);
	buttons.push_back(&highScores);
	buttons.push_back(&options);
	buttons.push_back(&newGame);
}

CMainMenu* CMainMenu::GetInstance() 
{
	static CMainMenu* instance = NULL;

	if(instance == NULL)
		instance = new CMainMenu();

	return instance;
}

void CMainMenu::RegisterEvents()
{
	if( event.type == SDL_MOUSEMOTION )
	{
		for(vector<CRotatingTextButton*>::iterator btn = buttons.begin(); btn!=buttons.end(); ++btn)
			(*btn)->MouseMotionEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		for(vector<CRotatingTextButton*>::iterator btn = buttons.begin(); btn!=buttons.end(); ++btn)
			(*btn)->MouseClickEventHandling();
	}

    if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();
}

void CMainMenu::Logic()
{
	if(exit.mMouseClick)
	{
		CGame::GetInstance()->Quit();
		return;
	}

	if(newGame.mMouseClick)
		CGame::GetInstance()->nextState = CLevel::GetInstance();

	if(about.mMouseClick)
		CGame::GetInstance()->nextState = CAbout::GetInstance();

	if(options.mMouseClick)
		CGame::GetInstance()->nextState = COptions::GetInstance();

	if(highScores.mMouseClick)
		CGame::GetInstance()->nextState = CHighScores::GetInstance();
}

void CMainMenu::Initialize()
{
	for(vector<CRotatingTextButton*>::iterator btn = buttons.begin(); btn!=buttons.end(); ++btn)
		(*btn)->ResetButton();
}


#include "f_color.h"
void CMainMenu::Draw()
{

	

	glPushMatrix();

	CVector mousePos(event.button.x, CGame::GetInstance()->screenHeight - event.button.y);

	glTranslatef(inset, CGame::GetInstance()->screenHeight - inset, 0);


 	F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color2);
 	headLine.Draw(CVector(0,0));

 	glColor3f(1,1,1);
 	headLine.Draw(CVector(5,5));

	glColor3f(1,1,1);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	
	

	glPopMatrix();


	

	copyrightLine.Draw(CVector((CGame::GetInstance()->screenWidth - copyrightLine.width)/2, 40));
	//headLine.DrawInRectCenter(exit.mRect);


	for(vector<CRotatingTextButton*>::iterator btn = buttons.begin(); btn!=buttons.end(); ++btn)
		(*btn)->Draw();
}
