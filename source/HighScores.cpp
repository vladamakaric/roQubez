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

#include "HighScores.h"
#include "Game.h"
#include "MainMenu.h"
#include "Square.h"

#include <sstream>

#define BTN_W 300

CHighScores::CHighScores() : 
	mainMenu(CVector((CGame::GetInstance()->screenWidth - BTN_W)/2, 50), 
	"menu", CGame::GetInstance()->mainMenuBtnFont, BTN_W, 50),
	highScoresBigTxt("HIGH SCORES", CGame::GetInstance()->mainMenuBtnFont, 0xffffffff),
	maxEntries(7)
{
	LoadFromFile();
}

CHighScores* CHighScores::GetInstance()
{
	static CHighScores* instance = NULL;

	if(instance == NULL)
		instance = new CHighScores();

	return instance;
}

void CHighScores::Draw()
{
	CSquare screenRect = CGame::GetInstance()->screenRect;
	screenRect.downLeft.y += CGame::GetInstance()->screenHeight*0.83;
	screenRect.CalculateWH();
	highScoresBigTxt.DrawInRectCenter(screenRect);

	mainMenu.Draw();

	DrawEntries(CVector((CGame::GetInstance()->screenWidth - averageEntryWidth)/2, CGame::GetInstance()->screenHeight - highScoresBigTxt.height - 105), 20);
}

void CHighScores::RegisterEvents()
{
	if( event.type == SDL_MOUSEMOTION )
	{
		mainMenu.MouseMotionEventHandling();
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		mainMenu.MouseClickEventHandling();
	}

	if(event.type == SDL_QUIT)
		CGame::GetInstance()->Quit();
}

void CHighScores::Initialize()
{
	mainMenu.ResetButton();
}

void CHighScores::Logic()
{
	if(mainMenu.mMouseClick)
		CGame::GetInstance()->nextState = CMainMenu::GetInstance();
}

void CHighScores::LoadFromFile()
{
	FILE *fp = fopen("data/highScores.txt", "r");

	if(fp==NULL)
		return;

	char *name = new char[CGame::GetInstance()->maxNameSize + 1];
	int points;
	int level;
	int matrixSize;

	while(fscanf(fp, "%8[^?]?%d?%d?%d\n", name, &points, &level, &matrixSize)!=EOF)
	{
		CHSEntry *newEntry = new CHSEntry(name, points, level, matrixSize);
		entries.insert(newEntry);
	}

	delete []name;

	fclose(fp);
	GenerateImages();
	CalculateAverageW();
}

void CHighScores::DrawEntries( CVector topLeft, double spaceBetween )
{
	double y = topLeft.y;
	double x = topLeft.x;

	for(set<CHSEntry*>::iterator entry = entries.begin(); entry!= entries.end(); ++entry)
	{
		(*entry)->Draw(CVector(x, y - (*entry)->text->height));
		y -= (*entry)->text->height + spaceBetween;
	}
}

void CHighScores::AddNewEntry( CHSEntry* entry )
{
	entries.insert(entry);

	std::set<CHSEntry*>::iterator iter = entries.end();
	--iter;

	if(entries.size()>maxEntries)
		entries.erase(iter);



	GenerateImages();
	CalculateAverageW();
}

void CHighScores::GenerateImages()
{
	int i=0;


	for(set<CHSEntry*>::iterator entry = entries.begin(); entry!= entries.end(); ++entry)
	{
		i++;
		(*entry)->GenerateTextImage(i);
	}
}

bool CHighScores::Placable( int _score )
{
	if(!entries.empty())
	lowestScore = (*entries.rbegin())->points;

	return entries.size()<maxEntries || lowestScore<_score;
}

void CHighScores::SaveToFile()
{
	FILE *fp;

	fp =fopen("data/highScores.txt", "w");

	int i=0;
	int last = entries.size()-1;

	std::string format = "%s?%d?%d?%d\n";
	for(set<CHSEntry*>::iterator entry = entries.begin(); entry!= entries.end(); ++entry)
	{
		if(i==last) format.erase( format.length() - 1 );

		fprintf(fp, format.c_str(), (*entry)->name.c_str(), (*entry)->points, (*entry)->level, (*entry)->matrixSize);
	}

	fclose(fp);
}

void CHighScores::CalculateAverageW()
{
	averageEntryWidth = 0;

	for(set<CHSEntry*>::iterator entry = entries.begin(); entry!= entries.end(); ++entry)
		averageEntryWidth+=(*entry)->text->width;

	if(!entries.empty())
	averageEntryWidth/=entries.size();
}

CHSEntry::CHSEntry( const std::string &_name, int _points, int _level, int _matrixSize ) : 
	name(_name), 
	points(_points), 
	level(_level),
	text(NULL),
	matrixSize(_matrixSize)
{

}

void CHSEntry::GenerateTextImage( int number )
{
	if(text!=NULL)
		delete text;

	stringstream ss;
	ss << number;
	string textStr = ss.str() + ". " + name + ", score ";
	ss.str( std::string() );
	ss.clear();
	ss << points;
	textStr+= ss.str() + " lvl ";
	ss.str( std::string() );
	ss.clear();
	ss << level;
	textStr+= ss.str() +  " matrix size ";
	ss.str( std::string() );
	ss.clear();
	ss << matrixSize;
	textStr +=ss.str() + "x" + ss.str();
	
	text = new CImage( textStr, CGame::GetInstance()->creditsFont, 0xffffffff);
}

void CHSEntry::Draw( const CVector &_lowerLeft )
{
	text->Draw(_lowerLeft + CVector(0, text->height));
}
