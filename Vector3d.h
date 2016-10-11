
#ifndef __VECTOR3D_H
#define __VECTOR3D_H

#include <stdio.h>
#include "maths.h"
//#include "matrices.h"

class VECTOR3D {
	public:

		float_t x,y,z;

		VECTOR3D() : x (0.0), y (0.0), z(0.0) {};
		VECTOR3D(float_t X, float_t Y,float_t Z) : x (X), y (Y), z(Z) {};
		VECTOR3D(float_t *m);
		VECTOR3D(const VECTOR3D &begin,const VECTOR3D &end);
		VECTOR3D(const MATRIX& MM);
		void Print(char *name) { printf("Vector %s : x = %.2f, y = %.2f, z = %.2f\n",name,x,y,z); }
		float_t operator *(const VECTOR3D &V1);
		VECTOR3D operator *(float_t scalar);
		VECTOR3D operator +(const VECTOR3D &V1);
		VECTOR3D operator -(const VECTOR3D &V1);
		float_t Length();
		void Normalize();
		float_t operator ^(VECTOR3D &V1);
		friend VECTOR3D operator *(float_t scalar,const VECTOR3D &V1);
		friend VECTOR3D operator *(const MATRIX &MM,VECTOR3D &V);
		VECTOR3D xVector(const VECTOR3D &V1);
		bool IsNull();
		bool Between(VECTOR3D &V1,float_t &angle);
		bool operator ||(VECTOR3D &V1);
};

VECTOR3D::VECTOR3D(float_t *m)
{
	if (m)
	{
		x = m[0];
		y = m[1];
		z = m[2];
	} else
	{
		x = 0;
		y = 0;
		z = 0;
	}
}

VECTOR3D::VECTOR3D(const VECTOR3D &begin,const VECTOR3D &end)
{
	x = end.x - begin.x;
	y = end.y - begin.y;
	z = end.z - begin.z;
}

VECTOR3D::VECTOR3D(const MATRIX& MM)
{
	if ((MM.cols != 1) || (MM.rows != 3))
		throw "[   THROW] Module VECTOR3D.H : initializing vector with invalid matrix in \
													VECTOR3D::VECTOR3D(const MATRIX& MM)";
	x = MM.M[0][0];
	y = MM.M[1][0];
	z = MM.M[2][0];
}

float_t VECTOR3D::operator *(const VECTOR3D& V1)
{
	return (x * V1.x + y * V1.y + z * V1.z);
}

VECTOR3D VECTOR3D::operator *(float_t scalar)
{
	return VECTOR3D(x * scalar,y * scalar,z * scalar);
}

VECTOR3D VECTOR3D::operator +(const VECTOR3D& V1)
{
	return VECTOR3D(V1.x + x,V1.y + y,V1.z + z);
}

VECTOR3D operator *(float_t scalar,const VECTOR3D &V1)
{
	return VECTOR3D(V1.x * scalar,V1.y * scalar,V1.z * scalar);
}

VECTOR3D operator *(const MATRIX &MM,VECTOR3D &V)
{
	if ((MM.cols != 3) || (MM.rows != 3))
		throw "[   THROW] Module VECTOR3D.H : vector and matrix incompatible in \
													friend operator *(const MATRIX&,VECTOR3D&)";

	MATRIX MV(V);

	return VECTOR3D(MM * MV);
}

VECTOR3D VECTOR3D::xVector(const VECTOR3D &V1)
{
	return VECTOR3D(y * V1.z - z * V1.y,z * V1.x - x * V1.z,x * V1.y - y * V1.x);
}

float_t VECTOR3D::Length()
{
	return FastSqrt(x * x + y * y + z * z);
}

void VECTOR3D::Normalize()
{
	float_t l = Length();

	x = x / l; y = y / l; z = z / l; 
}

VECTOR3D VECTOR3D::operator -(const VECTOR3D &V1)
{
	return VECTOR3D(x - V1.x,y - V1.y,z - V1.z);
}

float_t VECTOR3D::operator ^(VECTOR3D &V1)
{
	if (!IsNull() && !V1.IsNull())
		return ((float_t)acos(VECTOR3D(x,y,z) * V1 / (Length() * V1.Length())));
	throw "[   THROW] Module VECTOR3D.H : operating with zero vector in VECTOR3D::operator^()";
}

bool VECTOR3D::IsNull()
{
	if ((x == 0) && (y == 0) && (z == 0))
		return true;
	return false;
}

bool VECTOR3D::operator ||(VECTOR3D &V1)
{
	float_t angle;

	Between(V1,angle);
	if (angle == 0) return true;
	return false;
}

bool VECTOR3D::Between(VECTOR3D &V1,float_t &angle)
{
	if ((IsNull()) || (V1.IsNull()))
		return false;
	angle = VECTOR3D(x,y,z) ^ V1;
	return true;
}

typedef VECTOR3D POINT3D;
typedef VECTOR3D* VECTOR3D_PTR, POINT3D_PTR;

static const VECTOR3D I3D(1.0,0.0,0.0);
static const VECTOR3D J3D(0.0,1.0,0.0);
static const VECTOR3D K3D(0.0,0.0,1.0);


#endif