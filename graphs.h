
#ifndef __GRAPHS_H
#define __GRAPHS_H

#include <windows.h>		// SetPixel

typedef unsigned char UCHAR;

extern int screen_width,screen_height;
extern HDC hdc;

void line(int,int,int,int,UCHAR);
void clearscreen();
void Draw_Line(int,int,int,int,UCHAR);

#endif