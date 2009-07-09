/*
 * File:		lfmmdist.c
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		November 2002
 *
 * CSIRO Mathematical and Information Sciences is the owner of all
 * copyright subsisting in the software contained in this file. It may
 * not be disclosed to, or used by, anyone without the prior approval
 * of the Chief, CSIRO Mathematical and Information Sciences.
 *
*/

/*********************************************************************************************
 lfmmdist.c
 ------

  DESCRIPTION:
  A wrapper for geodist, which computes geodesic distance transforms from multiple seeds.

  HISTORY:
  Created by Ben Appleton (28/11/02)
  Contact: appleton@itee.uq.edu.au

  Modified for Pink by Hugues Talbot	 7 Jul 2009
**********************************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pde_toolbox.h"
#include "lfmm.h"

int lfmmdist(
	/* Input image */
	INT4_TYPE  * seed_in_buf,			/* Seeds from which to grow distance function */
	INT4_TYPE  * seed_out_buf,		/* Result of growing seeds - may point to seed_in_buf */
	uint32_t * dim_buf,					/* The image dimensions */
	uint32_t   dim_length,					/* The number of image dimesions */
	/* Metric image */
	DBL_TYPE    * g_buf,
	/* Halting criteria */
	const char stopping,			/* The type of halting criteria */
	const float threshold,			/* The halting threshold */
	/* Output image */
	DBL_TYPE * distance_buf
)
{
	BVECT * dim;
	BIMAGE * seeds, * g, * distance;
	int i;
	int num_pixels;

	/* Set up a BVECT to describe the image dimensions */
	dim = BVECT_constructor(dim_length);
	memcpy(dim->buf, dim_buf, dim_length*sizeof(int));
	num_pixels = BVECT_prod(dim);

	/* Create the BIMAGEs from the image data */
	seeds = BIMAGE_constructor_int((int32_t*)seed_in_buf, dim);
	g = BIMAGE_constructor_float(g_buf, dim);
	distance = BIMAGE_constructor(dim);

	/* Call the geodist function from the level set toolbox */
	geodist(seeds, g, stopping, threshold, distance);

	/* Display any LSTB_error or LSTB_debug messages */
	lreadLSTBmsgs();

	/* Convert back to expected output */
	for (i = 0; i < num_pixels; i++) {
		distance_buf[i] = (DBL_TYPE)distance->buf[i];
		seed_out_buf[i] = (INT4_TYPE)seeds->buf[i];
	}

	/* Free everything */
	BVECT_destructor(dim);
	BIMAGE_destructor(seeds);
	BIMAGE_destructor(g);
	BIMAGE_destructor(distance);

	return 0;
}

