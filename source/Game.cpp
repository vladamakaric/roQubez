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

#include "Game.h"
#include "global.h"
#include "TetriminoManager.h"
#include "HighScores.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

CGame::CGame(int _screenWidth, int _screenHeight) : 
	screenWidth(_screenWidth),
	screenHeight(_screenHeight),
	screenRect(CVector(0,0), CVector(screenWidth, screenHeight)),
	maxNameSize(8)
{

	matrixSize = 11;
	srand (time(NULL));
	
	
	color2 = 0x00ff5500;
	color1 = 0x005500ff;

	run = true;

	CTetriminoManager::LoadTetriminos();

	InitGrafix();
	nextState = NULL;
	

	double fontMultiplier = 1;
	headLineFont = TTF_OpenFont("data/SF Square Head.ttf", 86*fontMultiplier);

	

	mainMenuBtnFont = TTF_OpenFont("data/SF Square Head.ttf", 36*fontMultiplier);
	smallBtnFont = TTF_OpenFont("data/SF Square Head.ttf", 18*fontMultiplier);
	copyrightFont = TTF_OpenFont("data/LiberationSans-Regular.ttf", 12);
	creditsFont = TTF_OpenFont("data/SF Square Head Condensed.ttf", 23*fontMultiplier);
	highScoresFont = TTF_OpenFont("data/SF Square Head.ttf", 26*fontMultiplier);

}

void CGame::InitGrafix()
{
	SDL_Init( SDL_INIT_EVERYTHING );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_SetVideoMode( screenWidth, screenHeight, 32, SDL_OPENGL );

	TTF_Init();


    glClearColor( 0, 0, 0, 0 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, screenWidth, 0, screenHeight, -100, 100 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	//glEnable(GL_LINE_SMOOTH);

//  	glEnable(GL_POLYGON_SMOOTH);
  // 	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
 	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA). 
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 	glShadeModel(GL_FLAT);

    SDL_WM_SetCaption( "RoQubez", NULL );
}

void CGame::Run()
{
	int thisTime, lastTime = 0;
	currentState = CMainMenu::GetInstance();

	

	while(run)
	{
		thisTime = SDL_GetTicks();
		deltaTime = (double)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		if(deltaTime>0.1)
			deltaTime = 0.1;

		while(SDL_PollEvent(&event))
			currentState->RegisterEvents();

		
		currentState->Logic();
		if(nextState != NULL)
		{
			currentState = nextState;
			nextState = NULL;
			currentState->Initialize();
			continue;
		}

        glClear(GL_COLOR_BUFFER_BIT);

		currentState->Draw();

        SDL_GL_SwapBuffers();
		SDL_Delay(2);
	}
}

CGame *CGame::GetInstance()
{
	static CGame* instance = NULL;

	if(instance == NULL)
		instance = new CGame(600,600);

	return instance;
}

void CGame::Quit()
{
	CHighScores::GetInstance()->SaveToFile();
	run = false;
}









