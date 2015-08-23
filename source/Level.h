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

#ifndef LEVEL_H
#define LEVEL_H

#include "State.h"
#include "Button.h"
#include "LevelMatrix.h"
#include <list>
#include <vector>
#include "Tetrimino.h"
#include "TetriminoType.h"

class CLevel : public CState
{	
public:
	enum{ROTATING, WAITING_FOR_SNAP, NONE};


	bool MOVE_LEFT, MOVE_RIGHT, SPEED_UP, ROTATE_TETRIMINO;

	bool ROTATE_LEVEL;
	bool ROTATE_LEVELCCW;


	int nextTetrimino;
	static CLevel* GetInstance();
	void Draw();
	void Logic();
	void RegisterEvents();
	void Initialize();
	void UpdateDynamicTetriminoPositions(double _amount);
	void UpdateLevelMatrix();
	void UpdateLevelMatrixOnlyForFallingBlock();
	void HandleCollision();
	bool MoveTetriminoSideways(int dir);
	void RotateTetriminoCW();
	void DetectDynamicTetriminoDownwardFreeze();
	void RegulateKeyPressTime();
	void AddNewFallingBlock();
	void RecursiveFreeze(CTetrimino* _tetrimino);

	void RecursiveThaw(CTetrimino* _tetrimino);
	bool FTBottomCollisionWithDT();
	void DynamicTetriminosLogic();
	void FocusTetriminoLogic();
	void ClickFTWithDynamicTs();

	void ClearTetriminos();
	void AddNewDynamicBlock(int i, int j);

	void HandleSlicingTetriminos();

	void ThawStaticTetriminos();

	void FinishRotate();

	bool FocusTetriminoCutOff();

	void RotateTetriminoWithLevel(CTetrimino* _tetrimino, int dir);
	void Rotate(int dir);
	void DrawNextTetrimino();

	double gameOverAlpha;
	double previewTileSize;
	double inset;
	int rotateState;

	bool updateDynamicT;
	bool updateFocusT;

	double DEBUG_angle;

	bool THAW;
	~CLevel(){};


	int points;
	int level;

	CImage* pointsTxt;
	CImage* levelTxt;
	CImage nextTetriminoTxt;
	CVector levelTxtPos, pointsTxtPos;
	std::vector<CTetrimino*> slicedTetriminos;
	std::vector<CTetrimino*> pendingThawTetriminos;
	std::vector<CTetrimino*> tetriminos; 

	CTetrimino* focusTetrimino;


	CLevelMatrix LM;

	////////////////
	double currentRotAngle;
	double degreesPerSecund;
	int rotationDir;

	double currentLevelTime;
	double levelDuration;
	double pixelsPerSecundPerLevel;

	

	/////////////////////////
	double deltaTime;
	bool focusClicked;
	double sidewaysKeysTime;
	double sidewaysMovementPeriod;
	double maxSMovementPeriod;

	double FTUpdateAmount;
	double DTUpdateAmount;

	double rotateKeyTime;
	double rotatePeriod;

	int pixelsPerSecond;

	double tileProgress;
	bool matrixSnap;

	double focusTTileProgress;
	bool focusTMatrixSnap;

	bool gameOver;

	/////////////

	bool dbgDrop;
private: 
	CLevel(double _inset, int _matrixDimension);
	void CuttOffFocusTetrimino();
	CRotatingTextButton mainMenu;
	CRotatingTextButton cwBtn;
	CRotatingTextButton ccwBtn;

};

#endif