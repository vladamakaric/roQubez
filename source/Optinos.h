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
#ifndef OPTIONS_H
#define OPTIONS_H

#include "State.h"
#include "Image.h"
#include "Button.h"
#include "Slider.h"


class COptions : public CState
{	
public:
	static COptions* GetInstance();
	void Draw();
	void Logic();
	void RegisterEvents();
	void Initialize();
	~COptions(){};
private: 
	COptions();


	CSlider matrixSizeSl;
	CImage *matrixSizeSlTxt;
	CImage levelMatrixSizeTxt;
	CImage controlsTxt;
	CImage instructionLine1;
	CImage instructionLine2;
	CImage instructionLine3;
	CImage instructionLine4;
	CRotatingTextButton mainMenu;
};



#endif