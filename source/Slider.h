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

#ifndef SLIDER_H
#define SLIDER_H

#include "Square.h"
#include "Vector.h"



class CSlider
{
public:
	CSlider(const CVector &_bottomLeft, double _width, double _height, double _sliderW, double _sliderOutset ,double _startValue, double _endValue, int _numberOfValsInBetween, double _currentVal );


	void UpdateSliderPosition();

	void MouseMotionEventHandling();
	void MouseDownEventHandling();
	void MouseUpEventHandling();
	void Reset();
	CVector prevMousePos;
	bool mouseOverLeft;
	bool mouseOverRight;
	bool mouseOverSlider;
	bool mouseDownSlider;

	void Draw();


	bool mouseDown;
	double startValue, endValue, currentValue;
	int numOfValsInBetween;
	double stepSize;
	double width;
	double height;
	CSquare slider;
	
	CSquare left, right;
	CVector bottomLeft;
	CSquare middle;




};






#endif