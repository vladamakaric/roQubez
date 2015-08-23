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

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <ostream>
#include <cstdlib>

using namespace std;

class CVector
{
public:
	double x,y;
public:
	CVector(void){}

	CVector(double lenght)
	{
		int xSign = rand()%10>5 ? 1 : -1;
		int ySign = rand()%10>5 ? 1 : -1;

		x = (rand()%100)*xSign, 
		y = (rand()%100)*ySign;

		Normalise();
		x*=lenght;
		y*=lenght;
	}

	CVector(double Ix,double Iy): x(Ix), y(Iy){}

	void Set(double x1, double y1) {x=x1; y=y1;};
	void Rescale(double lenght) { Normalise(); (*this)*= lenght; } 
	CVector &operator /=(const double Scalar)  { x /= Scalar; y /= Scalar;		return *this; }

	CVector &operator *=(const double Scalar)  { x *= Scalar; y *= Scalar;		return *this; }
	
	CVector &operator +=(const CVector &Other) { x += Other.x;	y += Other.y;	return *this; }

	CVector &operator -=(const CVector &Other) { x -= Other.x;	y -= Other.y;	return *this; }

	double operator ^ (const CVector &V) const {	return (x * V.y) - (y * V.x); } // cross product

	double operator * (const CVector &V) const {	return (x*V.x) + (y*V.y); } // dot product

	CVector operator * (double  s)			const	{	return CVector(x*s, y*s); }
	
	CVector operator / (double  s)			const	{	return CVector(x/s, y/s); }
	
	CVector operator + (const CVector &V)	const	{	return CVector(x+V.x, y+V.y); }
		
	CVector operator - (const CVector &V)	const	{	return CVector(x-V.x, y-V.y); }

	bool operator ==(const CVector &V) const { return x==V.x && y==V.y; } 
	bool operator !=(const CVector &V) const { return x!=V.x || y!=V.y; } 

	friend CVector operator * (double k, const CVector& V) {	return CVector(V.x*k, V.y*k); }

	friend ostream &operator<<( ostream &out, const CVector &v )
	{
		out << "x: " << v.x << " y: " << v.y << endl;
		return out;
	}

	CVector operator -(void) const { return CVector(-x, -y); }
	
	double Length(void) const { return (double) sqrt(x*x + y*y); }
	double LengthSq() const { return x*x + y*y; }

	double Normalise(void) 
	{	
		double fLength = Length();	
		
		if (fLength == 0.0f) 
			return 0.0f; 
			
		(*this) /= fLength;
	
		return fLength;	
	}

	CVector Direction(void) const
	{
		CVector Temp(*this);

		Temp.Normalise();
		return Temp;
	}
	
	void Perp()
	{
		double temp = x;
		x = -y;
		y = temp;
	}

	void Clear()
	{
		Set(0,0);
	}

	CVector GetPerp() const
	{
       CVector a= *this;
		return CVector(-a.y, a.x);
	}

	CVector& rotate(double angle)
	{
		double tx = x;
		x =  x * cos(angle) - y * sin(angle);
		y = tx * sin(angle) + y * cos(angle);
		return *this;
	}

	CVector& transform(const CVector& trans, double rot)
	{
		CVector D = *this;
		D.rotate(rot);
		*this = trans + D;
		return *this;
	}
};

#endif