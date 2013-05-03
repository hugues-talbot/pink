
/*
 * File:		rotate3d.h
 *
 *
 *
*/

#ifndef ROTATE3D_H
#define ROTATE3D_H

#include <math.h>

/* can be useful */
#ifdef DMALLOC
#include "dmalloc.h"
#endif



#define myhypot(x,y) (sqrt((x)*(x)+(y)*(y)))
#define top(x)	  ((x) == 0.0 ? 0 : (int)floor((x))+1)
#define bottom(x) ((int)floor((x)))
#define LARGEDBL   10000000000.0
#define myround(x) (x) /* use to be standard technique to avoid rounding errors, that doesnt work */

#define SWAPINT(a,b) do {int c; c = (a); (a) = (b) ; (b) = c;  } while (0)


typedef enum {CALC_BORDER=0, EAT_BORDER, SAME_SIZE , DEBUG_BORDER} border_mgmt;
typedef enum {NEAREST_NEIGHBOUR=0, BILINEAR_INTERP} interp_mgmt;

// Prototypes

void addborder_3d(void *in,	    /* Input buffer  */
		  void **out,	    /* Output buffer */
		  int size,	    /* Image pixel size */
		  int inx,	    /* nb of cols (input) */
		  int iny,	    /* nb of rows (input) */
		  int inz,	    /* nb of slices (input) */
		  int *outx,	    /* nb of cols (output) */
		  int *outy,	    /* nb of rows (output) */
		  int *outz,	    /* nb of slices (output) */
		  int offsetx,	    /* offset value in x */
		  int offsety,	    /* offset value in y */
		  int offsetz);

void getsubset_3d(void *in,	    /* Input buffer  */
		  void **out,	    /* Output buffer */
		  int size,	    /* Image pixel size */
		  int inx,	    /* nb of cols (input) */
		  int iny,	    /* nb of rows (input) */
		  int inz,	    /* nb of slices (input) */
		  int *outx,	    /* nb of cols (output) */
		  int *outy,	    /* nb of rows (output) */
		  int *outz,	    /* nb of slices (output) */
		  int offsetx,	    /* offset value in x */
		  int offsety,	    /* offset value in y */
		  int offsetz);	    /* offset value in z */
		  
		  
void rec2p(double x, double y, double *rho, double *phi);

void p2rec(double rho, double phi, double *x, double *y);

void rotvect_3d(double x, double y, double z, double a, double b, double c,
		double *newx, double *newy, double *newz);
		
		
void sheardim(int mode,		/* different shear modes */
	      int origx,	/* nb of cols in the unrotated image */
	      int origy,	/* nb of rows in the unrotated image */
	      int origz,	/* nb of slices in the unrotated image */
	      int *outx,	/* nb of cols (output) */
	      int *outy,	/* nb of rows (output) */
	      int *outz,	/* nb of slices (output) */
	      double a,		/* shear factor in X direction */
	      double b,		/* shear factor in Y direction */
	      double c);		/* shear factor in Z direction */		
		
		
double get_angle(double old_angle);
		
int neworig2d(int oldx,  /* old origin, from top-left corner */
	      int oldy, 
	      int oldnx, /* old image dimensions */
	      int oldny,
	      int newnx, /* new image dimensions */
	      int newny,
	      int *newx, /* new origin (same system) */
	      int *newy,
	      double a);  /* rotation angle in degrees */		
		
	
		
#endif // ROTATE3D_H
		
