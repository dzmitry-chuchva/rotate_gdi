
#ifndef __MATRICES_H
#define __MATRICES_H

#include <stdio.h>			// printf
#include "maths.h"			// logging
//#include "vector2d.h"
//#include "vector3d.h"

#define MAX_COLUMNS 100
#define MAX_ROWS 100

class MATRIX {
	int cols,rows;
public:
	float_t **M;

	MATRIX(int Rows = 2,int Cols = 2);
	MATRIX(const MATRIX &MM);
	MATRIX(const float_t **m);
//	MATRIX(const VECTOR2D &V);
//	MATRIX(const VECTOR3D &V);
	const MATRIX& operator =(const MATRIX &MM);
	float_t& operator [](int i);
	bool Exist();
	void Fill(float_t fval);
	void Clear();
	void Print(char *name);
	void Identity();
	MATRIX operator +(const MATRIX &MM);
	MATRIX operator -(const MATRIX &MM);
	MATRIX operator *(const MATRIX &MM);
	~MATRIX();
};

#endif