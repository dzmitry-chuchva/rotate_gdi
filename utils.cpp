
#include "utils.h"

void make_transform_matrix(MATRIX &MM,float_t x_tr,float_t y_tr,float_t z_tr)
{
	if (!MM.Exist()) return;

	MM.Identity();
	MM.M[3][0] = x_tr;
	MM.M[3][1] = y_tr;
	MM.M[3][2] = z_tr;
}

void make_rotatex_matrix(MATRIX &MM,float_t x_rot)
{
	if (!MM.Exist()) return;

	MM.Identity();
	MM.M[1][1] = FastCos(x_rot);
	MM.M[2][2] = MM.M[1][1];
	MM.M[2][1] = -FastSin(x_rot);
	MM.M[1][2] = -MM.M[2][1];
}

void make_rotatey_matrix(MATRIX &MM,float_t y_rot)
{
	if (!MM.Exist()) return;

	MM.Identity();
	MM.M[0][0] = FastCos(y_rot);
	MM.M[2][2] = MM.M[0][0];
	MM.M[2][0] = FastSin(y_rot);
	MM.M[0][2] = -MM.M[2][0];
}

void make_rotatez_matrix(MATRIX &MM,float_t z_rot)
{
	if (!MM.Exist()) return;

	MM.Identity();
	MM.M[0][0] = FastCos(z_rot);
	MM.M[1][1] = MM.M[0][0];
	MM.M[1][0] = -FastSin(z_rot);
	MM.M[0][1] = -MM.M[1][0];
}

void make_screen_matrix(MATRIX &MM,int fov,int screen_width,int screen_height)
{
	float_t focus = ((float_t)screen_width - 1) / 2 * tan(DEG2RAD((float_t)fov / 2));
	float_t format_corr = (float_t)screen_width / (float_t)screen_height,
		xcorr = ((float_t)screen_width - 1) / 2, ycorr = ((float_t)screen_height - 1) / 2;

	if (!MM.Exist())
		return;

	MM.Identity();
	MM.M[0][0] = focus;
	MM.M[1][1] = -focus;
	MM.M[3][3] = 0;
	MM.M[2][3] = 1;
	MM.M[2][0] = xcorr;
	MM.M[2][1] = ycorr;
}

void make_scale_matrix(MATRIX &MM,float_t kx,float_t ky,float_t kz)
{
	if (!MM.Exist()) return;

	MM.Identity();
	MM.M[0][0] = kx;
	MM.M[1][1] = ky;
	MM.M[2][2] = kz;
}