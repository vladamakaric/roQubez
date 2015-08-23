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

#include "Image.h"
#include "f_graphics.h"

CImage::CImage( const std::string& _text, const TTF_Font *_font, UColor _txtColor )
{
	textureID = F::GRAPHICS::GenerateGLTextureFromTTFText(_font, _txtColor, _text, width, height, powWidth, powHeight);
}

CImage::CImage( const std::string& _imageFile )
{
	SDL_Surface* surface = SDL_LoadBMP(_imageFile.c_str());
//	textureID = F::GRAPHICS::GetGLTextureIDFromSDL_Surface(surface);
	width = surface->w;
	height = surface->h;
	textureID = F::GRAPHICS::GGetGLTextureIDFromSDL_Surface(surface, powWidth, powHeight);
}

CImage::~CImage()
{
	glDeleteTextures( 1, &textureID );
}

void CImage::Draw( const CVector &_topLeft )
{
	F::GRAPHICS::DrawTexture(_topLeft, textureID, powWidth, powHeight);
}

void CImage::Draw( const CVector &_topLeft, int _width, int _height )
{
	F::GRAPHICS::DrawTexture(_topLeft, textureID, _width, _height);
}

void CImage::DrawInRectCenter( const CSquare &_rect )
{
	F::GRAPHICS::DrawTexture(CVector(_rect.downLeft.x + _rect.width/2 - width/2, _rect.upRight.y - _rect.height/2 + height/2), textureID, powWidth, powHeight);
}

