/*
 * File:		rotate3d_generic.h
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
 
#ifndef ROTATE3D_HPP
#define ROTATE3D_HPP

#include <stdlib.h>
#include <algorithm>
#include <cmath>

#include "pinkconst.h"

#define XFLIP_IN_USE 1
#define ORTHOROT_IN_USE 1

/* Simple rotations */


/**
 * \brief This function rotates the image of +90 degree about x axis

    Simple rotation of +90 degrees about x axis

	TESTS: Works in place.

 * \author John Lin
 * \date 03 Dec 1998
*/

// double rint( double x)
// // Copyright (C) 2001 Tor M. Aamodt, University of Toronto
// // Permisssion to use for all purposes commercial and otherwise granted.
// // THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY, OR ANY CONDITION OR
// // OTHER TERM OF ANY KIND INCLUDING, WITHOUT LIMITATION, ANY WARRANTY
// // OF MERCHANTABILITY, SATISFACTORY QUALITY, OR FITNESS FOR A PARTICULAR
// // PURPOSE.
// {
//     if( x > 0 ) {
//         __int64 xint = (__int64) (x+0.5);
//         if( xint % 2 ) {
//             // then we might have an even number...
//             double diff = x - (double)xint;
//             if( diff == -0.5 )
//                 return double(xint-1);
//         }
//         return double(xint);
//     } else {
//         __int64 xint = (__int64) (x-0.5);
//         if( xint % 2 ) {
//             // then we might have an even number...
//             double diff = x - (double)xint;
//             if( diff == 0.5 )
//                 return double(xint+1);
//         }
//         return double(xint);
//     }
// }

template <typename Type>
int xrotp90_3d (Type *bufin,	    /* Input buffer  */
  	Type *bufout,    /* Output buffer */
   	int nx,			 /* nb of cols (input image) */
   	int ny,		    /* nb of rows (input image) */
   	int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * nz) * y + (nz-z-1) * nx;
		for (x = 0 ; x < nx ; x++) {
		    *q++ = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Xrotp90_3d: insufficient memory");
	res = 1;
    }
    return res;
}


/**
 * \brief This function rotates the image of -90 degree about x axis

    Simple rotation of -90 degrees about x axis

	TESTS: Works in place also.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
static int xrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (ny-y-1) * (nx * nz) + nx * z;
		for (x = 0 ; x < nx ; x++) {
		    *q++ = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Xrotm90_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of 180 degree about x axis

    Simple rotation of 180 degrees about x axis

	TESTS: Works in place also.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
static int xrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nz-z-1) * (nx * ny) + (ny-y-1) * nx;
		for (x = 0 ; x < nx ; x++) {
		    *q++ = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Xrot180_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of +90 degree about y axis

    Simple rotation of +90 degrees about y axis

	TESTS: Works in place.

 * \author John Lin
 * \date 02 Dec 1998
*/

template <typename Type>
int yrotp90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nz-z) - 1 + nz * y;
		for (x = 0 ; x < nx ; x++) {
		    *q = *p++;
		    q += ny * nz;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Yrotp90_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of -90 degree about y axis

    Simple rotation of -90 degrees about y axis

	TESTS: Works in place also.

 * \author John Lin
 * \date 02 Dec 1998
*/

template <typename Type>
static int yrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + z + (nx-1) * ny * nz + nz * y;
		for (x = 0 ; x < nx ; x++) {
		    *q = *p++;
		    q -= ny * nz;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Yrotm90_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of 180 degree about y axis

    Simple rotation of 180 degrees about y axis

    TESTS: Works in place also.

 * \author John Lin
 * \date 02 Dec 1998
*/

template <typename Type>
static int yrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nz-z-1) * (nx * ny) + nx * (y+1) - 1;
		for (x = 0 ; x < nx ; x++) {
		    *q-- = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Yrot180_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of +90 degree about z axis

    Simple rotation of +90 degrees about z axis

    TESTS: Works in place.

 * \author John Lin
 * \date 02 Dec 1998
*/

template <typename Type>
int zrotp90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * ny) * z + (nx-1) * ny + y;
		for (x = 0 ; x < nx ; x++) {
		    *q = *p++;
		    q -= ny;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Zrotp90_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of -90 degree about z axis

    Simple rotation of -90 degrees about z axis

    TESTS: Works in place.

 * \author John Lin
 * \date 02 Dec 1998
*/
template <typename Type>
int zrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * ny) * z + ny-y-1;
		for (x = 0 ; x < nx ; x++) {
		    *q = *p++;
		    q += ny;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Zrotm90_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function rotates the image of 180 degree about z axis

    Simple rotation of 180 degrees about z axis

    TESTS: Works in place.

 * \author John Lin
 * \date 02 Dec 1998
*/

template <typename Type>
int zrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz)		    /* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	p = bufin;
	for (z = 0 ; z < nz ; z++) {
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * ny) * z + nx * (ny-y) - 1;
		for (x = 0 ; x < nx ; x++) {
		    *q-- = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Zrot180_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function flips the image about the x axis

    Simple flip of the 3d image abount the y-z plane going
	through the center of the image.

    TESTS: Works in place.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
int xflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz)		/* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *)malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	for (z = 0 ; z < nz ; z++) {
	    p = bufin + (nx * ny) * z;
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * ny) * z + nx * (y+1) - 1;
		for (x = 0; x < nx ; x++) {
		    *q-- = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Xflip_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function flips the image about the y axis

    Simple flip of the 3d image abount the x-z plane going
	through the center of the image.

    TESTS: Works in place.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
int yflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz)		/* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	for (z = 0 ; z < nz ; z++) {
	    p = bufin + (nx * ny) * z;
	    for (y = 0 ; y < ny ; y++) {
		q = tempBuff + (nx * ny) * z + nx * (ny-y-1);
		for (x = 0; x < nx ; x++) {
		    *q++ = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Yflip_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function flips the image about the z axis

    Simple flip of the 3d image abount the x-y plane going
	through the center of the image.

    TESTS: Works in place.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
int zflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz)		/* nb of slices (input image) */
{
    int x, y, z, res = 0;
    Type *tempBuff, *p, *q;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    if (tempBuff != NULL) {
	for (z = 0 ; z < nz ; z++) {
	    p = bufin + (nx * ny) * z;
	    q = tempBuff + (nx * ny) * (nz-z-1);
	    for (y = 0 ; y < ny ; y++) {
		for (x = 0; x < nx ; x++) {
		    *q++ = *p++;
		}
	    }
	}
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Zflip_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function flips the image about the axis of symmetry

    Fast flip of a 3d image abount the axis of symmetry

    TESTS: Works in place.

 * \author John Lin
 * \date 03 Dec 1998
*/

template <typename Type>
int pointsymm_3d
  (Type *bufin,	 /* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz)		/* nb of slices (input image) */
{
    int num, tmp, res = 0;
    Type *tempBuff, *p, *q, *r, *s;

    tempBuff = (Type *) malloc(nx * ny * nz * sizeof(Type));
    num = (nx * ny * nz) / 2;
    p = q = r = s = NULL;

    p = bufin;
    q = bufin + nx * ny * nz - 1;
    r = tempBuff;
    s = tempBuff + nx * ny * nz - 1;

    if (tempBuff != NULL) {
	for (tmp = 0 ; tmp < num ; tmp++) {
	    *r++ = *q--;
	    *s-- = *p++;
	}
	if ((nx * ny * nz) > (num * 2)) /* odd entries, one left */
	    *r = *p;
	/* copy the stuff back where it belongs */
	memcpy(bufout, tempBuff, nx * ny * nz * sizeof(Type));
	free(tempBuff);
    } else {
	LIARerror("Pointsymm_3d: insufficient memory");
	res = 1;
    }
    return res;
}

/**
 * \brief This function trims the border around a 3D image.

	This function takes a 3d image as input, and returns a 3d image
	which is a subset of the original image, with borders removed

    TESTS: Works in place.

 * \author John Lin
 * \date 11 Dec 1998
*/

template <typename Type>
static int trimborder_3d
  (Type *in,	    /* Input buffer  */
   Type **out,   /* Output buffer */
   int dx,	    /* nb of border cols */
   int dy,	    /* nb of border rows */
   int dz,	    /* nb of border slices */
   int inx,	    /* nb of cols (input) */
   int iny,	    /* nb of rows (input) */
   int inz,	    /* nb of slices (input) */
   int *outx,	    /* nb of cols (output) */
   int *outy,	    /* nb of rows (output) */
   int *outz)	    /* nb of slices (output) */
{
    *outx = inx - 2 * dx;
    *outy = iny - 2 * dy;
    *outz = inz - 2 * dz;

    if ((*outx) > 0 && (*outy) > 0 && (*outz) > 0) {
	*out = (Type *) calloc((*outx)*(*outy)*(*outz), sizeof(Type));
	getsubset_3d((void *)in, (void **)out, sizeof(Type), inx, iny, inz, outx, outy, outz, dx, dy, dz);
	return 0;
    } else
	return 1;
}

/**
 * \brief This function removes useless border at 0 around images.

	This function takes any 3d image as input, finds if it has a border set
	at zero around the interesting bits of the image, and if so, removes
	it. This is useful to remove useless border left after rotating images.

 * \author John Lin
 * \date 09 Dec 1998
*/

template <typename Type>
static int rmborder_3d
  (Type *in,   /* Input buffer  */
   Type **out, /* Output buffer */
   int inx,	/* nb of cols (input) */
   int iny,	/* nb of rows (input) */
   int inz,	/* nb of slices (input) */
   int *outx,	/* nb of cols (output) */
   int *outy,	/* nb of rows (output) */
   int *outz,	/* nb of slices (output) */
   int bgval)	/* value of the background */
{
    int found, set, final, tt, tb, tl, tr;
    int x, y, z, top, bottom, left, right, front, back;
    Type *p;

    top = iny - 1;
    left = inx - 1;
    front = inz - 1;
    bottom = 0;
    right = 0;
    back = 0;
    set = 0;
    final = 0;

    for (z = 0, p = in ; z < inz ; z++) {
	tt = iny - 1;
	tl = inx - 1;
	found = tb = tr = 0;

	for (y = 0 ; y < iny ; y++) {
	    for (x = 0 ; x < inx ; x++, p++) {
		if (*p != bgval) {
		    if (!found) found = 1;
		    if (y > tb)
			tb = y;
		    if (y < tt)
			tt = y;
		    if (x > tr)
			tr = x;
		    if (x < tl)
			tl = x;
		}
	    }
	}

	if (found) {
	    if (!set) {
		front = z;
		set = 1;
	    }

	    if (tt < top)
		top = tt;
	    if (tb > bottom)
		bottom = tb;
	    if (tl < left)
		left = tl;
	    if (tr > right)
		right = tr;
	} else if (set && !final) {
		back = z - 1;
		final = 1;
	}
    }

    if (back == 0)
	back = inz - 1;

    LIARdebug("Input: %d x %d x %d", inx, iny, inz);
    LIARdebug("Top:%d, bottom:%d, left:%d, right:%d, front:%d, back:%d",
	      top, bottom, left, right, front, back);

    *outx = right - left + 1;
    *outy = bottom - top + 1;
    *outz = back - front + 1;

    if ((*out = (Type *)malloc((*outx)*(*outy)*(*outz)*sizeof(Type))) == NULL)
	return(1);
    else {
	getsubset_3d((void *)in, (void **)out, sizeof(Type), inx, iny, inz, outx, outy, outz, left, top, front);
	return(0);
    }
}

/**
 * \brief Gets the inportant bits of rotated image

	Image rotation can generate huge useless border around the important data.
	This functions throws these borders out. It calculates where the 8 corners of
	the image are and deduces the bounding box from there.

 * \author John Lin
 * \date 09 Dec 1998
*/

template <typename Type>
static int pullrotated_3d
  (Type *in,	/* Input buffer  */
   Type **out,	/* Output buffer */
   int inx,		/* nb of cols (input) */
   int iny,		/* nb of rows (input) */
   int inz,		/* nb of slices (input) */
   int origx,		/* nb of cols in the original image */
   int origy,		/* nb of rows in the original image */
   int origz,		/* nb of slices in original image */
   int *outx,		/* nb of cols (output) */
   int *outy,		/* nb of rows (output) */
   int *outz,		/* nb of slices (output) */
   double alpha,	/* shear factor in radians */
   double beta,		/* shear factor in radians */
   double gamma)	/* shear factor in radians */
{
    int i;
    double xcorner[8], ycorner[8], zcorner[8];
    double rotatedx, rotatedy, rotatedz;
    double centerx, centery, centerz;
    double origcenterx, origcentery, origcenterz;
    double leftx, rightx, topy, bottomy, frontz, backz;
    int borderx, bordery, borderz, res;

    origcenterx = (origx == 1) ? 0 : (double)origx/2;
    origcentery = (origy == 1) ? 0 : (double)origy/2;
    origcenterz = (origz == 1) ? 0 : (double)origz/2;

    xcorner[0] = xcorner[2] = xcorner[4] = xcorner[6] = -origcenterx;
    xcorner[1] = xcorner[3] = xcorner[5] = xcorner[7] = origcenterx;
    ycorner[0] = ycorner[1] = ycorner[4] = ycorner[5] = -origcentery;
    ycorner[2] = ycorner[3] = ycorner[6] = ycorner[7] = origcentery;
    zcorner[0] = zcorner[1] = zcorner[2] = zcorner[3] = -origcenterz;
    zcorner[4] = zcorner[5] = zcorner[6] = zcorner[7] = origcenterz;

    leftx = rightx = topy = bottomy = frontz = backz = 0.0;
    rotatedx = rotatedy = rotatedz = 0.0;    for (i = 0 ; i < 8 ; i++) {
	rotvect_3d(xcorner[i], ycorner[i], zcorner[i], alpha, beta, gamma,
		&rotatedx, &rotatedy, &rotatedz);
	if (rotatedx > rightx)
	    rightx = rotatedx;
	if (rotatedx < leftx)
	    leftx = rotatedx;
	if (rotatedy > bottomy)
	    bottomy = rotatedy;
	if (rotatedy < topy)
	    topy = rotatedy;
	if (rotatedz > backz)
	    backz = rotatedz;
	if (rotatedz < frontz)
	    frontz = rotatedz;
    }

    centerx = (double)inx/2;
    centery = (double)iny/2;
    centerz = (double)inz/2;

    borderx = std::max(bottom(centerx - rightx), 0);
    bordery = std::max(bottom(centery - bottomy), 0);
    borderz = std::max(bottom(centerz - backz), 0);

    LIARdebug("Original: %d x %d x %d", origx, origy, origz);
    LIARdebug("BoundingBox: Left:%3.1f, Top:%3.1f, Front:%3.1f", leftx, topy, frontz);
    LIARdebug("Xborder: %d, Yborder:%d, Zborder:%d", borderx, bordery, borderz);

    res = trimborder_3d(in, out, borderx, bordery, borderz,\
			inx, iny, inz, outx, outy, outz);
    if (res == 0)
	LIARdebug("Final image is %d x %d x %d", *outx, *outy, *outz);

    return res;
}

template <typename Type>
static int pullcenteredsubset_3d
  (Type *in,	/* Input buffer  */
   Type **out,	/* Output buffer */
   int inx,		/* nb of cols (input) */
   int iny,		/* nb of rows (input) */
   int inz,		/* nb of slices (input) */
   int origx,		/* nb of cols in the original image */
   int origy,		/* nb of rows in the original image */
   int origz,		/* nb of slices in original image */
   int row0,            /* rotation center (row) */
   int col0,            /* rotation center (col) */
   int slice0,          /* rotation center (slice) */
   double a,
   double b,
   double c)
{
    int outx, outy, outz;
    int offx, offy, offz;
    int dx, dy, dz, retval;
    double newx, newy, newz;
    double rho, sigma, phi, psi, alpha; /* only for 2D */

    retval = 0;
    
    outx = origx;
    outy = origy;
    outz = origz;

    if (col0 != -1)
	dx = col0 - (origx/2);
    else
	dx = 0;

    if (row0 != -1)
	dy = row0 - (origy/2);
    else
	dy = 0;

    if (slice0 != -1)
	dz = slice0 - (origz/2);
    else
	dz = 0;

/*     rotvect_3d(dx, dy, dz, a, b, c, */
/* 	       &newx, &newy, &newz); */

    /* we should stick to either degrees or radian, but not both! */
    alpha = (a * M_PI)/180.0;
    
    rec2p(dx, dy, &rho, &phi);
    psi = phi + (180.0 - a)/2;
    sigma = 2 * rho * sin(alpha/2);
    p2rec(sigma, psi, &newx, &newy);
    newz = 0;

    
    offx = (inx - origx)/2 + newx;
    offy = (iny - origy)/2 + newy;
    offz = (inz - origz)/2 + newz;
    
    *out = (Type *)malloc(origx*origy*origz*sizeof(Type));

    if (*out == NULL)
	retval = 1;
    else 
	getsubset_3d(in, (void **)out,
		     sizeof(Type),
		     inx, iny, inz,
		     &outx, &outy, &outz,
		     offx, offy, offz);
    
    return retval;
}

/**
 * \brief This function realizes a shear in 3D

	This function makes a shear in the direction with
	given factor. This can be used in fast rotation
	algorithms. The output image dimension is generally
	altered. Unaligned data is interpolated, the rest of
	the image is filled with the given value.

	REFERENCES: Three Dimentional Rotation by Three Shears.

	KEYWORDS: 3D Shear, fast rotation

 * \author John Lin
 * \date 07 Dec 1998
*/

template <typename Type>
static int shear1_itp
  (Type *imin,		/* input buffer  */
   Type **imout,	/* output buffer */
   int nxin,		/* nb of cols in the input */
   int nyin,		/* nb of rows in the input */
   int nzin,		/* nb of slices in the input */
   int *nxout,		/* nb of cols in the output */
   int *nyout,		/* nb of rows in the output */
   int *nzout,		/* nb of slices in the output */
   double a1,		/* shear factor in X direction */
   double b1,		/* shear factor in Y direction */
   double c1,		/* shear factor in Z direction */
   int fillval)		/* fill the image with this value */

{
    int x, y, z, count; /* Pixel counter */
    int startx, startx1, starty;
    double tmp1, tmp2, tmpA, tmpB, tmpC, tmpD, a, b, c;
    double err1, err2, err3, area1, area2, area3, area4;
    Type *p, *q, *r;

    /* invert the angles (images have their origin on top...) */
    a = -a1;
    b = -b1;
    c = -c1;

    /* calculate dimension of the bounding box after shearing */
    sheardim(1, nxin, nyin, nzin, nxout, nyout, nzout, a, b, c);
    LIARdebug("newX = %d, newY = %d, newZ = %d", *nxout, *nyout, *nzout);

    /* allocate the ouput buffer */
    (*imout) = (Type *)malloc((*nxout)*(*nyout)*(*nzout)*sizeof(Type));
    if (*imout == NULL) {
	LIARerror("Shear1_itp: insufficient memory");
	return 1;
    }
    memset((*imout), fillval, (*nxout)*(*nyout)*(*nzout)*sizeof(Type));

    /* calculate the offset of the rotated image to the original coordinate */
    tmpA = a * (nyin - 1);
    tmpB = b * (nzin - 1);
    tmpC = tmpA + tmpB;
    tmpD = c * (nzin - 1);

    /* get x offset */
    if (tmpA < 0 || tmpB < 0 || tmpC < 0) {
	if (tmpA < tmpB)
	    tmp1 = tmpA;
	else tmp1 = tmpB;
	if (tmpC < tmp1)
	    tmp1 = tmpC;
    } else
	tmp1 = 0;

    /* get y offset */
    tmp2 = (tmpD < 0) ? tmpD : 0;

    for (z = 0 ; z < nzin ; z++) {
	/* first row */
	p = imin + (nxin * nyin) * z;
	q = (*imout) + ((*nxout) * (*nyout)) * z;

	startx = std::max(bottom(b * z - tmp1), 0);
	err1 = (b * z - tmp1) - startx;
	starty = std::max(bottom(c * z - tmp2), 0);
	err3 = (c * z - tmp2) - starty;

	q += (*nxout)*starty + startx;	/* starting position in output image */
	area3 = err1 * (1 - err3);	/* interpolation error calculation */
	area4 = (1 - err1) * (1 - err3);

	*q++ = pink::rint((1-area4)*fillval + (*p)*area4);	/* first point */
	for (x = 1 ; x < nxin ; x++, p++)
	    *q++ = pink::rint((1-area3-area4)*fillval + (*p)*area3 + *(p+1)*area4);
	if (area3 > 0.0)
	    *q = pink::rint((1-area3)*fillval + (*p)*area3); /* last point */

	for (y = 1 ; y < nyin ; y++) {
	    p = imin + (nxin * nyin) * z + nxin * y;
	    r = p - nxin;
	    q = (*imout) + ((*nxout) * (*nyout)) * z;

	    startx = std::max(bottom(a * y + b * z - tmp1), 0);
	    err1 = (a * y + b * z - tmp1) - startx;
	    startx1 = std::max(bottom(a * (y-1) + b * z - tmp1), 0);
	    err2 = (a * (y-1) + b * z - tmp1) - startx1;
	    starty = std::max(bottom(y + c * z - tmp2), 0);
	    err3 = (y + c * z - tmp2) - starty;

	    q += (*nxout) * starty + startx;
	    area1 = err2 * err3;
	    area2 = (1 - err2) * err3;
	    area3 = err1 * (1 - err3);
	    area4 = (1 - err1) * (1 - err3);

	    /* pixel shift error in the row offsets taken into account */
	    if (startx == startx1) {
		count = nxin - 1;
		*q++ = pink::rint((area1+area3)*fillval + (*p)*area4 + (*r)*area2);
	    } else if ((startx1 - startx) == 1) {
		count = nxin - 2;
		*q++ = pink::rint((1-area4)*fillval + (*p)*area4);
		*q++ = pink::rint(area1*fillval + (*p)*area3 + *(p+1)*area4 + (*r)*area2);
		p++;
	    } else {
		count = nxin - 2;
		*(q-1) = pink::rint((1-area2)*fillval + (*r)*area2);
		*q++ = pink::rint(area3*fillval + (*p)*area4 + (*r)*area1 + *(r+1)*area2);
		r++;
	    }

	    for (x = 1 ; x <= count ; x++, p++, r++)
		*q++ = pink::rint((*p)*area3 + *(p+1)*area4 + (*r)*area1 + *(r+1)*area2);
	    if (area3 > 0.0 || area1 > 0.0) {
	    	if (startx == startx1)
		    *q = pink::rint((area2+area4)*fillval + (*p)*area3 + (*r)*area1);
		else if ((startx1 - startx) == 1) {
		    if (nxin > 1)
			*q++ = pink::rint(area4*fillval + (*p)*area3 + (*r)*area1 + *(r+1)*area2);
		    *q = pink::rint((1-area1)*fillval + *(r+1)*area1);
		}
		else {
		    if (nxin > 1)
			*q++ = pink::rint(area2*fillval + (*p)*area3 + *(p+1)*area4 + (*r)*area1);
		    *q = pink::rint((1-area3)*fillval + *(p+1)*area3);
		}
	    }
	}

	/* last row */
	p = imin + (nxin * nyin) * z + nxin * (nyin - 1);
	q = (*imout) + ((*nxout)*(*nyout))*z + (*nxout)*(starty+1) + startx;
	area1 = err1 * err3;
	area2 = (1 - err1) * err3;

	if (err3 > 0.0) { /* extra row required */
	    *q++ = pink::rint((1-area2)*fillval + (*p)*area2);
	    for (x = 1 ; x < nxin ; x++, p++)
		*q++ = pink::rint((1-area1-area2)*fillval + (*p)*area1 + *(p+1)*area2);
	    if (area1 > 0.0)
		*q = pink::rint((1-area1)*fillval + (*p)*area1);
	}
    }
    return 0;
}

/**
 * \brief This function realizes a shear in 3D

	This function makes a shear in the direction with
	given factor. This can be used in fast rotation
	algorithms. The output image dimension is generally
	altered. The rest of the image is filled with the given value.

	REFERENCES: Three Dimentional Rotation by Three Shears.

	KEYWORDS: 3D Shear, fast rotation

 * \author John Lin
 * \date 07 Dec 1998
*/

template <typename Type>
static int shear1_nn
  (Type *imin,		/* input buffer  */
   Type **imout,		/* output buffer */
   int nxin,			/* nb of cols in the input */
   int nyin,			/* nb of rows in the input */
   int nzin,			/* nb of slices in the input */
   int *nxout,		/* nb of cols in the output */
   int *nyout,		/* nb of rows in the output */
   int *nzout,		/* nb of slices in the output */
   double a1,		/* shear factor in X direction */
   double b1,		/* shear factor in Y direction */
   double c1,		/* shear factor in Z direction */
   int fillval)		/* fill the image with this value */
{
    int x, y, z; /* Pixel counter */
    int startx, starty;
    double tmp1, tmp2, tmpA, tmpB, tmpC, tmpD, a, b, c;
    Type *p, *q;

    /* invert the angles (images have their origin on top...) */
    a = -a1;
    b = -b1;
    c = -c1;

    /* calculate dimension of the bounding box after shearing */
    sheardim(1, nxin, nyin, nzin, nxout, nyout, nzout, a, b, c);
    LIARdebug("newX = %d, newY = %d, newZ = %d", *nxout, *nyout, *nzout);

    /* allocate the ouput buffer */
    (*imout) = (Type *)malloc((*nxout)*(*nyout)*(*nzout)*sizeof(Type));
    if (*imout == NULL) {
	LIARerror("Shear1_nn: insufficient memory");
	return 1;
    }
    memset((*imout), fillval, (*nxout)*(*nyout)*(*nzout)*sizeof(Type));

    tmpA = a * (nyin - 1);
    tmpB = b * (nzin - 1);
    tmpC = tmpA + tmpB;
    tmpD = c * (nzin - 1);

    /* get x offset */
    if (tmpA < 0 || tmpB < 0 || tmpC < 0) {
	if (tmpA < tmpB)
	    tmp1 = tmpA;
	else tmp1 = tmpB;
	if (tmpC < tmp1)
	    tmp1 = tmpC;
    } else
	tmp1 = 0;

    /* get y offset */
    tmp2 = (tmpD < 0) ? tmpD : 0;

    for (z = 0 ; z < nzin ; z++) {
	for (y = 0 ; y < nyin ; y++) {
	    p = imin + (nxin * nyin) * z + nxin * y;
	    q = (*imout) + ((*nxout) * (*nyout)) * z;

	    startx = std::max(bottom(a * y + b * z - tmp1), 0);
	    starty = std::max(bottom(y + c * z - tmp2), 0);

	    q += (*nxout) * starty + startx;

	    for (x = 0 ; x < nxin ; x++) {
		*q++ = *p++;
	    }
	}
    }
    return 0;
}

/**
 * \brief This function realizes a shear in 3D

	This function makes a shear in the direction with
	given factor. This can be used in fast rotation
	algorithms. The output image dimension is generally
	altered. Unaligned data is interpolated, the rest of
	the image is filled with the given value.

	REFERENCES: Three Dimentional Rotation by Three Shears.

	KEYWORDS: 3D Shear, fast rotation

 * \author John Lin
 * \date 07 Dec 1998
*/

template <typename Type>
static int shear2_itp
  (Type *imin,		/* input buffer  */
   Type **imout,	/* output buffer */
   int nxin,		/* nb of cols in the input */
   int nyin,		/* nb of rows in the input */
   int nzin,		/* nb of slices in the input */
   int *nxout,		/* nb of cols in the output */
   int *nyout,		/* nb of rows in the output */
   int *nzout,		/* nb of slices in the output */
   double a1,		/* shear factor in X direction */
   double b1,		/* shear factor in Y direction */
   double c1,		/* shear factor in Z direction */
   int fillval)		/* fill the image with this value */

{
    int x, y, z, count; /* Pixel counter */
    int startz, startz1, starty;
    double tmp1, tmp2, tmpA, tmpB, tmpC, tmpD, a, b, c;
    double err1, err2, err3, area1, area2, area3, area4;
    Type *p, *q, *r;

    /* invert the angles (images have their origin on top...) */
    a = -a1;
    b = -b1;
    c = -c1;

    /* calculate dimension of the bounding box after shearing */
    sheardim(2, nxin, nyin, nzin, nxout, nyout, nzout, a, b, c);
    LIARdebug("newX = %d, newY = %d, newZ = %d", *nxout, *nyout, *nzout);

    /* allocate the ouput buffer */
    (*imout) = (Type *)malloc((*nxout)*(*nyout)*(*nzout)*sizeof(Type));
    if (*imout == NULL) {
	LIARerror("Shear2_itp: insufficient memory");
	return 1;
    }
    memset((*imout), fillval, (*nxout)*(*nyout)*(*nzout)*sizeof(Type));

    tmpA = b * (nxin - 1);
    tmpB = c * (nyin - 1);
    tmpC = tmpA + tmpB;
    tmpD = a * (nxin - 1);

    /* get z offset */
    if (tmpA < 0 || tmpB < 0 || tmpC < 0) {
	if (tmpA < tmpB)
	    tmp1 = tmpA;
	else tmp1 = tmpB;
	if (tmpC < tmp1)
	    tmp1 = tmpC;
    } else
	tmp1 = 0;

    /* get y offset */
    tmp2 = (tmpD < 0) ? tmpD : 0;

    for (x = 0 ; x < nxin ; x++) {
	/* first row */
	p = imin + x;
	q = (*imout) + x;

	startz = std::max(bottom(b * x - tmp1), 0);
	err1 = (b * x - tmp1) - startz;
	starty = std::max(bottom(a * x - tmp2), 0);
	err3 = (a * x - tmp2) - starty;

	q += ((*nxout) * (*nyout)) * startz + (*nxout) * starty;
	area3 = err1 * (1 - err3);
	area4 = (1 - err1) * (1 - err3);

	*q = pink::rint((1-area4)*fillval + (*p)*area4);	/* first point */
	q += (*nxout) * (*nyout);
	for (z = 1 ; z < nzin ; z++) {
	    *q = pink::rint((1-area3-area4)*fillval + (*p)*area3 + *(p+nxin*nyin)*area4);
	    q += (*nxout) * (*nyout);
	    p += nxin * nyin;
	}
	if (area3 > 0.0)
	    *q = pink::rint((1-area3)*fillval + (*p)*area3); /* last point */

	for (y = 1 ; y < nyin ; y++) {
	    p = imin + x + nxin * y;
	    r = p - nxin;
	    q = (*imout) + x;

	    startz = std::max(bottom(b * x + c * y - tmp1), 0);
	    err1 = (b * x + c * y - tmp1) - startz;
	    startz1 = std::max(bottom(b * x + c * (y-1) - tmp1), 0);
	    err2 = (b * x + c * (y-1) - tmp1) - startz1;
	    starty = std::max(bottom(y + a * x - tmp2), 0);
	    err3 = (y + a * x - tmp2) - starty;

	    q += ((*nxout) * (*nyout)) * startz + (*nxout) * starty;
	    area1 = err2 * err3;
	    area2 = (1 - err2) * err3;
	    area3 = err1 * (1 - err3);
	    area4 = (1 - err1) * (1 - err3);

	    /* pixel shift error in the row offsets taken into account */
	    if (startz == startz1) {
		count = nzin - 1;
		*q = pink::rint((area1+area3)*fillval + (*p)*area4 + (*r)*area2);
		q += (*nxout) * (*nyout);
	    } else if ((startz1 - startz) == 1) {
		count = nzin - 2;
		*q = pink::rint((1-area4)*fillval + (*p)*area4);
		q += (*nxout) * (*nyout);
		*q = pink::rint(area1*fillval + (*p)*area3 + *(p+nxin*nyin)*area4 + (*r)*area2);
		q += (*nxout) * (*nyout);
		p += nxin * nyin;
	    } else {
		count = nzin - 2;
		*(q-(*nxout)*(*nyout)) = pink::rint((1-area2)*fillval + (*r)*area2);
		*q = pink::rint(area3*fillval + (*p)*area4 + (*r)*area1 + *(r+nxin*nyin)*area2);
		q += (*nxout) * (*nyout);
		r += nxin * nyin;
	    }

	    for (z = 1 ; z <= count ; z++) {
		*q = pink::rint((*p)*area3 + *(p+nxin*nyin)*area4 + (*r)*area1 + *(r+nxin*nyin)*area2);
		q += (*nxout) * (*nyout);
		p += nxin * nyin;
		r += nxin * nyin;
	    }
	    if (area3 > 0.0 || area1 > 0.0) {
	    	if (startz == startz1)
		    *q = pink::rint((area2+area4)*fillval + (*p)*area3 + (*r)*area1);
		else if ((startz1 - startz) == 1) {
		    if (nzin > 1) {
			*q = pink::rint(area4*fillval + (*p)*area3 + (*r)*area1 + *(r+nxin*nyin)*area2);
			q += (*nxout) * (*nyout);
		    }
		    *q = pink::rint((1-area1)*fillval + *(r+nxin*nyin)*area1);
		}
		else {
		    if (nzin > 1) {
			*q = pink::rint(area2*fillval + (*p)*area3 + *(p+nxin*nyin)*area4 + (*r)*area1);
			q += (*nxout) * (*nyout);
		    }
		    *q = pink::rint((1-area3)*fillval + *(p+nxin*nyin)*area3);
		}
	    }
	}

	if (err3 > 0) {	/* more rows are required */
	    p = imin + x + nxin*(nyin-1);
	    q = (*imout) + x + ((*nxout)*(*nyout))*startz + (*nxout)*(starty+1);
	    area1 = err1 * err3;
	    area2 = (1 - err1) * err3;

	    *q = pink::rint((1-area2)*fillval + (*p)*area2);
	    q += (*nxout) * (*nyout);
	    for (z = 1 ; z < nzin ; z++) {
		*q = pink::rint((1-area1-area2)*fillval + (*p)*area1 + *(p+nxin*nyin)*area2);
		q += (*nxout) * (*nyout);
		p += nxin * nyin;
	    }
	    if (area1 > 0.0)
		*q = pink::rint((1-area1)*fillval + (*p)*area1);
	}
    }
    return 0;
}

/**
 * \brief This function realizes a shear in 3D

	This function makes a shear in the direction with
	given factor. This can be used in fast rotation
	algorithms. The output image dimension is generally
	altered. The rest of the image is filled with the given value.

	REFERENCES: Three Dimentional Rotation by Three Shears.

	KEYWORDS: 3D Shear, fast rotation
	
 * \author John Lin
 * \date 07 Dec 1998
*/

template <typename Type>
static int shear2_nn
  (Type *imin,		/* input buffer  */
   Type **imout,		/* output buffer */
   int nxin,			/* nb of cols in the input */
   int nyin,			/* nb of rows in the input */
   int nzin,			/* nb of slices in the input */
   int *nxout,		/* nb of cols in the output */
   int *nyout,		/* nb of rows in the output */
   int *nzout,		/* nb of slices in the output */
   double a1,		/* shear factor in X direction */
   double b1,		/* shear factor in Y direction */
   double c1,		/* shear factor in Z direction */
   int fillval)		/* fill the image with this value */
{
    int x, y, z; /* Pixel counter */
    int starty, startz;
    double tmp1, tmp2, tmpA, tmpB, tmpC, tmpD, a, b, c;
    Type *p, *q;

    /* invert the angles (images have their origin on top...) */
    a = -a1;
    b = -b1;
    c = -c1;

    /* calculate dimension of the bounding box after shearing */
    sheardim(2, nxin, nyin, nzin, nxout, nyout, nzout, a, b, c);
    LIARdebug("newX = %d, newY = %d, newZ = %d", *nxout, *nyout, *nzout);

    /* allocate the ouput buffer */
    (*imout) = (Type *)malloc((*nxout)*(*nyout)*(*nzout)*sizeof(Type));
    if (*imout == NULL) {
	LIARerror("Shear2_nn: insufficient memory");
	return 1;
    }
    memset((*imout), fillval, (*nxout)*(*nyout)*(*nzout)*sizeof(Type));

    tmpA = b * (nxin - 1);
    tmpB = c * (nyin - 1);
    tmpC = tmpA + tmpB;
    tmpD = a * (nxin - 1);

    /* get z offset */
    if (tmpA < 0 || tmpB < 0 || tmpC < 0) {
	if (tmpA < tmpB)
	    tmp1 = tmpA;
	else tmp1 = tmpB;
	if (tmpC < tmp1)
	    tmp1 = tmpC;
    } else
	tmp1 = 0;

    /* get y offset */
    tmp2 = (tmpD < 0) ? tmpD : 0;

    for (x = 0 ; x < nxin ; x++) {
	for (y = 0 ; y < nyin ; y++) {
	    p = imin + x + nxin * y;
	    q = (*imout) + x;

	    startz = std::max(bottom(b * x + c * y  - tmp1), 0);
	    starty = std::max(bottom(y + a * x - tmp2), 0);

	    q += ((*nxout) * (*nyout)) * startz + (*nxout) * starty;

	    for (z = 0 ; z < nzin ; z++) {
		*q = *p;
		q += (*nxout) * (*nyout);
		p += nxin * nyin;
	    }
	}
    }
    return 0;
}

template <typename Type>
int orthorot_3d
  (Type *bufin,	/* input buffer  */
   Type **bufout,	/* output buffer */
   int inx,		/* nb of cols in the input */
   int iny,		/* nb of rows in the input */
   int inz,		/* nb of slices in the input */
   int *fnx,		/* nb of cols in the output */
   int *fny,		/* nb of rows in the output */
   int *fnz,		/* nb of slices in the output */
   int rotx,		/* rotational angle about X */
   int roty,		/* rotational angle about Y */
   int rotz)		/* rotational angle about Z */
{
    int nx, ny, nz;
    int x, y, z;
    int res, tmp;
    Type *tmpbuf;

    x = (int) get_angle((double) rotx);
    y = (int) get_angle((double) roty);
    z = (int) get_angle((double) rotz);

    /* get the image dimensions */
    nx = inx;
    ny = iny;
    nz = inz;

    if ((tmpbuf = (Type*) malloc(nx * ny * nz * sizeof(Type))) == NULL) {
	LIARerror("Orthorot_3d: insufficient memory");
	return(1);
    }
    if (x == 180 && y == 180 && z == 180) {
	if ((res = pointsymm_3d(bufin, tmpbuf, nx, ny, nz)) != 0)
	    return(1);
    } else if (x == 90 && y == 90 && z == 90) {
	if ((res = yrotp90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
	    return(2); /* Error in +90 degree rotation about Y */
	tmp = nx;
	nx = nz;
	nz = tmp; /* exchange nx/nz */
    } else {
	memcpy(tmpbuf, bufin, nx * ny * nz * sizeof(Type));
	/* perform x rotations with the angle */
	if (x == 90) {
	    /* do a rotation of 90 degrees... */
	    if ((res = xrotp90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(2); /* Error in +90 degree rotation about X */
	    tmp = ny;
	    ny = nz;
	    nz = tmp; /* exchange ny/nz */
	    LIARdebug("Intermediary rotation of +90 degrees about X");
	} else if (x == -90) {
	    /* do a rotation of -90 degrees... */
	    if ((res = xrotm90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(3); /* "Error in -90 degree rotation about X */
	    tmp = ny;
	    ny = nz;
	    nz = tmp; /* exchange ny/nz */
	    LIARdebug("Intermediary rotation of -90 degrees about X");
	} else if (x == 180) {
	    /* do a rotation of +180 degrees... */
	    if ((res = xrot180_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(4); /* "Error in 180 degree rotation about X */
	    LIARdebug("Intermediary rotation of +180 degrees about X");
	} else if (x != 0) {
	    LIARdebug("Incorrect angles of rotation");
	    return(5);
	}

	/* perform y rotations with the angle */
	if (y == 90) {
	    /* do a rotation of 90 degrees... */
	    if ((res = yrotp90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(2); /* Error in +90 degree rotation about Y */
	    tmp = nx;
	    nx = nz;
	    nz = tmp; /* exchange nx/nz */
	    LIARdebug("Intermediary rotation of +90 degrees about Y");
	} else if (y == -90) {
	    /* do a rotation of -90 degrees... */
	    if ((res = yrotm90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(3); /* "Error in -90 degree rotation about Y */
	    tmp = nx;
	    nx = nz;
	    nz = tmp; /* exchange nx/nz */
	    LIARdebug("Intermediary rotation of -90 degrees about Y");
	} else if (y == 180) {
	    /* do a rotation of +180 degrees... */
	    if ((res = yrot180_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(4); /* "Error in 180 degree rotation about Y */
	    LIARdebug("Intermediary rotation of +180 degrees about Y");
	} else if (y != 0) {
	    LIARdebug("Incorrect angles of rotation");
	    return(5);
	}

	/* perform orthogonal rotations with the right angles */
	if (z == 90) {
	    /* do a rotation of 90 degrees... */
	    if ((res = zrotp90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(2); /* Error in +90 degree rotation about Z */
	    tmp = nx;
	    nx = ny;
	    ny = tmp; /* exchange nx/ny */
	    LIARdebug("Intermediary rotation of +90 degrees about Z");
	} else if (z == -90) {
	    /* do a rotation of -90 degrees... */
	    if ((res = zrotm90_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(3); /* "Error in -90 degree rotation about Z */
	    tmp = nx;
	    nx = ny;
	    ny = tmp; /* exchange nx/ny */
	    LIARdebug("Intermediary rotation of -90 degrees about Z");
	} else if (z == 180) {
	    /* do a rotation of +180 degrees... */
	    if ((res = zrot180_3d(tmpbuf, tmpbuf, nx, ny, nz)) != 0)
		return(4); /* "Error in 180 degree rotation about Z */
	    LIARdebug("Intermediary rotation of +180 degrees about Z");
	} else if (z != 0) {
	    LIARdebug("Incorrect angles of rotation");
	    return(5);
	}
    }

    free(*bufout);
    *bufout = tmpbuf;
    *fnx = nx;
    *fny = ny;
    *fnz = nz;
    return(0);
}


/**
 * \brief Rotate an image by specified arbitrary amount

	Entire image is rotated, so for non-trivial
	angles, the output image is likely to have larger dimensions
	than the input image.
	
	The argument "rmbdr" can take the following values:

	<br> CALC_BORDER: estimates the size of the borders by calculating the positions 
	of the corners of the image after rotation
	<br> EAT_BORDER: trim off the widest possible fillval-filled border.
	This method may result in different-sized components
	when run on multicomponent images

	SAME_SIZE: make the output image the same size as the input border

	The argument "interpolate" can take the following values:
	
	NEAREST_NEIGHBOUR 
	<br> BILINEAR_INTERP

	TESTS: See wrapper test.

	BUGS: When alpha=90degrees, the border argument is ignored. 
		  The function behaves as if CALC_BORDER was used.

 * \return Rotated image
 * \author John Lin
 * \date 09 Dec 1998
*/

template <typename Type>
int lrotate3d
  (Type *bufin,		/* input buffer  */
   Type **bufout,	/* output buffer */
   int inx,			/* nb of cols in the input */
   int iny,			/* nb of rows in the input */
   int inz,			/* nb of slices in the input */
   int *fnx,			/* nb of cols in the output */
   int *fny,			/* nb of rows in the output */
   int *fnz,			/* nb of slices in the output */
   double alpha,		/* Euler angle 1, in degrees */
   double beta,		/* Euler angle 2, in degrees */
   double gamma,		/* Euler angle 3, in degrees */
   int interpolate,		/* interpolate rotated image */
   int value,		/* fill the image with this value */
   int rmbdr,		/* remove zero border around image */
   int row0,                 /* row center of rot */
   int col0,                 /* col center of rot */
   int slice0)               /* col center of rot */
{
    int nx, ny, nz, onx, ony, onz, realinx, realiny, realinz;
    int res;
    double radian1, radian2, radian3;
    double a, b, c, realalpha;
    Type *tmpbuf, *tmpinbuf, *originbuf;

    /* pre-process the rotation angle. It should not exceed
       +/-45 degree (memory usage become prohibitive */
    LIARdebug("Original rotation angles: %.3f %.3f, %.3f", alpha, beta, gamma);

    alpha = get_angle(alpha);
    beta = get_angle(beta);
    gamma = get_angle(gamma);

    LIARdebug("Modified rotation angles: %.3f %.3f, %.3f", alpha, beta, gamma);
    
    realinx = inx;
    realiny = iny;
    realinz = inz;
    realalpha = alpha;
    
    originbuf =  tmpinbuf = bufin; /* by default */
    /* fix alpha to be < 45 */
    if (alpha > 45.0) {
	/* angle is 0 <= angle <= 180 */
	tmpinbuf = (Type *)malloc(inx*iny*inz*sizeof(Type));
	if (tmpinbuf != NULL)
	    memcpy(tmpinbuf, bufin, inx*iny*inz*sizeof(Type)); 
	if (alpha > 135.0) {
	    zrot180_3d(tmpinbuf, tmpinbuf, inx, iny, inz);
	    alpha -= 180.0;
	} else {
	    /* we need another intermediary image */
	    zrotp90_3d(tmpinbuf, tmpinbuf, inx, iny, inz);
	    alpha -= 90.0;
	    SWAPINT(inx, iny); /* nz doesn't change */
	}
    } else if (alpha < -45.0) {
	 /* we need another intermediary image */
	tmpinbuf = (Type *)malloc(inx*iny*inz*sizeof(Type));
	if (tmpinbuf != NULL)
	    memcpy(tmpinbuf, bufin, inx*iny*inz*sizeof(Type));

	if (alpha < -135.0) {
	    zrot180_3d(tmpinbuf, tmpinbuf, inx, iny, inz);
	    alpha += 180.0;
	} else {
	    zrotm90_3d(tmpinbuf, tmpinbuf, inx, iny, inz);
	    alpha += 90.0;
	    SWAPINT(inx, iny);
	}
    }
    
    /* the `in?' variables are immutable from this point on */
    nx = inx;
    ny = iny;
    nz = inz;
    bufin = tmpinbuf;

    /* now the really difficult part */
    if (alpha == 0.0 && beta == 0.0 && gamma == 0.0) {
	/* this is quite possible! construct the output image */
	*bufout = (Type *) malloc(nx * ny * nz * sizeof(Type));
	memcpy(*bufout, bufin, nx * ny * nz * sizeof(Type));
	LIARdebug("No need to rotate!");
	*fnx = nx;
	*fny = ny;
	*fnz = nz;

	if (rmbdr == SAME_SIZE){
	  
	}

	res = 0;
    } else {
	/* convert the angles into radians first */
	radian1 = alpha * M_PI / 180.0;
	radian2 = beta * M_PI / 180.0;
	radian3 = gamma * M_PI / 180.0;

	a = pink::rint(-tan((radian1+radian3)/2));
	b = pink::rint(cos((radian1-radian3)/2) / cos((radian1+radian3)/2) * tan(radian2/2));
	c = pink::rint(-sin(radian3) * tan(radian2/2));

        LIARdebug("Shear factors: %.3f %.3f, %.3f", a, b, c);
        
	if (interpolate == 1)
	    res = shear1_itp(bufin, bufout, nx, ny, nz, &onx, &ony, &onz, \
			     a, b, c, value);
	else
	    res = shear1_nn(bufin, bufout, nx, ny, nz, &onx, &ony, &onz, \
			    a, b, c, value);
	if (res == 0) {
	    a = pink::rint(sin(radian1+radian3));
	    b = pink::rint(-sin(radian2) * cos(radian3));
	    c = pink::rint(-sin((radian1-radian3)/2) / cos((radian1+radian3)/2) * sin(radian2));
	    if (interpolate == 1)
		res = shear2_itp(*bufout, &tmpbuf, onx, ony, onz, &nx, &ny, \
				 &nz, a, b, c, value);
	    else
		res = shear2_nn(*bufout, &tmpbuf, onx, ony, onz, &nx, &ny, \
				&nz, a, b, c, value);
	    if (res == 0) {
		free(*bufout);
		a = pink::rint(-tan((radian1+radian3)/2));
		b = pink::rint(cos(radian1) * tan(radian2/2));
		c = pink::rint(sin(radian1) * tan(radian2/2));
		if (interpolate == BILINEAR_INTERP)
		    res = shear1_itp(tmpbuf, bufout, nx, ny, nz, &onx, &ony, \
				    &onz, a, b, c, value);
		else
		    res = shear1_nn(tmpbuf, bufout, nx, ny, nz, &onx, &ony, \
				    &onz, a, b, c, value);
		if (res == 0) {
		    free(tmpbuf);
		    switch (rmbdr) {
		      case DEBUG_BORDER:
			/* for debugging purposes only */
			*fnx = onx;
			*fny = ony;
			*fnz = onz;
			break;
			
		      case CALC_BORDER:
			res = pullrotated_3d(*bufout, &tmpbuf, onx, ony, onz, \
					     inx, iny, inz, fnx, fny, fnz, \
					     alpha, beta, gamma);
			break;

		      case EAT_BORDER:
			res = rmborder_3d(*bufout, &tmpbuf, onx, ony, onz, \
					  fnx, fny, fnz, value);
			break;

		      case SAME_SIZE:
			res = pullcenteredsubset_3d(*bufout, &tmpbuf, onx, ony, onz,
						    realinx, realiny, realinz, row0, col0, slice0,
						    realalpha, beta, gamma);
			*fnx = realinx;
			*fny = realiny;
			*fnz = realinz;
			break;
		    }
		    if ((res == 0) && (rmbdr != -1)) {
 			free(*bufout);
			*bufout = tmpbuf;
		    }
		}
	    } else {
                LIARdebug("Shear 2 failed\n");
            }
	} else {
            LIARdebug("Shear 1 failed\n");
        }
    }

    /* if we used an extra buffer */
    if (tmpinbuf != originbuf)
	free(tmpinbuf);
    
    return(res);

}


template<>
int lrotate3d(fcomplex *bufin, fcomplex **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0)
{
    //
    return 0;
}

template<>
int lrotate3d(dcomplex *bufin, dcomplex **bufout, int inx, int iny,int inz,int *fnx,int *fny,int *fnz,double alpha,double beta,double gamma,int interpolate,int value,int rmbdr,int row0,int col0,int slice0)
{
    //
    return 0;
}

#endif // ROTATE3D_HPP
