
#include "matrices.h"

MATRIX::MATRIX(int Rows,int Cols)
{
	int i,j;

	M = NULL;
	if ((Rows > MAX_ROWS) || (Cols > MAX_COLUMNS))
		throw "[   THROW] Module MATRICES.H : out of max sizes in MATRIX::MATRIX(int,int)";
	M = new float_t* [Rows];
	if (!M)
		throw "[   THROW] Module MATRICES.H : out of memory in MATRIX::MATRIX(int,int)";
	for (i = 0; i < Rows; i++)
	{
		M[i] = new float_t [Cols];
		if (!M[i])
		{
			for (int k = i - 1; k >= 0; k--)
				delete [] M[k];
			delete [] M;
			M = NULL;
			throw "[   THROW] Module MATRICES.H : out of memory in MATRIX::MATRIX(int,int)";
		}
		for (j = 0; j < Cols; j++)
			M[i][j] = 0.0;
	}

	rows = Rows; cols = Cols;
}

MATRIX::MATRIX(const MATRIX &MM)
{
	int i,j;

	if (!MM.M) throw "[   THROW] Module MATRICES.H : access violation in MATRIX::MATRIX(const MATRIX&)";
	cols = MM.cols;
	rows = MM.rows;
	M = new float_t* [MM.rows];
	if (!M)
		throw "[   THROW] Module MATRICES.H : out of memory in MATRIX::MATRIX(const MATRIX&)";
	for (i = 0; i < MM.rows; i++)
	{
		M[i] = new float_t [MM.cols];
		if (!M[i])
		{
			for (int k = i - 1; k >= 0; k--)
				delete [] M[k];
			delete [] M;
			M = NULL;		
			throw "[   THROW] Module MATRICES.H : out of memory in MATRIX::MATRIX(const MATRIX&)";
		}
		for (j = 0; j < MM.cols; j++)
			M[i][j] = MM.M[i][j];
	}
}

MATRIX::MATRIX(const float_t **m)
{
	if (!m)
		throw "[   THROW] Module MATRICES.H : get of NULL pointer in MATRIX::MATRIX(float_t**)";

}
/*
MATRIX::MATRIX(const VECTOR2D &V)
{
	MATRIX(2,1);
	M[0][0] = V.x;
	M[1][0] = V.y;
}

MATRIX::MATRIX(const VECTOR3D &V)
{
	MATRIX(3,1);
	M[0][0] = V.x;
	M[1][0] = V.y;
	M[2][0] = V.z;
}
*/
const MATRIX& MATRIX::operator =(const MATRIX &MM)
{
	int i,j;

	if (&MM == this) return *this;
	if (!MM.M) return *this;
	if (M)
	{
		for (i = 0; i < rows; i++)
			delete [] M[i];
		delete M;
		M = NULL;
	}
	cols = MM.cols;
	rows = MM.rows;
	M = new float_t* [MM.rows];
	if (!M)
	{
		return *this;
	}
	for (i = 0; i < MM.rows; i++)
	{
		M[i] = new float_t [MM.cols];
		if (!M[i])
		{
			for (int k = i - 1; k >= 0; k--)
				delete [] M[k];
			delete [] M;
			M = NULL;		
			return *this;
		}
		for (j = 0; j < MM.cols; j++)
			M[i][j] = MM.M[i][j];
	}
	return *this;
}

float_t& MATRIX::operator [](int i)
{
	if (!M)
	{
		throw "[   THROW] Module MATRICES.H : need of returning value, which not exist in MATRIX::operator[]()";
	}
	if ((i < 0) || (i > cols * rows - 1))
	{
		throw "[   THROW] Module MATRICES.H : out of range in MATRIX::operator[]()";
	}

	return (**(M + i));
}

void MATRIX::Print(char *name)
{
	printf("Matrix %s:\n",name);
	if (!M)
	{
		printf(" [ NULL ]\n");
		return;
	}
	for (int i = 0; i < rows; i++)
	{
		printf(" [");
		for (int j = 0; j < cols; j++)
			printf("%7.2f",M[i][j]);
		printf(" ]\n");
	}
}

bool MATRIX::Exist()
{
	if (M)
		return true;
	return false;
}

void MATRIX::Clear()
{
	Fill(0.0);
}

void MATRIX::Fill(float_t fval)
{
	if (!M) return;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			M[i][j] = fval;
}

void MATRIX::Identity()
{
	if (!M) return;
	if (rows != cols) return;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (i == j)
				M[i][j] = 1.0;
			else
				M[i][j] = 0.0;
}			
MATRIX MATRIX::operator +(const MATRIX &MM)
{
	if ((cols != MM.cols) || (rows != MM.cols))
		throw "[   THROW] Module MATRICES.H : matrices not equivalent in MATRIX::operator+()";

	MATRIX S(rows,cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			S.M[i][j] = M[i][j] + MM.M[i][j];

	return MATRIX(S);
}

MATRIX MATRIX::operator -(const MATRIX &MM)
{
	if ((cols != MM.cols) || (rows != MM.cols))
		throw "[   THROW] Module MATRICES.H : matrices not equivalent in MATRIX::operator-()";

	MATRIX S(rows,cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			S.M[i][j] = M[i][j] - MM.M[i][j];

	return MATRIX(S);
}

MATRIX MATRIX::operator *(const MATRIX &MM)
{
	if ((cols != MM.rows))
		throw "[   THROW] Module MATRICES.H : matrices not equivalent in MATRIX::operator-()";

	MATRIX S(rows,MM.cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < MM.cols; j++)
			for (int k = 0; k < cols; k++)
				S.M[i][j] += M[i][k] * MM.M[k][j];

	return MATRIX(S);
}

MATRIX::~MATRIX()
{
	int i;

	if (!M) return;

	for (i = 0; i < rows; i++)
		delete [] M[i];
	delete M;
	M = NULL;
}