/*
 * File:		setpxbrd.c
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

#include "liarp.h"

void setPIXborder(PIX_TYPE *image, int nx, int ny, int nz,
		  PIX_TYPE val)
{
/**
ARGUMENTS:
	PIX_TYPE *image:	pointer to input image
	int nx:			no. of cols of input image
	int ny:			no. of rows of input image
	int nz:			no. of planes of input image
	PIX_TYPE val:		value to which border pixels are 
	                        set 
RETURN VALUE:
	void 
DISCRIPTION:
	Sets all border pixels to the supplied value, `val'.
        This is done in place, with no change to the size of 
	the image.
HISTORY:
	Modified on 4 Jan 1999 by John Lin for single row/column 
	case
TESTS:
**/
    PIX_TYPE *i2;
    int  i, j, k;
    long size;

    if (ny == 1) {
	i2 = image + nx - 1;
	for (k = 0 ; k < nz ; k++, image += nx, i2 += nx) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)nx * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    } else if (nx == 1){
	i2 = image + ny - 1;
	for (k = 0 ; k < nz ; k++, image += ny, i2 += ny) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)ny * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < ny ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    }
    
    for (k = 0 ; k < nz ; k++) {
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
	i2 = image + nx - 1;
	for (i = 2 ; i < ny ; i++, image += nx,i2 += nx) {
	    *image = val; 
	    *i2 = val;
	}
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
    }
    if (nz > 1) { /* do last and first  planes */
	size = (long)nx * (long)ny * (1 - nz);
	image += - nx - 2;
	i2 = image + size;
	for (j = 2 ; j < ny ; j++, image -= 2, i2 -= 2) {
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
    }
}

void setSEEDborder(SEED_TYPE *image, int nx, int ny, int nz,
		   SEED_TYPE val)    
{
/**
ARGUMENTS:
	SEED_TYPE *image:	pointer to input image
	int nx:			no. of cols of input image
	int ny:			no. of rows of input image
	int nz:			no. of planes of input image
	SEED_TYPE val:		value to which border pixels are set 
RETURN VALUE:
	void 
DISCRIPTION:
	Sets all border pixels to the supplied value, `val'.
HISTORY:
	Modified on 4 Jan 1999 by John Lin for single row/column case
TESTS:
**/
    SEED_TYPE *i2;
    int i, j, k;
    long size;

    if (ny == 1) {
	i2 = image + nx - 1;
	for (k = 0 ; k < nz ; k++, image += nx, i2 += nx) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)nx * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    } else if (nx == 1){
	i2 = image + ny - 1;
	for (k = 0 ; k < nz ; k++, image += ny, i2 += ny) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)ny * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < ny ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    }
    
    for (k = 0 ; k < nz ; k++) {
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
	i2 = image + nx - 1;
	for (i = 2 ; i < ny ; i++, image += nx,i2 += nx) {
	    *image = val; 
	    *i2 = val;
	}
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
    }
    if (nz > 1) { /* do last and first  planes */
	size = (long)nx * (long)ny * (1 - nz);
	image += - nx - 2;
	i2 = image + size;
	for (j = 2 ; j < ny ; j++, image -= 2, i2 -= 2) {
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
    }
}
	    
void setINT4border(INT4_TYPE *image, int nx, int ny, int nz,
		   INT4_TYPE val)    
{
/**
ARGUMENTS:
	INT4_TYPE *image:	pointer to input image
	int nx:			no. of cols of input image
	int ny:			no. of rows of input image
	int nz:			no. of planes of input image
	INT4_TYPE val:		value to which border pixels are set 
RETURN VALUE:
	void 
DISCRIPTION:
	Sets all border pixels to the supplied value, `val'.
HISTORY:
	Modified on 4 Jan 1999 by John Lin for single row/column case
TESTS:
**/
    INT4_TYPE *i2;
    int i, j, k;
    long size;

    if (ny == 1) {
	i2 = image + nx - 1;
	for (k = 0 ; k < nz ; k++, image += nx, i2 += nx) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)nx * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    } else if (nx == 1){
	i2 = image + ny - 1;
	for (k = 0 ; k < nz ; k++, image += ny, i2 += ny) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)ny * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < ny ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    }
    
    for (k = 0 ; k < nz ; k++) {
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
	i2 = image + nx - 1;
	for (i = 2 ; i < ny ; i++, image += nx,i2 += nx) {
	    *image = val; 
	    *i2 = val;
	}
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
    }
    if (nz > 1) { /* do last and first  planes */
	size = (long)nx * (long)ny * (1 - nz);
	image += - nx - 2;
	i2 = image + size;
	for (j = 2 ; j < ny ; j++, image -= 2, i2 -= 2) {
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
    }
}

void setDBLborder(DBL_TYPE *image, int nx, int ny, int nz,
		   DBL_TYPE val)    
{
/**
ARGUMENTS:
	DBL_TYPE *image:	pointer to input image
	int nx:			no. of cols of input image
	int ny:			no. of rows of input image
	int nz:			no. of planes of input image
	DBL_TYPE val:		value to which border pixels are set 
RETURN VALUE:
	void 
DISCRIPTION:
	Sets all border pixels to the supplied value, `val'.
HISTORY:
	Added DBL case - Leanne Bischof 15SEP00
TESTS:
**/
    DBL_TYPE *i2;
    int i, j, k;
    long size;

    if (ny == 1) {
	i2 = image + nx - 1;
	for (k = 0 ; k < nz ; k++, image += nx, i2 += nx) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)nx * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    } else if (nx == 1){
	i2 = image + ny - 1;
	for (k = 0 ; k < nz ; k++, image += ny, i2 += ny) {
	    *image = val;
	    *i2 = val;
	}
	if (nz > 1) {
	    size = (long)ny * (1 - nz);
	    image -= 2;
	    i2 = image + size;
	    for (i = 2 ; i < ny ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
	return;
    }
    
    for (k = 0 ; k < nz ; k++) {
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
	i2 = image + nx - 1;
	for (i = 2 ; i < ny ; i++, image += nx,i2 += nx) {
	    *image = val; 
	    *i2 = val;
	}
	for (i = 0 ; i < nx ; i++, image++)
	    *image = val;
    }
    if (nz > 1) { /* do last and first  planes */
	size = (long)nx * (long)ny * (1 - nz);
	image += - nx - 2;
	i2 = image + size;
	for (j = 2 ; j < ny ; j++, image -= 2, i2 -= 2) {
	    for (i = 2 ; i < nx ; i++) {
		*image-- = val;
		*i2-- = val;
	    }
	}
    }
}
