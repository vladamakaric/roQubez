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

#ifndef TETRIMINO_TYPE
#define TETRIMINO_TYPE

#include <vector>
#include "TCube.h"

struct CTTFocusData
{
	std::vector<CTCube> rightCollisionFeelers[4];
	std::vector<CTCube> leftCollisionFeelers[4];

	///////////
	std::vector<CTCube> rightBottomCollisionFeelers[4];
	std::vector<CTCube> leftBottomCollisionFeelers[4];



	std::vector<CTCube> BottomLeftCollisionFeelers[4];
	std::vector<CTCube> BottomRightCollisionFeelers[4];
	std::vector<CTCube> UpperRightCollisionFeelers[4];
	std::vector<CTCube> UpperLeftCollisionFeelers[4];
	///////////


	int leftMostLocalI[4];
	int rightMostLocalI[4];
};

class CTetriminoType
{
public:
	void LoadType(){};
	std::vector<CTCube> tCubes[4];
	std::vector<CTCube> bottomCollisionFeelers[4];
	std::vector<CTCube> uppperCollisionFeelers[4];


	int downMostLocalJ[4];
	int upperMostLocalJ[4];
	CTTFocusData *FocusData;
};





#endif