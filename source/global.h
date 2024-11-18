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

#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __linux__
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_ttf.h"
#elif defined _WIN32 || defined _WIN64 || defined __APPLE__
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#else
#error "unknown platform"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern SDL_Event event;
extern const Uint8 *keystates;// = SDL_GetKeyState( NULL );

#endif
