/*
 * File:		rotate3d.c
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		March 2001
 *
 *
 * CSIRO Mathematical and Information Sciences is the owner of all
 * copyright subsisting in the software contained in this file. It may
 * not be disclosed to, or used by, anyone without the prior approval 
 * of the Chief, CSIRO Mathematical and Information Sciences.
 *
*/

/*------------------------------------------------------------------------
 *
 *  This file contains the code for some fundamental 3d image manipulation
 *  routines: image rotation, image flipping, image shearing, etc.
 *
 *  John Lin	02 Dec 1998
 *
 *-----------------------------------------------------------------------*/

#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "liarp.h"
#include "mccodimage.h"
#include "rotate3d.h"
#include "rotate3d_generic.hpp"
#include "pinkconst.h"



/**
 * \brief This function adds a border to the 3D image.

	This function takes a 3d image as input, and returns a 3d image
	which contains a border surrounding the original image, border
	width is specified by the three offset and image sizes.

 * \param *in Input buffer
 * \param **out Output buffer
 * \param size Image pixel size
 * \param inx nb of cols (input)
 * \param iny nb of rows (input)
 * \param inz nb of slices (input)
 * \param offsetx offset value in x
 * \param offsety offset value in y
 * \param offsetz offset value in z

 * \author John Lin
 * <br> Converted some pointer to char* for correct pointer arithmetic
 * \date 08 Jan 1999
*/


// this function does not exist on Windows(tm)
double pink::rint( double x) 
// Copyright (C) 2001 Tor M. Aamodt, University of Toronto
// Permisssion to use for all purposes commercial and otherwise granted.
// THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY, OR ANY CONDITION OR
// OTHER TERM OF ANY KIND INCLUDING, WITHOUT LIMITATION, ANY WARRANTY
// OF MERCHANTABILITY, SATISFACTORY QUALITY, OR FITNESS FOR A PARTICULAR
// PURPOSE.
{
    if( x > 0 ) {
      int64_t xint = static_cast<int64_t>(x+0.5);
      if( xint % 2 ) {
	// then we might have an even number...
	double diff = x - static_cast<double>(xint);
	if( diff == -0.5 )
	  return double(xint-1);
      }
      return double(xint);
    } else {
      int64_t xint = static_cast<int64_t>(x-0.5);
      if( xint % 2 ) {
	// then we might have an even number...
	double diff = x - static_cast<double>(xint);
	if( diff == 0.5 )
	  return double(xint+1);
      }
      return double(xint);
    }
} // rint


	
		
 // Definitions		
	

/**
 * \brief calculates new coordinate for vector (x, y, z) under rotatioon (a, b, c)
*/
void rec2p(double x, double y, double *rho, double *phi)
{
    *rho = myhypot(x, y);
    *phi = atan2(y, x) * 180.0 / M_PI;
}

void p2rec(double rho, double phi, double *x, double *y)
{
    double arg = phi * M_PI / 180.0;
    *x = rho * cos(arg);
    *y = rho * sin(arg);
}

void rotvect_3d(double x, double y, double z, double a, double b, double c,
		double *newx, double *newy, double *newz)
{
    double temp[2][3];
    double rho, phi;

    rec2p(x, y, &rho, &phi);
    phi -= c;
    p2rec(rho, phi, &temp[0][0], &temp[0][1]);
    temp[0][2] = z;

    rec2p(temp[0][0], temp[0][2], &rho, &phi);
    phi -= b;
    p2rec(rho, phi, &temp[1][0], &temp[1][2]);
    temp[1][1] = temp[0][1];

    rec2p(temp[1][0], temp[1][1], &rho, &phi);
    phi -= a;
    p2rec(rho, phi, newx, newy);
    *newz = temp[1][2];
    LIARdebug("newx:%3.1f, newy:%3.1f, newz:%3.1f", *newx, *newy, *newz);
}

/**
 * \brief Gets the dimension of important bits of rotated image

	Image shearing can generate images with larger dimensions than
	the original image. This function determines the boundary of
	the sheared object.
    

 * \param mode different shear modes
 * \param origx nb of cols in the unrotated image
 * \param origy nb of rows in the unrotated image
 * \param origz nb of slices in the unrotated image
 * \param *outx nb of cols (output)
 * \param *outy nb of rows (output)
 * \param *outz nb of slices (output)
 * \param a shear factor in X direction
 * \param b shear factor in Y direction
 * \param c shear factor in Z direction

 * \author John Lin
 * \date 04 Dec 1998
*/
void sheardim(int mode,		/* different shear modes */
	      int origx,	/* nb of cols in the unrotated image */
	      int origy,	/* nb of rows in the unrotated image */
	      int origz,	/* nb of slices in the unrotated image */
	      int *outx,	/* nb of cols (output) */
	      int *outy,	/* nb of rows (output) */
	      int *outz,	/* nb of slices (output) */
	      double a,		/* shear factor in X direction */
	      double b,		/* shear factor in Y direction */
	      double c)		/* shear factor in Z direction */
{
    if (mode == 1) {	/* first shear */
	*outx = origx + top(fabs(a) * (origy - 1) + fabs(b) * (origz - 1));
	*outy = origy + top(fabs(c) * (origz - 1));
	*outz = origz;
    } else {		/* second shear */
	*outx = origx;
	*outy = origy + top(fabs(a) * (origx - 1));
	*outz = origz + top(fabs(b) * (origx - 1) + fabs(c) * (origy - 1));
    }

    LIARdebug("BoundBox: Left:0, Right:%d, Top:%d, Bottom:0, Up:%d, Down:0",
	       *outx, *outy, *outz);
    return;
}

double get_angle(double old_angle)
{
    double angle = old_angle;

    /* any angle as input */
    while (angle < 0)
	angle += 360.0;
    while (angle >= 360.0)
	angle -= 360.0;

    /* now angle is 0 <= angle < 360 */
    if (angle > 180.0)
	angle -= 360.0;

    /* now angle is -180 < angle <= 180 */
    return angle;
}

int neworig2d(int oldx,  /* old origin, from top-left corner */
	      int oldy, 
	      int oldnx, /* old image dimensions */
	      int oldny,
	      int newnx, /* new image dimensions */
	      int newny,
	      int *newx, /* new origin (same system) */
	      int *newy,
	      double a)  /* rotation angle in degrees */
{
    double rho1, phi1;
    double alpha;
    double psi1;
    double x2, y2;
    double deltax, deltay;
    int    retval = 0;

    

    /* we should stick to either degrees or radians, but not both! */
    alpha = (a * M_PI)/180.0;

    if ((oldx != 0) && (oldy != 0)) {
	rec2p(oldx, oldy, &rho1, &phi1); /* this works in degrees? */
	psi1 = phi1 - a; /* y is pointing down -> a := -a */
	p2rec(rho1, psi1, &x2, &y2); /* new rotated coordinate */
    } else {
	x2 = oldx;
	y2 = oldy;
    }

    /* compute where the top-left corner is now... */
    if ((a >= 0.0) && (a < 90.0)) {
	deltax = 0;
	deltay = oldnx * sin(alpha); 
    } else if ((a < 0.0) && (a > -90.0)) {
	deltay = 0;
	deltax = newnx - oldnx * cos(alpha); 
    } else {
	deltax = deltay = 0;
	retval = 1;
    }

    *newx = pink::rint(deltax + x2);
    *newy = pink::rint(deltay + y2);
	
    return retval;

}

//
// Explicit instanciations are necessary here
//
// Hugues Talbot	 3 May 2013

#define Type unsigned char
template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0); 
#undef Type
#define Type int
template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0); 

#undef Type
#define Type double
template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0); 
#undef Type
#define Type unsigned short
template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0); 
#undef Type
#define Type float
template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0); 

// error C3416: 'lrotate3d' : an explicit specialization may not be explicitly instantiated
// #undef Type
// #define Type fcomplex
// template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0);
// #undef Type
// #define Type dcomplex
// template int lrotate3d(Type *bufin, Type **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0);
    












