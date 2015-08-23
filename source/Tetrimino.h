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

#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "TCube.h"
#include "Vector.h"
#include "LevelMatrix.h"
#include <list>
#include "TetriminoType.h"
#include "Color.h"

#include "TetriminoManager.h"

class CFocusTetriminoData; 
class CDynamicTetriminoData;

class CTetrimino
{
public:
	enum {LEFT_DIR = -1, RIGHT_DIR = 1};
	CTetrimino(CLevelMatrix *_lm, int i, int j, const std::vector<CTCube> &_tCubesRelative){};
	CTetrimino(CLevelMatrix *_lm, int i, int j, CTetriminoType* _tt, bool _dynamic);
	CTetrimino(CLevelMatrix *_lm, CTetriminoType* _tt);
	void CommonInit(CLevelMatrix *_lm, CTetriminoType *_tt, bool _dynamic);
	void UpdatePosition(const CVector &_offset);
	void Rotate( int dir );
	void Move(int dir);
	void Draw();
	void DrawTCube(const CTCube &_tc);
	void DebugDraw();
	void DrawTCubes(const std::vector<CTCube> &_tCubes);
	int GetApsoluteJCoord(int relativeJ) { return j+relativeJ; }
	int GetApsoluteICoord(int relativeI) { return i+relativeI; }
	void GetRotatedTetrimino(int dir, vector<CTCube> &_rotatedT);
	CTCube GetApsoluteTCube(const CTCube &_relativeTCube);
	void GetRotatedBounds(int dir, int &_leftI, int  &_rightI, int &_downJ);

	int GetRotatedFace(int dir);
	bool HitBottom();
	bool OnEdge(int _edgeDir);

	void AlignPositionWithMatrix();


	void ChangeToApsoluteCoords(vector<CTCube> &_tCubes);
	void ChangeToRelativeCoords(vector<CTCube> &_tCubes);
	void Freeze();
	void Thaw();

	int GetLowestApsoluteJCoord();
	int GetHighestApsoluteJCoord();
	int i,j;
	int face;
	bool focus;
	bool dynamic;

	UColor color;
	CTCube GetRelativeTCube(const CTCube &_apsoluteTCube);
	void MarkApsoluteTCubeForDeletion(CTCube _tCube);
	bool CubeMarkedForDeletion(CTCube _tCube);


	void GetRemainingTCubes(vector<CTCube> &_tCubes);

	bool sliced;
	bool pendingThaw;
	CVector position;
	CDynamicTetriminoData* DTD;
	CFocusTetriminoData* FTD;
	CTetriminoType* type; 
	CLevelMatrix *LM;
	std::vector<CTCube> *tCubes;

	bool tCubeDeletionMatrix[T_MATRIX_SIZE][T_MATRIX_SIZE];
};

class CFocusTetriminoData
{
public:
	CFocusTetriminoData(CTetrimino * _tetrimino) : 
		pixelsPerSecund(106), 
		maxPixelsPerSecond(2000), 
		PPSIncrementPerSecund(3500),
		PPSdampingPerSecond(30),
		framesActive(0)
		{Update(_tetrimino);}

	void Update(CTetrimino* _tetrimino)
	{
		leftCollisionFeelers = &_tetrimino->type->FocusData->leftCollisionFeelers[_tetrimino->face];
		rightCollisionFeelers = &_tetrimino->type->FocusData->rightCollisionFeelers[_tetrimino->face];
		leftMostLocalI = _tetrimino->type->FocusData->leftMostLocalI[_tetrimino->face];
		RightMostLocalI = _tetrimino->type->FocusData->rightMostLocalI[_tetrimino->face];

		leftBottomCollisionFeelers = &_tetrimino->type->FocusData->leftBottomCollisionFeelers[_tetrimino->face];
		rightBottomCollisionFeelers = &_tetrimino->type->FocusData->rightBottomCollisionFeelers[_tetrimino->face];

		///////
		UpperLeftCollisionFeelers = &_tetrimino->type->FocusData->UpperLeftCollisionFeelers[_tetrimino->face];
		UpperRightCollisionFeelers = &_tetrimino->type->FocusData->UpperRightCollisionFeelers[_tetrimino->face];
		BottomLeftCollisionFeelers = &_tetrimino->type->FocusData->BottomLeftCollisionFeelers[_tetrimino->face];
		BottomRightCollisionFeelers = &_tetrimino->type->FocusData->BottomRightCollisionFeelers[_tetrimino->face];
	}

	void UpdateSpeed(double _deltaT, bool _speedUp, int _minPixelsPerSecund);

	int framesActive;
	double pixelsPerSecund;
	double maxPixelsPerSecond;
	double PPSIncrementPerSecund;
	double PPSdampingPerSecond;

	int leftMostLocalI;
	int RightMostLocalI;


	std::vector<CTCube> *BottomLeftCollisionFeelers;
	std::vector<CTCube> *BottomRightCollisionFeelers;
	std::vector<CTCube> *UpperRightCollisionFeelers;
	std::vector<CTCube> *UpperLeftCollisionFeelers;
	///////
	std::vector<CTCube> *leftBottomCollisionFeelers;
	std::vector<CTCube> *rightBottomCollisionFeelers;
	////////
	std::vector<CTCube> *leftCollisionFeelers;
	std::vector<CTCube> *rightCollisionFeelers;
};

class CDynamicTetriminoData
{
public:
	CDynamicTetriminoData( CTetrimino* _tetrimino ) {Update(_tetrimino);}

	void Update(CTetrimino* _tetrimino)
	{
		bottomCollisionFeelers = &_tetrimino->type->bottomCollisionFeelers[_tetrimino->face];
		upperCollisionFeelers = &_tetrimino->type->uppperCollisionFeelers[_tetrimino->face];
		lowestLocalJ = _tetrimino->type->downMostLocalJ[_tetrimino->face];
		highestLocalJ = _tetrimino->type->upperMostLocalJ[_tetrimino->face];
	}

	int lowestLocalJ;
	int highestLocalJ;

	std::vector<CTCube> *bottomCollisionFeelers;
	std::vector<CTCube> *upperCollisionFeelers;
};


#endif
