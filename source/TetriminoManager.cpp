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

#include "TetriminoManager.h"
#include <sstream> 
#include <iostream>
#include "f_misc.h"
#include <algorithm>
#include <queue>
#include <stdlib.h>
#include <cstring>
using namespace std;

vector<CTetriminoType*> CTetriminoManager::types;

void CTetriminoManager::LoadTetriminos()
{
	FILE *fp = fopen("data/numberOfTTs.txt", "r");
	int numberOfTTs;



	fscanf(fp, "%d", &numberOfTTs);

	for (int i = 0; i < numberOfTTs ; i++)
	{
		CTetriminoType* newTT = new CTetriminoType();

		stringstream ss;
		ss << i + 1;
		string filename = "data/" + ss.str() + ".txt";

		FILE *ttfp = fopen(filename.c_str(), "r");
		bool tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE];

		for (int j = 0; j < T_MATRIX_SIZE ; j++)
			for (int i = 0; i < T_MATRIX_SIZE; i++)
			{
				int val;
				fscanf(ttfp, "%d", &val);
				
				
				tMatrix[i][j] = val;
			}

		vector<CTCube> tCubes;
		PopulateVectorFromMatrix(tCubes, tMatrix);

		types.push_back(GetNewTypeFromTCubeVector(tCubes, tMatrix, false, true, false));

		fclose(ttfp);
	}
}

void CTetriminoManager::PopulateVectorFromMatrix( std::vector<CTCube> &tCubeVector, bool tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE] )
{
	for (int j = 0; j < T_MATRIX_SIZE; j++)
		for (int i = 0; i < T_MATRIX_SIZE; i++)
		{
			if(tMatrix[i][j])
			{
				int ti = i;
				int tj = j;

				//MakeCoordsRelativeToCenterOfSquareMatrix(ti,tj, T_MATRIX_SIZE);
				tCubeVector.push_back(CTCube(ti,tj));

			}
		}
}

void CTetriminoManager::MakeCoordsRelativeToCenterOfSquareMatrix( int &i, int &j, int matrixSize )
{
	i -= matrixSize/2;
	j = ((matrixSize-1) - j) - matrixSize/2;
}

void CTetriminoManager::MakeRelativeCoordsApsoluteToSquareMatrix( int &i, int &j, int matrixSize )
{
	i += matrixSize/2;
	j += matrixSize/2;
	j = matrixSize - 1 - j;	
}

void CTetriminoManager::RotateVectorA90CWToB( const vector<CTCube> &A, vector<CTCube> &B )
{
	vector<CTCube>::const_iterator it = A.begin();
	for(; it != A.end(); it++)
		B.push_back(CTCube( it->j, -it->i ));
}

void CTetriminoManager::RotateFirstVectorToNext3InArray90CW( vector<CTCube> vectorsToRotate[4] )
{
	RotateVectorA90CWToB(vectorsToRotate[0], vectorsToRotate[1]);
	RotateVectorA90CWToB(vectorsToRotate[1], vectorsToRotate[2]);
	RotateVectorA90CWToB(vectorsToRotate[2], vectorsToRotate[3]);
}

void CTetriminoManager::PopulateFeelerVector( vector<CTCube> &_feelerVector, bool _tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], const vector<CTCube> &_tCubes, int _iOffset, int _jOffset )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		bool feelerClear = !F::MISC::ValidIndices(it->i + _iOffset, it->j + _jOffset, T_MATRIX_SIZE, T_MATRIX_SIZE);
		feelerClear |= !_tMatrix[it->i + _iOffset][it->j + _jOffset];

		if(feelerClear)
		{
			int ti = it->i;
			int tj = it->j;

			MakeCoordsRelativeToCenterOfSquareMatrix(ti,tj, T_MATRIX_SIZE);
			_feelerVector.push_back(CTCube(ti + _iOffset, tj - _jOffset)); //relativne j koord su obrnute
		}
	}
}

void CTetriminoManager::RelativiseVectorCoordinates( vector<CTCube> &_coords, int _matrixSize)
{
	vector<CTCube>::iterator it = _coords.begin();
	for(; it != _coords.end(); it++)
		MakeCoordsRelativeToCenterOfSquareMatrix(it->i,it->j, _matrixSize);

}

void CTetriminoManager::UnRelativiseVectorCoordinates( vector<CTCube> &_coords, int _matrixSize )
{
	vector<CTCube>::iterator it = _coords.begin();
	for(; it != _coords.end(); it++)
		MakeRelativeCoordsApsoluteToSquareMatrix(it->i, it->j, _matrixSize);
}


bool compareJ(const CTCube &_A, const CTCube &_B) { return _A.j<_B.j; }
bool compareI(const CTCube &_A, const CTCube &_B) { return _A.i<_B.i; }

void CTetriminoManager::CalculateExtremePoints( CTetriminoType* _tt )
{
	int maxJ = max_element(_tt->tCubes[0].begin(), _tt->tCubes[0].end(), compareJ)->j;	
	int minJ = min_element(_tt->tCubes[0].begin(), _tt->tCubes[0].end(), compareJ)->j;
	int maxI = max_element(_tt->tCubes[0].begin(), _tt->tCubes[0].end(), compareI)->i;
	int minI = min_element(_tt->tCubes[0].begin(), _tt->tCubes[0].end(), compareI)->i;

	_tt->upperMostLocalJ[0] = maxJ;
	_tt->upperMostLocalJ[1] = -minI;
	_tt->upperMostLocalJ[2] = -minJ;
	_tt->upperMostLocalJ[3] = maxI;

	_tt->downMostLocalJ[0] = minJ;
	_tt->downMostLocalJ[1] = -maxI;
	_tt->downMostLocalJ[2] = -maxJ;
	_tt->downMostLocalJ[3] = minI;

	if(_tt->FocusData==NULL)
		return;

	_tt->FocusData->leftMostLocalI[0] = minI;
	_tt->FocusData->leftMostLocalI[1] = minJ;
	_tt->FocusData->leftMostLocalI[2] = -maxI;
	_tt->FocusData->leftMostLocalI[3] = -maxJ;

	_tt->FocusData->rightMostLocalI[0] = maxI;
	_tt->FocusData->rightMostLocalI[1] = maxJ;
	_tt->FocusData->rightMostLocalI[2] = -minI;
	_tt->FocusData->rightMostLocalI[3] = -minJ;	
}

void CTetriminoManager::RotateVectorAToB( const vector<CTCube> &A, vector<CTCube> &B, void (*RAroundFunc)(int &i, int &j) )
{
	vector<CTCube>::const_iterator it = A.begin();
	for(; it != A.end(); it++)
	{
		int ti = it->i;
		int tj = it->j;

		(*RAroundFunc)(ti,tj);

		B.push_back(CTCube( ti, tj ));
	}
}

CTetriminoType* CTetriminoManager::GetNewTypeFromTCubeVector( 
	const vector<CTCube> &_tCubes, 
	bool _tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], 
	bool _centralCoords /*= false*/, 
	bool _focus /*= false*/, 
	bool _emptyMatrix /*= true */ )
{
	CTetriminoType* newTT = new CTetriminoType();
	vector<CTCube> *leftFeelers, *rightFeelers;

	newTT->tCubes[0] = _tCubes;
	
	if(_emptyMatrix)
	{
		memset (_tMatrix, 0, T_MATRIX_SIZE*T_MATRIX_SIZE);

		if(_centralCoords)
			UnRelativiseVectorCoordinates(newTT->tCubes[0], T_MATRIX_SIZE);

		_centralCoords = false;

		PopulateMatrixFromVector(_tMatrix, newTT->tCubes[0]);
	}

	if(_centralCoords) 
		UnRelativiseVectorCoordinates(newTT->tCubes[0], T_MATRIX_SIZE);

	if(!_focus)
	{
		newTT->FocusData = NULL;
		leftFeelers = new vector<CTCube>();
		rightFeelers = new vector<CTCube>();
	}
	else
	{
		newTT->FocusData = new CTTFocusData();
		leftFeelers = &newTT->FocusData->leftCollisionFeelers[0];
		rightFeelers = &newTT->FocusData->rightCollisionFeelers[0];
	}

	PopulateFeelerVector(newTT->bottomCollisionFeelers[0], _tMatrix, newTT->tCubes[0], 0, 1);
	PopulateFeelerVector(newTT->uppperCollisionFeelers[0], _tMatrix, newTT->tCubes[0], 0, -1);

	PopulateFeelerVector(*leftFeelers, _tMatrix, newTT->tCubes[0], -1, 0);
	PopulateFeelerVector(*rightFeelers, _tMatrix, newTT->tCubes[0], 1, 0);
	

	////////bottom
	RotateVectorAToB(*rightFeelers, newTT->bottomCollisionFeelers[1], F::MISC::RAroundOrigin90CW);
	RotateVectorAToB(newTT->uppperCollisionFeelers[0], newTT->bottomCollisionFeelers[2], F::MISC::RAroundOrigin180CW);
	RotateVectorAToB(*leftFeelers, newTT->bottomCollisionFeelers[3], F::MISC::RAroundOrigin90CCW);

	///////upper
	RotateVectorAToB(*leftFeelers, newTT->uppperCollisionFeelers[1], F::MISC::RAroundOrigin90CW);
	RotateVectorAToB(newTT->bottomCollisionFeelers[0], newTT->uppperCollisionFeelers[2], F::MISC::RAroundOrigin180CW);
	RotateVectorAToB(*rightFeelers, newTT->uppperCollisionFeelers[3], F::MISC::RAroundOrigin90CCW);

	/////////////////////////
	RelativiseVectorCoordinates(newTT->tCubes[0], T_MATRIX_SIZE);
	RotateFirstVectorToNext3InArray90CW(newTT->tCubes);

	if(_focus)
	{
		newTT->FocusData = new CTTFocusData();
		newTT->FocusData->rightCollisionFeelers[0] = *rightFeelers;
		newTT->FocusData->leftCollisionFeelers[0] = *leftFeelers;

		///////left
		RotateVectorAToB(newTT->bottomCollisionFeelers[0], newTT->FocusData->leftCollisionFeelers[1], F::MISC::RAroundOrigin90CW);
		RotateVectorAToB(newTT->FocusData->rightCollisionFeelers[0], newTT->FocusData->leftCollisionFeelers[2], F::MISC::RAroundOrigin180CW);
		RotateVectorAToB(newTT->uppperCollisionFeelers[0], newTT->FocusData->leftCollisionFeelers[3], F::MISC::RAroundOrigin90CCW);

		////right
		RotateVectorAToB(newTT->bottomCollisionFeelers[0], newTT->FocusData->rightCollisionFeelers[3], F::MISC::RAroundOrigin90CCW);
		RotateVectorAToB(newTT->FocusData->leftCollisionFeelers[0], newTT->FocusData->rightCollisionFeelers[2], F::MISC::RAroundOrigin180CW);
		RotateVectorAToB(newTT->uppperCollisionFeelers[0], newTT->FocusData->rightCollisionFeelers[1], F::MISC::RAroundOrigin90CW);
		///////////////////////////////////////////////////////////

		for(int i=0; i<4; i++)
		{
			vector<CTCube> downTempFeelers = newTT->bottomCollisionFeelers[i];
			vector<CTCube> leftTempFeelers = newTT->FocusData->leftCollisionFeelers[i];
			vector<CTCube> rightTempFeelers = newTT->FocusData->rightCollisionFeelers[i];

			newTT->FocusData->leftBottomCollisionFeelers[i] = downTempFeelers;
			newTT->FocusData->rightBottomCollisionFeelers[i] = downTempFeelers;

			OffsetTCubes(newTT->FocusData->leftBottomCollisionFeelers[i], -1, 0);
			OffsetTCubes(newTT->FocusData->rightBottomCollisionFeelers[i], 1, 0);

			ComplementVectorAWithB(newTT->FocusData->leftBottomCollisionFeelers[i], downTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->rightBottomCollisionFeelers[i], downTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->leftBottomCollisionFeelers[i], newTT->tCubes[i]);
			ComplementVectorAWithB(newTT->FocusData->rightBottomCollisionFeelers[i], newTT->tCubes[i]);

			///////
			newTT->FocusData->BottomLeftCollisionFeelers[i] = leftTempFeelers;
			newTT->FocusData->BottomRightCollisionFeelers[i] = rightTempFeelers;
			newTT->FocusData->UpperLeftCollisionFeelers[i] = leftTempFeelers;
			newTT->FocusData->UpperRightCollisionFeelers[i] = rightTempFeelers;

			OffsetTCubes(newTT->FocusData->BottomLeftCollisionFeelers[i], 0, -1);
			OffsetTCubes(newTT->FocusData->BottomRightCollisionFeelers[i], 0, -1);
			OffsetTCubes(newTT->FocusData->UpperLeftCollisionFeelers[i], 0, 1);
			OffsetTCubes(newTT->FocusData->UpperRightCollisionFeelers[i], 0, 1);

			ComplementVectorAWithB(newTT->FocusData->BottomLeftCollisionFeelers[i], leftTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->BottomRightCollisionFeelers[i], rightTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->UpperLeftCollisionFeelers[i], leftTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->UpperRightCollisionFeelers[i], rightTempFeelers);
			ComplementVectorAWithB(newTT->FocusData->BottomLeftCollisionFeelers[i], newTT->tCubes[i]);
			ComplementVectorAWithB(newTT->FocusData->BottomRightCollisionFeelers[i], newTT->tCubes[i]);
			ComplementVectorAWithB(newTT->FocusData->UpperLeftCollisionFeelers[i], newTT->tCubes[i]);
			ComplementVectorAWithB(newTT->FocusData->UpperRightCollisionFeelers[i], newTT->tCubes[i]);
			///////
		}
	}

	CalculateExtremePoints(newTT);

	return newTT;
}

CTetriminoType* CTetriminoManager::GetNewTypeFromTCubeVector( const vector<CTCube> &_tCubes )
{
	bool matrix[T_MATRIX_SIZE][T_MATRIX_SIZE];
	return GetNewTypeFromTCubeVector(_tCubes,matrix, true, false, true);
}

void CTetriminoManager::PopulateMatrixFromVector( bool tMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE], vector<CTCube> &tCubeVector )
{
	vector<CTCube>::iterator it = tCubeVector.begin();
	for(; it != tCubeVector.end(); it++)
		tMatrix[it->i][it->j] = true;
}

bool CTetriminoManager::TCubeExistsInVector( const CTCube &_tc, const vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(_tc.i == it->i && _tc.j == it->j)
			return true;
	}

	return false;
}

void CTetriminoManager::ComplementVectorAWithB( vector<CTCube> &_A, const vector<CTCube> &_B )
{
	vector<CTCube>::iterator it = _A.begin();
	for(; it != _A.end();)
	{
		if(TCubeExistsInVector(*it, _B))
			it = _A.erase(it);
		else
			it++;
	}
}

void CTetriminoManager::OffsetTCubes( vector<CTCube> &_tCubes, int i, int j )
{
	vector<CTCube>::iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		it->i += i;
		it->j += j;
	}
}

void CTetriminoManager::PartitionTCubesIntoClusters( vector<vector<CTCube> > &_clusters, vector<CTCube> &_tCubes )
{
	bool matrix[T_MATRIX_SIZE][T_MATRIX_SIZE];
	memset(matrix, 0, T_MATRIX_SIZE*T_MATRIX_SIZE);
	UnRelativiseVectorCoordinates(_tCubes, T_MATRIX_SIZE);
	PopulateMatrixFromVector(matrix, _tCubes);

	queue<CTCube> ffQueue;
	vector<CTCube>::iterator it = _tCubes.begin();

	while(!_tCubes.empty())
	{
		ffQueue.push(*it);

		_clusters.push_back(vector<CTCube>());

		while(!ffQueue.empty())
		{
			CTCube temp = ffQueue.front();	
			ffQueue.pop();

			if(F::MISC::ValidIndicesInSquareMatrix(temp.i, temp.j, T_MATRIX_SIZE))
			if(matrix[temp.i][temp.j])
			{
				_clusters.rbegin()->push_back(temp);

				matrix[temp.i][temp.j] = false;
				
				ffQueue.push(CTCube(temp.i-1, temp.j));
				ffQueue.push(CTCube(temp.i, temp.j-1));
				ffQueue.push(CTCube(temp.i+1, temp.j));
				ffQueue.push(CTCube(temp.i, temp.j+1));
			}

			
		}

		RelativiseVectorCoordinates(*_clusters.rbegin(), T_MATRIX_SIZE);

		_tCubes.clear();
		PopulateVectorFromMatrix(_tCubes, matrix);

		if(_tCubes.empty()) return;

		it = _tCubes.begin();
	}
}

void CTetriminoManager::CetraliseRelativeTCubesVector( int &_globalI, int &_globalJ , vector<CTCube> &_tCubes )
{






}




