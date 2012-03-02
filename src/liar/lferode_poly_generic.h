/*
 * File:		lferode_poly_generic.h
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

#include "liarlmts.h"                   
#include "liarp.h"

int XCAT2(lferode_poly, I_NAME)
(I_TYPE *inbuf, I_TYPE *outbuf, int ncol, int nrow, int radius, int type, int sides)
{
    char LIARstrbuf[1024];

    if (sides == 0) {
	if (radius<=3)       
	    sides=4;
	else
	    if (radius<=8) 
		sides=8;
	    else 
		if (type)
		    sides = 8;
		else
		    sides = 12;
    }

    sprintf(LIARstrbuf, "Erosion by a polygone, %d sides, radius= %d, type= %d", sides, radius, type);
    LIARdebug(LIARstrbuf);

    /* doesn't work */
/*     if (inbuf != outbuf) */
/* 	memcpy(outbuf, inbuf, ncol*nrow); */

    if (type == PERIODIC) {
	XCAT2(circleminmax, I_NAME)
	    (outbuf, ncol, nrow, radius, XCAT2(genfmin, I_NAME), periodic, sides/2);
    } else {
	XCAT2(circleminmax, I_NAME)
	    (outbuf, ncol, nrow, radius, XCAT2(genfmin, I_NAME), bresenham, sides/2);
    }
    return 0;
}














