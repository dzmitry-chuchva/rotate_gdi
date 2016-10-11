
#include "drawing.h"

HANDLE hFind = NULL;
WIN32_FIND_DATA fd;

POINT4 *vlist_local = NULL; int v_num = 0;
POINT4 *vlist = NULL;
POLY3 *plist = NULL; int p_num = 0;

float_t near_z = 0.1, far_z = 200;

float_t objx_rot = 0, objy_rot = 0, objz_rot = 0;		// our rotations angles
float_t drot = .5;
#if (PULSE)
float_t pulse = 1.0, sinpulse = 90;					// pulsing effect
float_t dsin = .4;
#endif
MATRIX after_cam_matrix(4,4), cam_matrix(4,4);

float_t obj_posx = 0, obj_posy = 0, obj_posz = 50;		// object position
POINT3 cam_pos = { 0, 0, 0 };					// camera position
POINT3 cam_dir = { obj_posx, obj_posy , obj_posz };		// camera direction
int fov = 90;											// FOV

int gmain(int screen_width,int screen_height)
{
	MATRIX m(4,4),mtemp(4,4);
	
	try {

		if (KEY_DOWN(VK_SPACE))
		{
			if (!load_next_model())
				if (!reset_search())
					throw "search engine crash";
			Sleep(100);
		}

#ifndef DEBUG
		clearscreen();
#endif

		duplicate_vlist();						// dont damage our model
		make_rotatex_matrix(m,objx_rot);
		make_rotatey_matrix(mtemp,objy_rot);
		m = m * mtemp;
		make_rotatez_matrix(mtemp,objz_rot);
		m = m * mtemp;
#if (PULSE)
		make_scale_matrix(mtemp,pulse,pulse,pulse);
		m = m * mtemp;
#endif
		make_transform_matrix(mtemp,obj_posx,obj_posy,obj_posz);// make transform matrix for figure
															// we have only one object, so
															// dont complicate program
															// by adding support for multiple
															// objects
		m = m * mtemp;
		apply_actions(m);
		cull_backfaces();
		m = cam_matrix * after_cam_matrix;
		apply_actions(m);
		visualize();
		objx_rot += drot;
		objy_rot += drot;
		objz_rot += drot;

		if (objx_rot > 360)
			objx_rot = 0;

		if (objy_rot > 360)
			objy_rot = 0;

		if (objz_rot > 360)
			objz_rot = 0;

#if (PULSE)
		if (sinpulse > 90)
			dsin = - dsin;
		if (sinpulse < 0)
			dsin = - dsin;

		sinpulse += dsin;

		pulse = FastSin(sinpulse);
#endif
		
	}
	catch (const char* s)
	{
		WriteLog(s);
		gcleanup();
		return 0;
	}
	return 1;
}

int ginit(int screen_width,int screen_height)
{
	Build_SinCos_Tables();
	WriteLog("cos, sin tables done");
	if (!load_first_model())
	{
		WriteLog("models not found");
		return 0;
	}
	make_const_matrix(screen_width,screen_height);
	WriteLog("model done");

	return 1;
}

void gcleanup()
{
	if (vlist_local)
		delete [] vlist_local;
	if (vlist)
		delete [] vlist;
	if (plist)
		delete [] plist;
	if (hFind)
		FindClose(hFind);
}

int load_my_model(char *filename)
{
	FILE *f = fopen(filename,"rt");

	// just reads vertecies and polies (sections *MESH_VERTEX_LIST and *MESH_FACE_LIST)
	// no color, materials, lights and other stuff supported

	if (!f)
		return 0;

	char buff[200]; char stemp[200];
	const char fmt_indicator[] = "*3DSMAX_ASCIIEXPORT",
		mesh_section[] = "*MESH", v_section[] = "*MESH_VERTEX_LIST", f_section[] = "*MESH_FACE_LIST";
	UCHAR m_found = 0,v_found = 0,f_found = 0;

	fgets(buff,200,f);
	if (!strstr(buff,fmt_indicator))
	{
		fclose(f);
		return 0;
	}

	while ((fgets(buff,200,f)) && (!(m_found && v_found && f_found)))
	{
		if ((strstr(buff,mesh_section)) && (!m_found))	// *MESH found
		{
			m_found = 1;
			fgets(buff,200,f);		// skip TIMEVALUE line

			fgets(buff,200,f);		// get NUMVERTEX
			sscanf(buff,"%s %d",stemp,&v_num);
			fgets(buff,200,f);		// get NUMFACES
			sscanf(buff,"%s %d",stemp,&p_num);
			if ((v_num <= 0) || (p_num <= 0))
			{
				fclose(f);
				return 0;
			}

			vlist_local = new POINT4 [v_num];
			vlist = new POINT4 [v_num];
			plist = new POLY3 [p_num];
			if ((!vlist_local) || (!vlist) || (!plist))
			{
				fclose(f);
				return 0;	// gcleanup() frees memory
			}

			for (int p_ind = 0; p_ind < p_num; p_ind++)
			{
				plist[p_ind].color = DEFAULT_COLOR;
				plist[p_ind].vlist = vlist;
				plist[p_ind].need = 1;
			}

		}

		if ((strstr(buff,v_section)) && (m_found) && (!v_found))
		{
			v_found = 1;
			int v_ind_from_file;
			float_t x,y,z;

			for (int v_ind = 0; v_ind < v_num; v_ind++)
			{
				fgets(buff,200,f);
				sscanf(buff,"%s %d %f %f %f",stemp,&v_ind_from_file,&x,&y,&z);
				vlist_local[v_ind_from_file].w = 1.0;
				vlist_local[v_ind_from_file].x = x;
				vlist_local[v_ind_from_file].y = y;
				vlist_local[v_ind_from_file].z = z;
			}
		}

		if ((strstr(buff,f_section)) && (m_found) && (v_found) && (!f_found))
		{
			f_found = 1;
			int p_ind_from_file;
			int v1,v2,v3; int wire01,wire12,wire20;

			for (int p_ind = 0; p_ind < p_num; p_ind++)
			{
				fgets(buff,200,f);
				sscanf(buff,"%s %d %s %s %d %s %d %s %d %s %d %s %d %s %d %s",
					stemp,&p_ind_from_file,stemp,stemp,&v1,stemp,&v2,stemp,&v3,
					stemp,&wire01,stemp,&wire12,stemp,&wire20,stemp);
				plist[p_ind_from_file].vnum[0] = v1; plist[p_ind_from_file].wires[0] = wire01;
				plist[p_ind_from_file].vnum[1] = v2; plist[p_ind_from_file].wires[1] = wire12;
				plist[p_ind_from_file].vnum[2] = v3; plist[p_ind_from_file].wires[2] = wire20;
			}
		}
	}
	
	fclose(f);

	return 1;
}

void duplicate_vlist()
{
	if (!vlist)
	{
		WriteLog("vlist = NULL");
		return;
	}

	for (int i = 0; i < v_num; i++)
		vlist[i] = vlist_local[i];
}

void make_const_matrix(int screen_width,int screen_height)
{
	MATRIX mtemp(4,4);

	make_transform_matrix(cam_matrix,-cam_pos.x,-cam_pos.y,-cam_pos.z);
	compute_uvncam_matrix(mtemp,cam_dir);
	cam_matrix = cam_matrix * mtemp; 

	make_screen_matrix(after_cam_matrix,fov,screen_width,screen_height);
}

void visualize()
{
	POINT3 p1,p2,p3;

	for (int i = 0; i < p_num; i++)
	{
		if (!plist[i].need)
		{
			plist[i].need = 1;
			continue;
		}
		POINT4TO3(plist[i].vlist[plist[i].vnum[0]],p1);
		POINT4TO3(plist[i].vlist[plist[i].vnum[1]],p2);
		POINT4TO3(plist[i].vlist[plist[i].vnum[2]],p3);

#ifndef DEBUG

		if (plist[i].wires[0])
			Draw_Line((int)(p1.x + 0.5),(int)(p1.y + 0.5),(int)(p2.x + 0.5),(int)(p2.y + 0.5),plist[i].color);

		if (plist[i].wires[1])
			Draw_Line((int)(p2.x + 0.5),(int)(p2.y + 0.5),(int)(p3.x + 0.5),(int)(p3.y + 0.5),plist[i].color);

		if (plist[i].wires[2])
			Draw_Line((int)(p3.x + 0.5),(int)(p3.y + 0.5),(int)(p1.x + 0.5),(int)(p1.y + 0.5),plist[i].color);

#endif
	}
}

void apply_actions(MATRIX& MM)
{
	MATRIX v(1,4);

	for (int i = 0; i < v_num; i++)
	{
		v.M[0][0] = vlist[i].x;
		v.M[0][1] = vlist[i].y;
		v.M[0][2] = vlist[i].z;
		v.M[0][3] = vlist[i].w;

		v = v * MM;

		vlist[i].x = v.M[0][0];
		vlist[i].y = v.M[0][1];
		vlist[i].z = v.M[0][2];
		vlist[i].w = v.M[0][3];
	}
}

void cull_backfaces()
{
	POINT3 view, norm;

	for (int i = 0; i < p_num; i++)
	{
		view.x = cam_pos.x - plist[i].vlist[plist[i].vnum[0]].x;
		view.y = cam_pos.y - plist[i].vlist[plist[i].vnum[0]].y;
		view.z = cam_pos.z - plist[i].vlist[plist[i].vnum[0]].z;
		compute_normal(plist[i].vlist[plist[i].vnum[0]],plist[i].vlist[plist[i].vnum[1]],
			plist[i].vlist[plist[i].vnum[2]],norm);
		if (compute_vector_product(view,norm) <= 0)
			plist[i].need = 0;
	}
}

void compute_uvncam_matrix(MATRIX &MM,POINT3 &p)
{
	POINT3 n = { p.x - cam_pos.x, p.y - cam_pos.y, p.z - cam_pos.z };
	POINT3 v = { 0, 1, 0 }, u;


	if (!MM.Exist())
		return;

	compute_vector_cross(v,n,u);
	compute_vector_cross(n,u,v);
	
	normalize(v); normalize(u); normalize(n);

	MM.Identity();
	MM.M[0][0] = u.x; MM.M[0][1] = v.x; MM.M[0][2] = n.x;
	MM.M[1][0] = u.y; MM.M[1][1] = v.y; MM.M[1][2] = n.y;
	MM.M[2][0] = u.z; MM.M[2][1] = v.z; MM.M[2][2] = n.z;
}

void compute_normal(POINT4 &p1,POINT4 &p2,POINT4 &p3,POINT3 &norm)
{
	POINT3 v1 = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z },
		v2 = { p1.x - p3.x, p1.y - p3.y, p1.z - p3.z };

	norm.x = v1.y * v2.z - v1.z * v2.y;	
	norm.y = v1.z * v2.x - v1.x * v2.z;
	norm.z = v1.x * v2.y - v1.y * v2.x;
}

float_t compute_length(POINT3 &v)
{
	return (float_t)fsqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void normalize(POINT3 &v)
{
	float_t length = compute_length(v);

	v.x = v.x / length;
	v.y = v.y / length;
	v.z = v.z / length;
}

void compute_vector_cross(POINT3 &v,POINT3 &n,POINT3 &u)
{
	u.x = v.y * n.z - v.z * n.y;
	u.y = v.z * n.x - v.x * n.z;
	u.z = v.x * n.y - v.y * n.x;
}

float_t compute_vector_product(POINT3 &v,POINT3 &u)
{
	return (v.x * u.x + v.y * u.y + v.z * u.z);
}

int load_first_model()
{
	hFind = FindFirstFile(MODELMASK,&fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;

	if (!load_my_model(fd.cFileName))
		return 0;

	return 1;
}

int load_next_model()
{
	if (vlist_local)
		delete [] vlist_local;
	if (vlist)
		delete [] vlist;
	if (plist)
		delete [] plist;

	if (!FindNextFile(hFind,&fd))
		return 0;

	if (!load_my_model(fd.cFileName))
		return 0;

	return 1;
}

int reset_search()
{
	if (hFind)
	{
		FindClose(hFind);
		hFind = NULL;
	}

	if (!load_first_model())
		return 0;

	return 1;
}
