/*
 * File:		chngeval.c
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

void changePIXval(PIX_TYPE *IN, long size,
		  PIX_TYPE val1,
		  PIX_TYPE val2)
{
/**
ARGUMENTS:
	PIX_TYPE *IN:
	long size:  number of pixels in IN image.
	PIX_TYPE val1:
	PIX_TYPE val2:
RETURN VALUE:
	void 
DISCRIPTION:
	sets those pixel values in IN that are equal to val1
	to val2.
HISTORY:
TESTS: 
        (1) Linted.
REFERENCES:
**/

    long i;
    for(i=0;i<size;i++,IN++)
	if(*IN == val1)
	    *IN = val2;
}

void changeSEEDval(SEED_TYPE *IN, long size,
		  SEED_TYPE val1,
		  SEED_TYPE val2)
{
    long i;
    for(i=0;i<size;i++,IN++)
	if(*IN == val1)
	    *IN = val2;
}

INT4_TYPE testINTneighs(SEED_TYPE *v,LONG_TYPE *func,INT4_TYPE n)

{
/**
ARGUMENTS:
        SEED_TYPE *v:input image of seeds, offset to raster position
        LONG_TYPE *func: the neighbourhood function
        INT4_TYPE n: the connectivity
RETURN VALUE:
        INT4_TYPE 
DISCRIPTION:
        test neighbours
HISTORY:Written by Ed Breen
TESTS:Linted and purified
**/

    INT4_TYPE i,h=0;
    PIX_TYPE first=1;

    for(i=0;i<n;i++)
        if( *(v+func[i]) >0) {
            if(first) { 
                h = *(v+func[i]);
                first = 0;
             }else if(h != *(v+func[i])) 
                    return 2;
        }
    return 1;
}


void getsubset_3d(void *in,         /* Input buffer  */
                  void **out,       /* Output buffer */
                  int size,         /* Image pixel size */
                  int inx,          /* nb of cols (input) */
                  int iny,          /* nb of rows (input) */
                  int inz,          /* nb of slices (input) */
                  int *outx,        /* nb of cols (output) */
                  int *outy,        /* nb of rows (output) */
                  int *outz,        /* nb of slices (output) */
                  int offsetx,      /* offset value in x */
                  int offsety,      /* offset value in y */
                  int offsetz)      /* offset value in z */
{
    char *bufin, *bufout;
    int   y, z;
    long  offset;

    bufin  = in;
    bufout = *out;
    for (z = 0 ; z < (*outz) ; z++) {
        offset =  offsetx + inx * offsety  + (inx * iny) * (offsetz + z);
        offset *= size;
        for (y = 0 ; y < (*outy) ; y++, bufout += (*outx)*size, offset += inx*size)
            memcpy(bufout, bufin+offset, (*outx)*size);
    }
}


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
void addborder_3d(void *in,         /* Input buffer  */
                  void **out,       /* Output buffer */
                  int size,         /* Image pixel size */
                  int inx,          /* nb of cols (input) */
                  int iny,          /* nb of rows (input) */
                  int inz,          /* nb of slices (input) */
                  int *outx,        /* nb of cols (output) */
                  int *outy,        /* nb of rows (output) */
                  int *outz,        /* nb of slices (output) */
                  int offsetx,      /* offset value in x */
                  int offsety,      /* offset value in y */
                  int offsetz)      /* offset value in z */
{
    char *bufin, *bufout;
    int   y, z;
    long  offset;

    bufin = (char *)in;    /* we need to do pointer arithmetic */
    bufout = (char *)*out;
    for (z = 0 ; z < inz ; z++) {
        offset =  offsetx + (*outx)*offsety + ((*outx)*(*outy))*(offsetz+z);
        offset *= size;
        for (y = 0 ; y < iny ; y++, offset += (*outx)*size, bufin += inx*size)
            memcpy((char *)bufout + offset, bufin, inx*size);
    }
}
