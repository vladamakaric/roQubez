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

#ifndef SQUARE_H
#define SQUARE_H

#include "Vector.h"
#include "f_misc.h"

class CSquare
{
public:
	CSquare(const CVector &_downLeft, const CVector &_upRight, bool _sort=false)
	{
		Set(_downLeft, _upRight, _sort);
	}

	CSquare(){};

	void Move(const CVector &_offset)
	{
		upRight+=_offset;
		downLeft+=_offset;
	}

	void SetBottomLeftAndUpdate(const CVector &_btleft)
	{
		downLeft = _btleft;
		upRight = downLeft;

		upRight += CVector(width, height);
	}

	void Set(const CVector &_downLeft, const CVector &_upRight, bool _sort=false)
	{
		if(!_sort)
		{
			upRight = _upRight;
			downLeft = _downLeft;
			CalculateWH();
			return;
		}

		double x1,x2,y1,y2;

		x1 = _upRight.x;
		x2 = _downLeft.x;
		y1 = _upRight.y;
		y2 = _downLeft.y;

		upRight.Set(F::MISC::Max(x1,x2), F::MISC::Max(y1,y2));
		downLeft.Set(F::MISC::Min(x1,x2), F::MISC::Min(y1,y2));

		CalculateWH();
	}

	void CalculateWH()
	{
		width = upRight.x - downLeft.x;
		height = upRight.y - downLeft.y;
	}

	double width, height;
	CVector upRight,downLeft;
};

#endif