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

#ifndef BUTTON_H
#define BUTTON_H

#include "Square.h"
#include "Vector.h"
#include "Image.h"
#include <string>

class CRotatingTextButton
{
public:
	CSquare mRect;
	CRotatingTextButton(){}
	CRotatingTextButton( const CVector _position, const std::string & _text, TTF_Font* _font,double _width, double _height );

	int rotateDir;
	bool beginRotate;
	bool rotate;
	CImage *btnText;
	double maxAnglularVel;
	double minAngularVel;
	double anglularVel;
	double angularAcc;
	double alpha;


	bool mMouseOver, mMouseClick;
	void (*mMouseOverCallBack)();
	void (*mMouseClickCallBack)();
	void MouseMotionEventHandling();
	void MouseClickEventHandling();
	void ResetButton();
	double angle;
	void Draw();
};



#endif