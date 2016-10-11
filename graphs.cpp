
#include "graphs.h"

// clean Bresenham line drawing algorithm

// !!! something is wrong here, because i notice, that function draws some
// lines not to end and make artefacts (black pixel in wire frame on joins)

void line(int x1,int y1,int x2,int y2,UCHAR clr)
{
	int xerr,yerr;
	int dx,dy,d;
	int x,y;
	short incX,incY;
	COLORREF clrr = RGB( clr , clr , clr );

    
	xerr = 0; yerr = 0;
	dx = x2 - x1; dy = y2 - y1;
	if (dx > 0) 
		incX = 1;
	else
		if (dx < 0)
			incX = -1;
		else
			incX = 0;
	if (dy > 0) 
		incY = 1;
	else
		if (dy < 0)
			incY = -1;
		else
			incY = 0;	
	dx = abs(dx); //absolute
	dy = abs(dy);
	if (dx > dy)
		d = dx;
	else
		d = dy;
	x = x1; y = y1;
	SetPixel(hdc,x,y,clrr);
	for (int count = 1; count <= d; count++)
	{
		xerr = xerr + dx;
		yerr = yerr + dy;
		if (xerr > d)
		{
			xerr = xerr - d;
			x = x + incX;
		}
		if (yerr > d)
		{
			yerr = yerr - d;
			y = y + incY;
		}
	SetPixel(hdc,x,y,clrr);
	}
}

// Line drawing algorithm by (c) Andre Lamothe ( modification of Bresenham algorithm )
// modified by me for adaptation

void Draw_Line(int x0, int y0, // starting position 
              int x1, int y1, // ending position
              UCHAR color) // color index
{
// this function draws a line from xo,yo to x1,y1 using differential error
// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2, 
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

	int x = x0, y = y0;
	COLORREF clr = RGB(color , color , color);

// compute horizontal and vertical deltas
	dx = x1-x0;
	dy = y1-y0;

// test which direction the line is going in i.e. slope angle
	if (dx>=0)
	{
		x_inc = 1;
	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx = -dx;  // need absolute value
	} // end else moving left

// test y component of slope

	if (dy>=0)
	{
		y_inc = 1;
	} // end if line is moving down
	else
	{
		y_inc = -1;
		dy = -dy;  // need absolute value
	} // end else moving up


// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx; 

   // draw the line
   for (index=0; index <= dx; index++)
       {

	   SetPixel(hdc,x,y,clr);

       // test if error has overflowed
       if (error >= 0) 
          {
          error-=dx2;

          // move to next line
          y+=y_inc;

	   } // end if error overflowed

       // adjust the error term
       error+=dy2;

       // move to the next pixel
       x+=x_inc;

       } // end for

   } // end if |slope| <= 1
else
   {
   // initialize error term
   error = dx2 - dy; 

   // draw the line
   for (index=0; index <= dy; index++)
       {
       // set the pixel
	   SetPixel(hdc,x,y,clr);
       // test if error overflowed
       if (error >= 0)
          {
          error-=dy2;

          // move to next line
          x+=x_inc;

          } // end if error overflowed

       // adjust the error term
       error+=dx2;

       // move to the next pixel
       y+=y_inc;

       } // end for

   } // end else |slope| > 1

} // end Draw_Line

void clearscreen()
{
	PatBlt(hdc,0,0,screen_width,screen_height,BLACKNESS);
}
