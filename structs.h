
#ifndef __STRUCTS_H
#define __STRUCTS_H

#define POINT3TO4(p3,p4) (p4).x = (p3).x; (p4).y = (p3).y; (p4).z = (p3).z; (p4).w = 1.0
#define POINT4TO3(p4,p3) p3.x = p4.x / p4.w; p3.y = p4.y / p4.w; p3.z = p4.z / p4.w

typedef struct {
	float_t x,y,z;
} POINT3;

typedef struct {
	float_t x,y,z,w;
} POINT4;

typedef struct {
	int color;
	int need;

	POINT4 *vlist;
	int vnum[3];
	int wires[3];	// 0 : 0 -> 1, 1 : 1 -> 2, 2 : 2 -> 0
} POLY3;

#endif