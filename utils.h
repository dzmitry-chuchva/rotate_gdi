
#ifndef __UTILS_H
#define __UTILS_H

#include "matrices.h"

void make_transform_matrix(MATRIX &MM,float_t x_tr,float_t y_tr,float_t z_tr);
void make_rotatex_matrix(MATRIX &MM,float_t x_rot);
void make_rotatey_matrix(MATRIX &MM,float_t y_rot);
void make_rotatez_matrix(MATRIX &MM,float_t z_rot);
void make_scale_matrix(MATRIX &MM,float_t kx,float_t ky,float_t kz);
void make_screen_matrix(MATRIX &MM,int fov,int screen_width,int screen_height);

#endif