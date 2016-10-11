
#ifndef __DRAWING_H
#define __DRAWING_H

#include "globaldefs.h"		// PULSE, DEBUG defs
#include <windows.h>		// finding files
#include "graphs.h"			// lines drawing
#include "maths.h"			// logging
#include "structs.h"		// structs
#include "utils.h"			// 3D special matrices building
#include "matrices.h"		// matrices

#define KEY_DOWN(key) ((GetAsyncKeyState(key) & 0x8000) ? 1 : 0)
#define KEY_UP(key) ((GetAsyncKeyState(key) & 0x8000) ? 0 : 1)

#define MODELMASK "*.ase"
#define DEFAULT_COLOR 255

typedef unsigned char UCHAR;

extern int screen_width, screen_height;		// screen geom
extern HDC hdc;

int gmain(int,int);
int ginit(int,int);
void gcleanup();
int load_my_model(char *);
int load_first_model();
int load_next_model();
void duplicate_vlist();
void make_const_matrix(int,int);
void visualize();
void apply_actions(MATRIX&);
void cull_backfaces();
void compute_uvncam_matrix(MATRIX &MM,POINT3 &p);
void compute_normal(POINT4 &p1,POINT4 &p2,POINT4 &p3,POINT3 &norm);
float_t compute_length(POINT3 &v);
void normalize(POINT3 &v);
void compute_vector_cross(POINT3 &v,POINT3 &n,POINT3 &u);
float_t compute_vector_product(POINT3 &v,POINT3 &u);
int reset_search();

#endif