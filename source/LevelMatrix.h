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
#ifndef LEVEL_MATRIX
#define LEVEL_MATRIX

#include "TCube.h"
#include "Vector.h"
#include <vector>
#include <set>

class CTetrimino;

class CLevelMatrix
{
public:
	CLevelMatrix(const CVector &_bottomLeft, int _dim, int _tileSize);
	void ClearDynamicTetriminos();
	void AddDynamicTetrimino(CTetrimino* _dynamicTetrimino);
	void ClearTetrimino(CTetrimino* _tetrimino);
	void RemoveAllTCubesUpwardOfMatrix(std::vector<CTCube> &_tCubes);
	bool ValidTCubesPlacement(const std::vector<CTCube> &_tCubes);
	bool ValidTCubePlacement(const CTCube &_tc);
	bool TCubeInBounds(const CTCube &_tc);
	bool TCubeSidewaysInBounds(const CTCube &_tc);
	bool ColumnInBounds(int _i);
	bool RowInBoundsFromBelow(int _j);
	bool TCubesInMatrixCollisionWithStaticTetriminos(const std::vector<CTCube> &_tCubes);
	bool TCubesInMatrixCollisionWithTetriminos(const std::vector<CTCube> &_tCubes);
	bool TCubesIntersectTetriminos(const std::vector<CTCube> &_tCubes);
	bool TCubesIntersectStaticTetriminos(const std::vector<CTCube> &_tCubes);
	bool TCubesIntersectDynamicTetriminos(const std::vector<CTCube> &_tCubes);
	bool TCubesSidewaysOutOfBounds(const std::vector<CTCube> &_tCubes);

	bool BlockedFromBellowByStaticTCubes(CTetrimino* _tetrimino);
	bool BlockedFromBellowByNonDynamicTCubes(CTetrimino* _tetrimino);
	void UpdateTetrimino(CTetrimino* _terimino);
	void GetTetriminosIntersectedByTCubes(vector<CTetrimino*> &_tetriminos, const vector<CTCube> &_tCubes);

	void HandleFullRows();

	void PrintToConsole();
	void Clear();

	bool FullStaticRow(int _row);
	
	bool HighestRowOccupied();

	CTetrimino* GetTetriminoAtTCube(const CTCube &_tc);

	CVector bottomLeft;
	CTCube instantiationCube;
	CVector instantiationPosition;
	CTetrimino ***cubeMatrix;

	void AddTetriminoRowsToPotentialFullRows(CTetrimino *_tetrimino);




	bool *potentialFullRows;
	std::set<int> fullStaticRows;

	int dimension;
	int tileSize;
	bool **stateMatrix;
	bool tetriminoOutOfMatrixFreeze;

	CVector matrixCenter;
	void Draw();
};


#endif