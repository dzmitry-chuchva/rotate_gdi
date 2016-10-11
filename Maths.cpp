
#include "maths.h"

static float_t sin_table[361];
static float_t cos_table[361];
static logstarted = 0;

char *months[] = 
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

char *days[] =
{
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

void Build_SinCos_Tables()
{
	for (int i = 0; i < 361; i++)
	{
		sin_table[i] = sin(DEG2RAD((float)i));
		cos_table[i] = cos(DEG2RAD((float)i));
	}
}

float_t FastCos(float_t theta)
{
	theta = fmod(theta,360);
	if (theta < 0) theta += 360.0;

	int theta_int = (int)theta;
	float_t theta_frac = theta - theta_int;

	return (cos_table[theta_int] + theta_frac * (cos_table[theta_int + 1] - cos_table[theta_int]));
}

float_t FastSin(float_t theta)
{
	theta = fmod(theta,360);
	if (theta < 0) theta += 360.0;

	int theta_int = (int)theta;
	float_t theta_frac = theta - theta_int;

	return (sin_table[theta_int] + theta_frac * (sin_table[theta_int + 1] - sin_table[theta_int]));
}

float_t FastSqrt(float_t q2)
{	
	float_t q;

	_asm {
		mov eax,q2
		sub eax,0x3f800000
		sar eax,1
		add eax,0x3f800000
		mov q,eax
	}

	return q;
}

void WriteLog(const char *string)
{
	FILE *f = NULL;
	SYSTEMTIME time;
	char *fmttime = new char [8 + 1];	

	GetLocalTime(&time);

	memset(fmttime,0,9);
	if (time.wHour < 10)
		sprintf(fmttime,"0%d:",time.wHour);
	else
		sprintf(fmttime,"%d:",time.wHour);
	if (time.wMinute < 10)
		sprintf(fmttime + 3,"0%d:",time.wMinute);
	else
		sprintf(fmttime + 3,"%d:",time.wMinute);
	if (time.wSecond < 10)
		sprintf(fmttime + 6,"0%d",time.wSecond);
	else
		sprintf(fmttime + 6,"%d",time.wSecond);


	if (!logstarted)
	{
		f = fopen(MATHLOGFILE,"wt");
		fprintf(f,"[    INFO] Math Subsystem ver.%s, log started at %s %s %2d %s %d\n",
			VERSION,months[time.wMonth - 1],days[time.wDayOfWeek],time.wDay,fmttime,time.wYear);
		logstarted = 1;
	} else
		f = fopen(MATHLOGFILE,"at");
	if (!f) return;
	fprintf(f,"[%s] %s\n",fmttime,string);
	fclose(f);
	delete [] fmttime;
}