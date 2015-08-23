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

#ifndef IMAGE_H
#define IMAGE_H

#include "Vector.h"
#include "global.h"
#include "Color.h"
#include "Square.h"

class CImage
{
public:
	CImage(){}
	CImage(const std::string& _text, const TTF_Font *_font, UColor _txtColor);
	CImage(const std::string& _imageFile);
	~CImage();

	SDL_Surface* texture;
	void Draw(const CVector &_topLeft);
	void Draw(const CVector &_topLeft, int _width, int _height);
	void DrawInRectCenter(const CSquare &_rect);



	GLuint textureID;	
	int powWidth;
	int powHeight;
	int width;
	int height;
};

#endif