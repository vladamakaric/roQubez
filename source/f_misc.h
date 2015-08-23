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

#ifndef MISC_H
#define MISC_H

#include "Vector.h"
#include <vector>


using namespace std;

namespace F
{
	namespace MISC
	{
		template<class T> inline T MinByRef(T& _a,T& _b)  { return _a > _b ? _b : _a; }
		template<class T> inline T MaxByRef(T& _a,T& _b)  { return _a > _b ? _a : _b; }
		template<class T> inline T Min(T _a,T _b)  { return _a > _b ? _b : _a; }
		template<class T> inline T Max(T _a,T _b)  { return _a > _b ? _a : _b; }

		template<class T> inline void CutOffValue(T &value, T limit) { if(value > limit) value = limit; }

		template<class T> inline void ConstrainValueToInterval(T &value, T min, T max) 
		{ if(value > max) value = max; else if(value<min) value = min; }

		template<class T> inline void RAroundOrigin90CW( T &_i, T &_j )
		{
			T _tempI = _i;
			_i = _j;
			_j = -_tempI;
		}

		template<class T> inline void RAroundOrigin90CCW( T &_i, T &_j )
		{
			T _tempI = _i;
			_i = -_j;
			_j = _tempI;
		}

		template<class T> inline void RAroundOrigin180CCW( T &_i, T &_j )
		{
			RAroundOrigin90CCW(_i, _j);
			RAroundOrigin90CCW(_i, _j);
		}

		template<class T> inline void RAroundOrigin180CW( T &_i, T &_j )
		{
			RAroundOrigin90CW(_i, _j);
			RAroundOrigin90CW(_i, _j);
		}

		template<class T> inline void Swap(T &_a, T &_b) 
		{
			T temp = _a;
			_a = _b;
			_b = temp;
		}

		bool DoublesEqual(double _a, double _b, double _precision);

		double RealPositiveModulo(double _positiveN, double _positiveBase );
		int ModuloOperator(int n, int positiveBase); // a mod b

		bool ValidIndices( int i, int j, int matrixILenght, int matrixJLenght);
		bool ValidIndexInSquareMatrix( int i, int matrixILenght);
		bool ValidIndicesInSquareMatrix( int i, int j, int matrixILenght);	
		double ToRadians(double degrees);
		double ToDegrees(double radians);
	}
}

#endif