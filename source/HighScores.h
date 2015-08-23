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

#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include "State.h"
#include "Button.h"
#include "Image.h"

#include <set>
#include <string>

class CHSEntry
{
public:
	CHSEntry(const std::string &_name, int _points, int _level, int _matrixSize);
	std::string name;
	int points;
	int level;
	int matrixSize;
	CImage *text;

	void GenerateTextImage(int number);
	void Draw(const CVector &_lowerLeft);
};

struct CHEntryCompare {
	bool operator() (const CHSEntry* lhs, const CHSEntry* rhs) const{

		if(lhs->points != rhs->points)
		return lhs->points > rhs->points;

		return lhs->name<rhs->name;
	}
};

class CHighScores : public CState
{	
public:
	static CHighScores* GetInstance();
	void Draw();
	void DrawEntries(CVector topLeft, double spaceBetween);
	void Logic();
	void RegisterEvents();
	void Initialize();
	bool Placable(int _score);
	void AddNewEntry(CHSEntry* entry);
	void LoadFromFile();
	void GenerateImages();
	void SaveToFile();
	void CalculateAverageW();


	~CHighScores(){};
private: 
	int lowestScore;
	int averageEntryWidth;
	CHighScores();
	int maxEntries;
	std::set<CHSEntry*,CHEntryCompare> entries;
	CImage highScoresBigTxt;
	CRotatingTextButton mainMenu;
};


#endif