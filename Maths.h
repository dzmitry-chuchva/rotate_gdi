
#ifndef __MATHS_H
#define __MATHS_H

#include <windows.h>	// time functions
#include <stdio.h>		// file i/o
#include <math.h>		// sin, cos

#define PI (3.141592654f)
#define RAD2DEG(x) ((x) / PI * 180)
#define DEG2RAD(x) ((x) * PI / 180)
#define BIG (10e30f)
#define MATHLOGFILE "mathlog.txt"
#define VERSION "0.9a"

#define FastCos fcos
#define FastSin fsin
#define FastSqrt fsqrt

typedef float float_t;

void Build_SinCos_Tables();
float_t FastCos(float_t theta);
float_t FastSin(float_t theta);
float_t FastSqrt(float_t q2);
void WriteLog(const char *string);

#endif