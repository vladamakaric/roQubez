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

#include "LevelMatrix.h"
#include "draw.h"
#include "Tetrimino.h"
#include "f_misc.h"

CLevelMatrix::CLevelMatrix(const CVector &_topLeft, int _dim, int _tileSize) : bottomLeft(_topLeft), dimension(_dim), tileSize(_tileSize)
{
	instantiationCube.i = _dim/2;
	instantiationCube.j = _dim + 0;
	tetriminoOutOfMatrixFreeze = false;

	potentialFullRows = new bool[dimension];
	memset(potentialFullRows, 0, dimension*sizeof(bool));

	instantiationPosition = bottomLeft + CVector( int(_dim/2)*_tileSize, (instantiationCube.j+1)*tileSize);

	matrixCenter = bottomLeft + CVector( double(dimension/2.0)*tileSize,  double(dimension/2.0)*tileSize);

	cubeMatrix = new CTetrimino**[dimension];
	for(int i=0; i<dimension; i++)
		cubeMatrix[i] = new CTetrimino*[dimension];

	Clear();
}

void CLevelMatrix::Draw()
{

	glLineWidth(2);


	glColor3f(1,1,1);

	glPushMatrix();
	Draw::Line(bottomLeft, CVector(bottomLeft.x + dimension*tileSize, bottomLeft.y));
	glTranslatef(0,dimension*tileSize, 0);
	Draw::Line(bottomLeft, CVector(bottomLeft.x + dimension*tileSize, bottomLeft.y));
	glPopMatrix();
	


	glPushMatrix();
	Draw::Line(bottomLeft, CVector(bottomLeft.x, bottomLeft.y + dimension*tileSize));
	glTranslatef(dimension*tileSize, 0, 0);
	Draw::Line(bottomLeft, CVector(bottomLeft.x, bottomLeft.y + dimension*tileSize));
	glPopMatrix();

	glLineWidth(1);

	
// 	for(int i = 1; i < dimension; i++)
// 	{
// 		Draw::Line(CVector(bottomLeft.x + i*tileSize, bottomLeft.y), CVector(bottomLeft.x + i*tileSize, bottomLeft.y + dimension*tileSize));
//  		Draw::Line(CVector(bottomLeft.x, bottomLeft.y + i*tileSize), CVector(bottomLeft.x + dimension*tileSize, bottomLeft.y + i*tileSize));
//  	}

// 	glColor4f(1,0,0,0.7);
// 	for(int i=0; i<dimension; i++)
// 		for(int j=0; j<dimension; j++)
// 		{
// 			if(cubeMatrix[i][j]!=NULL)
// 				Draw::Rect(bottomLeft + CVector(i*tileSize, tileSize*(j+1)), tileSize, tileSize);
// 		}
}

void CLevelMatrix::ClearDynamicTetriminos()
{
	for(int i=0; i<dimension; i++)
		for(int j=0; j<dimension; j++)
		{

			if(cubeMatrix[i][j] == NULL)
				continue;

			if(cubeMatrix[i][j]->dynamic)
				cubeMatrix[i][j] = NULL;
		}
}


void CLevelMatrix::AddDynamicTetrimino( CTetrimino* _dynamicTetrimino )
{
	vector<CTCube>::iterator it = _dynamicTetrimino->tCubes->begin();
	for(; it != _dynamicTetrimino->tCubes->end(); it++)
	{
		int globalI = it->i + _dynamicTetrimino->i;
		int globalJ = it->j + _dynamicTetrimino->j;

		if(!F::MISC::ValidIndices(globalI, globalJ, dimension, dimension))
			continue;

		cubeMatrix[globalI][globalJ] = _dynamicTetrimino;
	}
}

void CLevelMatrix::ClearTetrimino( CTetrimino* _tetrimino )
{
	vector<CTCube>::iterator it = _tetrimino->tCubes->begin();
	for(; it != _tetrimino->tCubes->end(); it++)
	{
		int globalI = it->i + _tetrimino->i;
		int globalJ = it->j + _tetrimino->j;

		if(!F::MISC::ValidIndices(globalI, globalJ, dimension, dimension))
			continue;

		if(cubeMatrix[globalI][globalJ] == _tetrimino)
		cubeMatrix[globalI][globalJ] = NULL;
	}
}

bool CLevelMatrix::ValidTCubesPlacement( const vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(!ValidTCubePlacement(*it))
			return false;
	}

	return true;
}

CTetrimino* CLevelMatrix::GetTetriminoAtTCube( const CTCube &_tc )
{
	return cubeMatrix[_tc.i][_tc.j];
}

bool CLevelMatrix::ValidTCubePlacement( const CTCube &_tc )
{
	if(TCubeInBounds(_tc))
		return GetTetriminoAtTCube(_tc) == NULL;
	
	return false;
}

bool CLevelMatrix::TCubeInBounds( const CTCube &_tc )
{
	return F::MISC::ValidIndicesInSquareMatrix(_tc.i, _tc.j, dimension);
}

bool CLevelMatrix::TCubesIntersectStaticTetriminos( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(TCubeInBounds(*it))
		{
			if(GetTetriminoAtTCube(*it)!=NULL)
			if(!GetTetriminoAtTCube(*it)->dynamic)
				return true;
		}
	}

	return false;
}

void CLevelMatrix::RemoveAllTCubesUpwardOfMatrix( std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::iterator it = _tCubes.begin();
	for(; it != _tCubes.end();)
	{
		if(it->j>dimension-1)
			it = _tCubes.erase(it);
		else
			it++;
	}
}

bool CLevelMatrix::TCubesSidewaysOutOfBounds( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(!TCubeSidewaysInBounds(*it))
			return true;
	}

	return false;
}

bool CLevelMatrix::TCubeSidewaysInBounds( const CTCube &_tc )
{
	return ColumnInBounds(_tc.i);
}

bool CLevelMatrix::RowInBoundsFromBelow( int _j )
{
	return _j>=0;
}

bool CLevelMatrix::ColumnInBounds( int _i )
{
	return F::MISC::ValidIndexInSquareMatrix(_i, dimension);
}

bool CLevelMatrix::TCubesInMatrixCollisionWithStaticTetriminos( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	CTetrimino* temp;

	for(; it != _tCubes.end(); it++)
	{
		 temp = GetTetriminoAtTCube(*it);

		 if(temp != NULL)
		 {
			 if(!temp->dynamic)
				 return true;
		 }

	}

	return false;
}

bool CLevelMatrix::BlockedFromBellowByStaticTCubes( CTetrimino* _tetrimino )
{
	vector<CTCube> apsoluteBottomFeelers = *_tetrimino->DTD->bottomCollisionFeelers;
	_tetrimino->ChangeToApsoluteCoords(apsoluteBottomFeelers);
	return TCubesIntersectStaticTetriminos(apsoluteBottomFeelers);
}

bool CLevelMatrix::BlockedFromBellowByNonDynamicTCubes( CTetrimino* _tetrimino )
{
	if(_tetrimino->HitBottom())
		return true;

	if(BlockedFromBellowByStaticTCubes(_tetrimino))
		return true;

	return false;
}

void CLevelMatrix::UpdateTetrimino( CTetrimino* _terimino )
{
	ClearTetrimino(_terimino);
	_terimino->j--;
	AddDynamicTetrimino(_terimino);
}

void CLevelMatrix::Clear()
{
	for(int i=0; i<dimension; i++)
		for(int j=0; j<dimension; j++)
			cubeMatrix[i][j] = NULL;
}

bool CLevelMatrix::TCubesIntersectTetriminos( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(TCubeInBounds(*it))
		{
			if(GetTetriminoAtTCube(*it)!=NULL)
					return true;
		}
	}

	return false;
}

bool CLevelMatrix::TCubesIntersectDynamicTetriminos( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(TCubeInBounds(*it))
		{
			if(GetTetriminoAtTCube(*it)!=NULL)
				if(GetTetriminoAtTCube(*it)->dynamic)
					return true;
		}
	}

	return false;
}

void CLevelMatrix::GetTetriminosIntersectedByTCubes( vector<CTetrimino*> &_tetriminos, const vector<CTCube> &_tCubes )
{

	CTetrimino* temp;
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
	{
		if(TCubeInBounds(*it))
		{
			temp = GetTetriminoAtTCube(*it);
			if(temp!=NULL)
				_tetriminos.push_back(temp);
			
		}
	}
}

#include <iostream>

using namespace std;

void CLevelMatrix::PrintToConsole()
{
	for(int i=0; i<dimension; i++)
	{
		for(int j=0; j<dimension; j++)
		{
			if(cubeMatrix[j][dimension -i-1]!=NULL)
				cout << int(cubeMatrix[j][dimension -i-1]->dynamic) + 1 << " ";
			else
				cout << bool(cubeMatrix[j][dimension -i-1]!=NULL) << " ";
		}

		cout << endl;
	}
}

bool CLevelMatrix::TCubesInMatrixCollisionWithTetriminos( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	CTetrimino* temp;

	for(; it != _tCubes.end(); it++)
	{
		temp = GetTetriminoAtTCube(*it);

		if(temp != NULL)
			return true;
	}

	return false;
}

void CLevelMatrix::HandleFullRows()
{
	for(int i=0; i < dimension; i++)
		if(potentialFullRows[i]) if(FullStaticRow(i))
			fullStaticRows.insert(i);

	memset(potentialFullRows, 0, dimension*sizeof(bool));
}

void CLevelMatrix::AddTetriminoRowsToPotentialFullRows( CTetrimino *_tetrimino )
{
	for(int i=_tetrimino->GetLowestApsoluteJCoord(); i <= _tetrimino->GetHighestApsoluteJCoord(); i++)
	{
		if(F::MISC::ValidIndexInSquareMatrix(i, dimension))
		potentialFullRows[i] = true;
		else
		{
			tetriminoOutOfMatrixFreeze = true;
			return;
		}
	}
	
}

bool CLevelMatrix::FullStaticRow( int _row )
{
	for (int i = 0; i < dimension ; i++)
	{
		if(cubeMatrix[i][_row]==NULL)
			return false;

		if(cubeMatrix[i][_row]->dynamic)
			return false;
	}

	return true;
}

bool CLevelMatrix::HighestRowOccupied()
{
	return potentialFullRows[dimension-1];
}
