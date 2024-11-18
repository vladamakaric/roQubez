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

#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "State.h"
#include "MainMenu.h"
#include "global.h"
#include "Color.h"
#include "Square.h"


class CGame
{
public:
	int screenHeight, screenWidth;
	CState *currentState, *nextState;
	double deltaTime;
	bool run;
	int matrixSize;
	int maxNameSize;
	CSquare screenRect;

	void Run();
	void InitGrafix();
	void Quit();;


	UColor color1, color2;
	static CGame *GetInstance();
	

	SDL_Window* window;
	SDL_GLContext glContext;
	TTF_Font* headLineFont;
	TTF_Font* mainMenuBtnFont;
	TTF_Font* highScoresFont;
	TTF_Font* scoreFont;
	TTF_Font* creditsFont;
	TTF_Font* smallBtnFont;
private:
	CGame(int _screenWidth, int _screenHeight);
};


#endif