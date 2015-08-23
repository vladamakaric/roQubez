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
#include <math.h>
#include "f_graphics.h"

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
static const Uint32 rmask = 0x000000FF;
static const Uint32 bmask = 0x0000FF00;
static const Uint32 gmask = 0x00FF0000;
static const Uint32 amask = 0xFF000000;
#else
static const Uint32 rmask = 0xFF000000;
static const Uint32 bmask = 0x00FF0000;
static const Uint32 gmask = 0x0000FF00;
static const Uint32 amask = 0x000000FF;
#endif

GLuint F::GRAPHICS::GenerateGLTextureFromTTFText(const TTF_Font *_font, UColor color, const std::string& _text, 
												 int &_width, int &_height, int &_powWidth, int &_powHeight )
{
	SDL_Color Color = {color.bytes[0], color.bytes[1], color.bytes[2]};
	SDL_Surface *message = TTF_RenderText_Blended(const_cast<TTF_Font*>(_font), _text.c_str(), Color);

	_width = message->w;
	_height =message->h;

	GLuint texID = F::GRAPHICS::GGetGLTextureIDFromSDL_Surface(message, _powWidth, _powHeight);

	SDL_FreeSurface(message);

	return texID;
}

GLuint F::GRAPHICS::GGetGLTextureIDFromSDL_Surface(SDL_Surface *_surface, int &_PO2width, int &_PO2height)
{
	SDL_PixelFormat *format = _surface->format;
	Uint32 width = _surface->w;
	Uint32 height = _surface->h;
	Uint32 widthPow = (unsigned) pow( 2, ceil( log( width ) / log( 2 ) ) );
	Uint32 heightPow = (unsigned) pow( 2, ceil( log( height ) / log( 2 ) ) );


	_PO2width = widthPow;

	_PO2height = heightPow;

	SDL_Surface* newSurface = SDL_CreateRGBSurface( SDL_SRCALPHA,
												   widthPow, heightPow, 32,
												   rmask, bmask, gmask, amask );
	Uint32 alpha = 0;
	alpha = SDL_MapRGBA( format, 0, 0, 0, Uint8(amask) );
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = heightPow;
	rect.w = widthPow;
	int ret = SDL_FillRect( newSurface, &rect, alpha);
	_surface->flags &= !SDL_SRCALPHA;

	SDL_SetAlpha( newSurface, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT );

	
	SDL_Rect smallRct = {0,0, Uint16(width), Uint16(height)};

	ret = SDL_BlitSurface( _surface, &smallRct, newSurface, &smallRct );

	GLuint texture = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
 	glGenTextures(1, &texture );
 	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 	gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
 		widthPow, heightPow, GL_RGBA,GL_UNSIGNED_BYTE, newSurface->pixels);

	SDL_FreeSurface( newSurface );
	return texture;
}


GLuint F::GRAPHICS::GetGLTextureIDFromSDL_Surface(SDL_Surface *_surface)
{
	GLenum textureFormat;
	GLuint texture;

	switch (_surface->format->BytesPerPixel) 
	{
	case 4:
		if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA_EXT;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
			textureFormat = GL_BGR_EXT;
		else
			textureFormat = GL_RGB;
		break;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, _surface->format->BytesPerPixel, _surface->w,
		_surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, _surface->pixels);

	return texture;
}	

void F::GRAPHICS::DrawTexture(const CVector &_topLeft, GLuint _textureid,int _width, int _height) 
{
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureid);

	glBegin(GL_QUADS);
	// top left
	glTexCoord2i(0, 0);
	glVertex3f(_topLeft.x, _topLeft.y, 0);

	// top right
	glTexCoord2i(1, 0);
	glVertex3f(_topLeft.x+_width, _topLeft.y, 0);

	// bottom right
	glTexCoord2i(1, 1);
	glVertex3f(_topLeft.x+_width, _topLeft.y - _height, 0);

	// bottom left
	glTexCoord2i(0, 1);
	glVertex3f(_topLeft.x, _topLeft.y - _height, 0);

	glEnd();

	glDisable(GL_TEXTURE_2D );
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}

void F::GRAPHICS::ChangeGLColor( UColor color )
{
	glColor3ub(color.bytes[0],color.bytes[1], color.bytes[2]);
}
