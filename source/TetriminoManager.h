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


#ifndef TETRIMINO_MANAGER_H
#define TETRIMINO_MANAGER_H

#include <vector>
#include <stdio.h>

#define T_MATRIX_SIZE 5

#include "TetriminoType.h"
#include "TCube.h"
using namespace std;

// I i J pocinju od 0,0 koji je u gornjem levom cosku matrice, I je kao X koordinata, znaci broji kolone, a J je ko y

class CTetriminoManager
{
public:
	CTetriminoManager(){}
	static void LoadTetriminos();
	static void PopulateVectorFromMatrix(vector<CTCube> &tCubeVector, bool tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE]);
	static void PopulateMatrixFromVector(bool tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], vector<CTCube> &tCubeVector);
	static void PopulateFeelerVector(vector<CTCube> &_feelerVector, bool _tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], const vector<CTCube> &_tCubes, int _iOffset, int _jOffset);
	

	static void RotateVectorA90CWToB(const vector<CTCube> &A, vector<CTCube> &B);

	static CTetriminoType* GetNewTypeFromTCubeVector(const vector<CTCube> &_tCubes);

	static CTetriminoType* GetNewTypeFromTCubeVector(const vector<CTCube> &_tCubes, bool _tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], 
	bool _centralCoords = false, bool _focus = false, bool _emptyMatrix = true );


	static void PartitionTCubesIntoClusters( vector<vector<CTCube> > &_clusters, vector<CTCube> &_tCubes );

	
	static void CetraliseRelativeTCubesVector(int &_globalI, int &_globalJ , vector<CTCube> &_tCubes);



	static void RotateVectorAToB(const vector<CTCube> &A, vector<CTCube> &B, void (*RAroundFunc)(int &i, int &j));
	static void RotateFirstVectorToNext3InArray90CW(vector<CTCube> vectorsToRotate[4]);
	static void MakeCoordsRelativeToCenterOfSquareMatrix(int &i, int &j, int matrixSize); //square matrix
	static void MakeRelativeCoordsApsoluteToSquareMatrix(int &i, int &j, int matrixSize); //square matrix

	static void CalculateExtremePoints(CTetriminoType* _tt);

	static void UnRelativiseVectorCoordinates(vector<CTCube> &_coords, int _matrixSize);
	static void RelativiseVectorCoordinates(vector<CTCube> &_coords, int _matrixSize);

	static void OffsetTCubes(vector<CTCube> &_tCubes, int i, int j);
	static bool TCubeExistsInVector(const CTCube &_tc, const vector<CTCube> &_tCubes);
	static void ComplementVectorAWithB(vector<CTCube> &_A, const vector<CTCube> &_B); //A-=B

	static std::vector<CTetriminoType*> types;
};

#endif

