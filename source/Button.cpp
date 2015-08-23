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

#include "Button.h"
#include "f_intersection.h"
#include "draw.h"
#include "Game.h"
#include "f_graphics.h"

CRotatingTextButton::CRotatingTextButton( const CVector _position, const std::string & _text, TTF_Font* _font,double _width, double _height ) : 
	mRect(_position, _position + CVector(_width, _height), true), 
	mMouseOver(false), 
	mMouseClick(false),
	btnText(new CImage(_text, _font, 0x00ffffff))
{
	rotate = false;
	beginRotate = false;
	rotateDir = 1;
	alpha = 0;
	angle = 0;
	angularAcc = 900;
	maxAnglularVel =900;
	minAngularVel = 30;
	anglularVel = maxAnglularVel;
}

void CRotatingTextButton::Draw()
{

	if(mMouseOver)
		alpha+=CGame::GetInstance()->deltaTime*10;
	else
		alpha-=CGame::GetInstance()->deltaTime*3;
	

	F::MISC::ConstrainValueToInterval(alpha, 0.0,1.0);




	glPushMatrix();


	if(mMouseOver)
	{
		if(!beginRotate)
		{
			rotateDir*=-1;
			if(rotateDir==-1)
				angle = 360;
			
			anglularVel = maxAnglularVel;

			beginRotate = true;
			rotate = true;
		}
	}
	else
		beginRotate = false;


		glTranslatef(mRect.downLeft.x + mRect.width/2, mRect.downLeft.y + mRect.height/2, 0);
		glRotatef(angle,0.2,2,0.66);

		glTranslatef(-mRect.downLeft.x - mRect.width/2, -mRect.downLeft.y - mRect.height/2, 0);


		if(rotate)
		{
			anglularVel -= angularAcc*CGame::GetInstance()->deltaTime;
			angle+=rotateDir*anglularVel*CGame::GetInstance()->deltaTime;


			if(anglularVel<minAngularVel)
				anglularVel = minAngularVel;

			if(angle<0 || angle>360)
			{
				angle = 0;
				rotate = false;
			}
		}



	

	F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color2);
	Draw::Rect(mRect);

	if(alpha > 0)
	{
		glColor4ub(CGame::GetInstance()->color1.bytes[0],CGame::GetInstance()->color1.bytes[1], CGame::GetInstance()->color1.bytes[2], alpha*255);
		Draw::Rect(mRect);
	}

	glColor3f(1,1,1);
	btnText->DrawInRectCenter(mRect);

	glPopMatrix();
}


void CRotatingTextButton::MouseMotionEventHandling()
{
	CVector mousePoint(event.motion.x, CGame::GetInstance()->screenHeight - event.motion.y);
	
	mMouseOver = F::INTERSECTION::PointInSquare(mousePoint, mRect);

	if(mMouseClick && !mMouseOver)
		mMouseClick = false;
}

void CRotatingTextButton::MouseClickEventHandling()
{
	CVector mousePoint(event.motion.x, CGame::GetInstance()->screenHeight - event.motion.y);
	mMouseClick = F::INTERSECTION::PointInSquare(mousePoint, mRect);
}

void CRotatingTextButton::ResetButton()
{
	mMouseOver = false; 
	mMouseClick = false; 
	angle = 0;

	rotate = false;
	beginRotate = false;
	rotateDir = 1;
	alpha = 0;
	angle = 0;
	angularAcc = 900;
	maxAnglularVel =900;
	minAngularVel = 30;
	anglularVel = maxAnglularVel;
}



