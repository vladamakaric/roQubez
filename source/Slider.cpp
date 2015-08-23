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

#include "Slider.h"
#include "f_graphics.h"
#include "draw.h"
#include "Game.h"
#include "f_intersection.h"
#include "f_misc.h"


CSlider::CSlider(const CVector &_bottomLeft, double _width, double _height, double _sliderW, double _sliderOutset ,double _startValue, double _endValue, int _numberOfValsInBetween, double _currentVal ) :
	mouseDown(false),
	mouseOverLeft(false),
	mouseOverRight(false), 
	mouseOverSlider(false),
	width(_width),
	height(_height),
	startValue(_startValue),
	endValue(_endValue),
	currentValue(_currentVal),
	left(_bottomLeft, _bottomLeft + CVector(height,height)), 
	right( _bottomLeft + CVector( width - height,0), _bottomLeft + CVector( width,height) ),
	middle(left.upRight - CVector(0, height), right.downLeft + CVector(0, height) )
{
	stepSize =	(endValue - startValue)/(_numberOfValsInBetween+1);

	slider.Set(CVector(0,0), CVector(_sliderW, height + 2*_sliderOutset));
	slider.Move(left.downLeft + CVector(left.width, -_sliderOutset));

	UpdateSliderPosition();
}

void CSlider::Draw()
{
	UColor color4, color3;

	UColor color5 = 0x33333333; 

	color3.bytes[0] = 0;
	color3.bytes[1] = 0;
	color3.bytes[2]	= 200;
	

	color4.bytes[0] = 200;
	color4.bytes[1] = 0;
	color4.bytes[2]	= 0;

	if(mouseOverLeft)
	F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color1);
	else
		F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color2);

	Draw::Rect(left);

	if(mouseOverRight)
		F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color1);
	else
		F::GRAPHICS::ChangeGLColor(CGame::GetInstance()->color2);

	Draw::Rect(right);


	F::GRAPHICS::ChangeGLColor(color5);
	Draw::Rect(middle);
	
	if(mouseOverSlider)
		F::GRAPHICS::ChangeGLColor(color4);
	else
		F::GRAPHICS::ChangeGLColor(color3);

	Draw::Rect(slider);
}

void CSlider::MouseMotionEventHandling()
{
	CVector mousePoint(event.motion.x, CGame::GetInstance()->screenHeight - event.motion.y);
	if(mouseOverSlider && mouseDown)
	{
		if(mouseOverSlider)
		{
			CVector diff = mousePoint - prevMousePos; //od mousePoint do preV..
			diff.y = 0;
			slider.Move(diff);
			double x = slider.downLeft.x;
			F::MISC::ConstrainValueToInterval(x, left.upRight.x, right.downLeft.x - slider.width);
			slider.SetBottomLeftAndUpdate(CVector(x,slider.downLeft.y));

			double currentProgress = (slider.downLeft.x - left.upRight.x)/(middle.width - slider.width);

			double currentRealValue = currentProgress * (endValue - startValue);
			int relativeValNum = currentRealValue/stepSize;
			currentValue = startValue + relativeValNum*stepSize;
		}
	}

	
	prevMousePos = mousePoint;
	mouseOverLeft = F::INTERSECTION::PointInSquare(mousePoint, left);
	mouseOverRight = F::INTERSECTION::PointInSquare(mousePoint, right);
	mouseOverSlider = F::INTERSECTION::PointInSquare(mousePoint, slider);
}

void CSlider::MouseDownEventHandling()
{
	mouseDown = true;

	if(mouseOverLeft)
		currentValue-=stepSize;

	if(mouseOverRight)
		currentValue+=stepSize;

	F::MISC::ConstrainValueToInterval(currentValue, startValue, endValue);

	if(mouseOverRight || mouseOverLeft)
		UpdateSliderPosition();
}


void CSlider::Reset()
{
	mouseOverLeft = mouseOverRight = mouseDown = mouseOverSlider = false;
}

void CSlider::UpdateSliderPosition()
{
	double progress = 1 - (endValue - currentValue)/(endValue - startValue);
	double coordinateProgress = progress*(middle.width - slider.width) + left.downLeft.x + left.width;
	slider.SetBottomLeftAndUpdate(CVector(coordinateProgress, slider.downLeft.y));
}

void CSlider::MouseUpEventHandling()
{
	mouseDown = false;
}


