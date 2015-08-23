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

#ifndef F_GRAPHICS_H 
#define F_GRAPHICS_H

#include "global.h"
#include <string>
#include "Color.h"
#include "Vector.h"

namespace F
{
	namespace GRAPHICS
	{
		GLuint GGetGLTextureIDFromSDL_Surface(SDL_Surface *_surface, int &_PO2width, int &_PO2height);
		GLuint GenerateGLTextureFromTTFText(const TTF_Font *_font, UColor color,const std::string& _text, int &_width, int &_height, int &_powWidth, int &_powHeight );
		GLuint GetGLTextureIDFromSDL_Surface(SDL_Surface *_surface);
		void DrawTexture(const CVector &_topLeft, GLuint _textureid,int _width, int _height);
		void ChangeGLColor(UColor color);
	}	
}


#endif