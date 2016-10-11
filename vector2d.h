
#ifndef __VECTOR2D_H
#define __VECTOR2D_H

#include <stdio.h>
#include "maths.h"
//#include "matrices.h"

class VECTOR2D {
	public:

		float_t x,y;

		VECTOR2D() : x (0.0), y (0.0) {};
		VECTOR2D(float_t X, float_t Y) : x (X), y (Y) {};
		VECTOR2D(float_t *m);
		VECTOR2D(const VECTOR2D &begin,const VECTOR2D &end);
		VECTOR2D(const MATRIX &MM);
		void Print(char *name) { printf("Vector %s : x = %.2f, y = %.2f\n",name,x,y); }
		float_t operator *(const VECTOR2D &V1);
		VECTOR2D operator -(const VECTOR2D &V1);
		float_t Length();
		void Normalize();
		bool IsNull();
		float_t operator ^(VECTOR2D &V1);
		bool Between(VECTOR2D &V1,float_t &angle);
		bool operator ||(VECTOR2D &V1);
		VECTOR2D operator *(float_t scalar);
		VECTOR2D operator *(const MATRIX &MM);
		VECTOR2D operator +(const VECTOR2D &V1);
		friend VECTOR2D operator *(float_t scalar,const VECTOR2D &V1);
		friend VECTOR2D operator *(const MATRIX &MM,VECTOR2D &V);
};

VECTOR2D::VECTOR2D(float_t *m)
{
	if (m)
	{
		x = m[0];
		y = m[1];
	} else
	{
		x = 0;
		y = 0;
	}
}

VECTOR2D::VECTOR2D(const VECTOR2D &begin,const VECTOR2D &end)
{
	x = end.x - begin.x;
	y = end.y - begin.y;
}

VECTOR2D::VECTOR2D(const MATRIX& MM)
{
	if ((MM.cols != 1) || (MM.rows != 2))
		throw "[   THROW] Module VECTOR2D.H : initializing vector with invalid matrix in \
													VECTOR2D::VECTOR2D(const MATRIX& MM)";
	x = MM.M[0][0];
	y = MM.M[1][0];
}

float_t VECTOR2D::operator *(const VECTOR2D& V1)
{
	return (x * V1.x + y * V1.y);
}

VECTOR2D VECTOR2D::operator *(float_t scalar)
{
	return VECTOR2D(x * scalar,y * scalar);
}

VECTOR2D VECTOR2D::operator +(const VECTOR2D& V1)
{
	return VECTOR2D(V1.x + x,V1.y + y);
}

VECTOR2D operator *(float_t scalar,const VECTOR2D &V1)
{
	return VECTOR2D(V1.x * scalar,V1.y * scalar);
}

VECTOR2D operator *(const MATRIX &MM,VECTOR2D &V)
{
	if ((MM.cols != 2) || (MM.rows != 2))
		throw "[   THROW] Module VECTOR2D.H : vector and matrix incompatible in \
													friend operator *(const MATRIX&,VECTOR2D&)";

	MATRIX MV(V);

	return VECTOR2D(MM * MV);
}

float_t VECTOR2D::Length()
{
	return FastSqrt(x * x + y * y);
}

void VECTOR2D::Normalize()
{
	float_t l = Length();

	x = x / l; y = y / l; 
}

VECTOR2D VECTOR2D::operator -(const VECTOR2D &V1)
{
	return VECTOR2D(x - V1.x,y - V1.y);
}

float_t VECTOR2D::operator ^(VECTOR2D &V1)
{
	if (!IsNull() && !V1.IsNull())
		return ((float_t)acos(VECTOR2D(x,y) * V1 / (Length() * V1.Length())));
//	return BIG; // here must be something not so stupid, i think, maybe "throw something;"
	throw "[   THROW] Module VECTOR2D.H : operating with zero vector in VECTOR2D::operator^()";
}

bool VECTOR2D::IsNull()
{
	if ((x == 0) && (y == 0))
		return true;
	return false;
}

bool VECTOR2D::operator ||(VECTOR2D &V1)
{
	float_t angle;

	Between(V1,angle);
	if (angle == 0) return true;
	return false;
}

bool VECTOR2D::Between(VECTOR2D &V1,float_t &angle)
{
	if ((IsNull()) || (V1.IsNull()))
		return false;
	angle = VECTOR2D(x,y) ^ V1;
	return true;
}

typedef VECTOR2D POINT2D;
typedef VECTOR2D* VECTOR2D_PTR, POINT2D_PTR;

static const VECTOR2D I2D(1.0,0.0);
static const VECTOR2D J2D(0.0,1.0);

#endif