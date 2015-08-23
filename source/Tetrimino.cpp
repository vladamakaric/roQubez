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

#include "f_color.h"
#include "TetriminoManager.h"
#include "Tetrimino.h"
#include "draw.h"
#include <iostream>
#include "f_misc.h"
#include <stdlib.h>
using namespace std;


CTetrimino::CTetrimino(CLevelMatrix *_lm, CTetriminoType *_tt)
{
	CommonInit(_lm,_tt,true);

	face = rand()%4;

	tCubes = &type->tCubes[face]; 

	DTD = new CDynamicTetriminoData(this);
	FTD = new CFocusTetriminoData(this);

	int lowestJ = DTD->lowestLocalJ;
	i = LM->instantiationCube.i;
	j = LM->instantiationCube.j - lowestJ + 2;
	AlignPositionWithMatrix();
}

CTetrimino::CTetrimino(CLevelMatrix *_lm, int _i, int _j, CTetriminoType* _tt, bool _dynamic) 
{
	CommonInit(_lm,_tt,_dynamic);
	face = 0;

	tCubes = &type->tCubes[face];

	i = _i;
	j = _j;

	DTD = new CDynamicTetriminoData(this);
	FTD = NULL;
	position = LM->bottomLeft + CVector(i*LM->tileSize, (j+1)*LM->tileSize);

	


}



void CTetrimino::CommonInit(CLevelMatrix *_lm, CTetriminoType *_tt, bool _dynamic)
{

	color = UColor(F::COLOR::GetRandomSaturizedColor(1, 0.7));
// 	color.bytes[0] = rand()%256;
// 	color.bytes[1] = rand()%256;
// 	color.bytes[2] = rand()%256;

	memset(tCubeDeletionMatrix, 0, T_MATRIX_SIZE*T_MATRIX_SIZE*sizeof(bool));
	LM = _lm;
	type = _tt; 
	focus = false;
	dynamic = _dynamic;
	sliced = false;
	pendingThaw = false;
}



void CTetrimino::Draw()
{
	glColor3ub(color.bytes[0], color.bytes[1], color.bytes[2]);
	DrawTCubes(*tCubes);
}

void CTetrimino::UpdatePosition(const CVector &_offset)
{
	position += _offset;
}

void CTetrimino::DrawTCube( const CTCube &_tc )
{
	Draw::Rect(CVector(position.x + _tc.i*LM->tileSize, position.y + _tc.j*LM->tileSize), LM->tileSize, LM->tileSize);
}

void CTetrimino::DebugDraw()
{
// 	glColor3f(0,1,0);
// 	DrawTCubes(*FTD->rightCollisionFeelers);

	glColor4f(1,0,0,0.3);
	DrawTCubes(*DTD->bottomCollisionFeelers);
	glColor4f(1,0,1,0.3);
	DrawTCubes(*DTD->upperCollisionFeelers);
}

void CTetrimino::DrawTCubes( const std::vector<CTCube> &_tCubes )
{
	vector<CTCube>::const_iterator it = _tCubes.begin();
	for(; it != _tCubes.end(); it++)
		DrawTCube(*it);
}

void CTetrimino::Rotate( int dir )
{
	face = GetRotatedFace(dir);

	tCubes = &type->tCubes[face];




	if(DTD)
	{
		DTD->Update(this);
		if(FTD) FTD->Update(this);
	}
}

void CTetrimino::Move( int dir )
{
	i+=dir;
	position.x += dir*LM->tileSize;
}

void CTetrimino::GetRotatedTetrimino( int _dir, vector<CTCube> &_rotatedT )
{
	int tempFace = GetRotatedFace(_dir);
	_rotatedT = type->tCubes[tempFace];



// 	_rotatedT.reserve( type->tCubes[tempFace].size() +  type->bottomCollisionFeelers[tempFace].size());
// 	_rotatedT.insert( _rotatedT.end(), type->tCubes[tempFace].begin(), type->tCubes[tempFace].end() );
// 	_rotatedT.insert( _rotatedT.end(), type->bottomCollisionFeelers[tempFace].begin(), type->bottomCollisionFeelers[tempFace].end() );
}

void CTetrimino::ChangeToApsoluteCoords( vector<CTCube> &_tCubes )
{
	
	for(vector<CTCube>::iterator it = _tCubes.begin(); it != _tCubes.end(); it++)
	{
		it->i += i;
		it->j += j;
	}
}

void CTetrimino::ChangeToRelativeCoords( vector<CTCube> &_tCubes )
{
	for(vector<CTCube>::iterator it = _tCubes.begin(); it != _tCubes.end(); it++)
	{
		it->i -= i;
		it->j -= j;
	}
}

int CTetrimino::GetHighestApsoluteJCoord()
{
	return GetApsoluteJCoord(DTD->highestLocalJ);
}

int CTetrimino::GetLowestApsoluteJCoord()
{
	return GetApsoluteJCoord(DTD->lowestLocalJ);
}

void CTetrimino::Freeze()
{
	dynamic = false;

	if(FTD!=NULL)
	{
		delete FTD;
		FTD = NULL;
	}

	position.y = LM->bottomLeft.y + (j+1)*LM->tileSize;

	LM->AddTetriminoRowsToPotentialFullRows(this);
}

void CTetrimino::GetRotatedBounds( int _dir, int &_leftI, int &_rightI, int &_downJ )
{
	int tempFace = GetRotatedFace(_dir);

	_leftI = GetApsoluteICoord(type->FocusData->leftMostLocalI[tempFace]); 
	_rightI = GetApsoluteICoord(type->FocusData->rightMostLocalI[tempFace]); 
	_downJ = GetApsoluteJCoord(type->downMostLocalJ[tempFace]); 
}

int CTetrimino::GetRotatedFace( int _dir )
{
	int tempFace = face;
	tempFace+=_dir;
	tempFace = F::MISC::ModuloOperator(tempFace, 4);


	return tempFace;
}

bool CTetrimino::HitBottom()
{
	return GetLowestApsoluteJCoord() == 0;
}

bool CTetrimino::OnEdge( int _edgeDir )
{
	int relativeCoord = FTD->leftMostLocalI;

	if(_edgeDir==RIGHT_DIR)
		relativeCoord = FTD->RightMostLocalI;

	return !F::MISC::ValidIndexInSquareMatrix(GetApsoluteICoord(relativeCoord) + _edgeDir, LM->dimension);
}

CTCube CTetrimino::GetApsoluteTCube( const CTCube &_relativeTCube )
{
	return CTCube(GetApsoluteICoord(_relativeTCube.i), GetApsoluteJCoord(_relativeTCube.j));
}

void CTetrimino::Thaw()
{
	dynamic = true;
}



void CTetrimino::MarkApsoluteTCubeForDeletion( CTCube _tCube )
{
	CTCube localRelative = GetRelativeTCube(_tCube);
	CTetriminoManager::MakeRelativeCoordsApsoluteToSquareMatrix(localRelative.i, localRelative.j, T_MATRIX_SIZE);
	tCubeDeletionMatrix[localRelative.i][localRelative.j] = 1;
}

CTCube CTetrimino::GetRelativeTCube( const CTCube &_apsoluteTCube )
{
	return CTCube(_apsoluteTCube.i - i, _apsoluteTCube.j - j);
}

void CTetrimino::GetRemainingTCubes( vector<CTCube> &_tCubes )
{
	for(vector<CTCube>::iterator it = tCubes->begin(); it != tCubes->end(); it++)
	{
		if(!CubeMarkedForDeletion(*it))
			_tCubes.push_back(*it);
	}
}

void CTetrimino::AlignPositionWithMatrix()
{
	position = LM->bottomLeft + CVector(i*LM->tileSize, (j+1)*LM->tileSize);
}

bool CTetrimino::CubeMarkedForDeletion( CTCube _tCube )
{
	CTetriminoManager::MakeRelativeCoordsApsoluteToSquareMatrix(_tCube.i, _tCube.j,T_MATRIX_SIZE);
	return tCubeDeletionMatrix[_tCube.i][_tCube.j];
}

void CFocusTetriminoData::UpdateSpeed( double _deltaT, bool _speedUp, int _minPixelsPerSecund)
{
	if(_speedUp)
		pixelsPerSecund += PPSIncrementPerSecund*_deltaT;
	else
		pixelsPerSecund -= PPSdampingPerSecond*pixelsPerSecund*_deltaT;


	if(F::MISC::DoublesEqual(pixelsPerSecund, _minPixelsPerSecund, 1))
		pixelsPerSecund =_minPixelsPerSecund;

	F::MISC::ConstrainValueToInterval(pixelsPerSecund, double(_minPixelsPerSecund), maxPixelsPerSecond);
}
