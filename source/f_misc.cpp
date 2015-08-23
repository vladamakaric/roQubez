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

#include "f_misc.h"
#include "f_intersection.h"

#define _USE_MATH_DEFINES
#include "math.h"

double F::MISC::ToRadians( double degrees )
{
	return (degrees/360)*2*M_PI;
}

double F::MISC::ToDegrees( double radians )
{
	return radians/(2*M_PI)*360;
}

bool F::MISC::ValidIndices( int i, int j, int matrixILenght, int matrixJLenght)
{
	return i>=0 && j>=0 && i<matrixILenght && j<matrixJLenght;
}

bool F::MISC::ValidIndexInSquareMatrix( int i, int matrixILenght) {return i>=0 && i<matrixILenght;}

bool F::MISC::ValidIndicesInSquareMatrix( int i, int j, int matrixILenght) 
{ 
	return ValidIndexInSquareMatrix(i, matrixILenght) && ValidIndexInSquareMatrix(j, matrixILenght); 
}

int F::MISC::ModuloOperator(int n, int positiveBase)  // a mod b
{
	if(n<0)
	{
		int tempN = -n;
		int numberOfTimesInBase = tempN/positiveBase + 1;
		return n + numberOfTimesInBase*positiveBase;
	}

	return n % positiveBase;
}

double F::MISC::RealPositiveModulo( double _positiveR, double _positiveBase )
{
	int numTimes = _positiveR/_positiveBase;
	_positiveR -= _positiveBase*numTimes;
	return _positiveR;
}

bool F::MISC::DoublesEqual(double _a, double _b, double _precision)
{
	return fabs(_a - _b) < _precision;
}