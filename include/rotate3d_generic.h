/*
 * File:		rotate3d.h
 *
 *
 *
*/

#ifndef ROTATE3D_GENERIC_HPP
#define ROTATE3D_GENERIC_HPP


template <typename Type>
int xrotp90_3d (
	Type *bufin,	    /* Input buffer  */
  	Type *bufout,    /* Output buffer */
   	int nx,			 /* nb of cols (input image) */
   	int ny,		    /* nb of rows (input image) */
   	int nz);		    /* nb of slices (input image) */


template <typename Type>
static int xrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */
   
   
template <typename Type>
static int xrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */


template <typename Type>
int yrotp90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */
   
   
 template <typename Type>
static int yrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */
   
   
   
template <typename Type>
static int yrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */  
   
   
template <typename Type>
int zrotp90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */   
   
   
template <typename Type>
int zrotm90_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */  
   
  
   
template <typename Type>
int zrot180_3d
  (Type *bufin,	    /* Input buffer  */
   Type *bufout,    /* Output buffer */
   int nx,		    /* nb of cols (input image) */
   int ny,		    /* nb of rows (input image) */
   int nz);		    /* nb of slices (input image) */ 
   
 
template <typename Type>
int xflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz);		/* nb of slices (input image) */  
     
     
template <typename Type>
int yflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz);		/* nb of slices (input image) */    
     
     
template <typename Type>
int zflip_3d
  (Type *bufin,	/* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz);		/* nb of slices (input image) */     
     
     
template <typename Type>
int pointsymm_3d
  (Type *bufin,	 /* Input buffer  */
   Type *bufout,	/* Output buffer */
   int nx,		/* nb of cols (input image) */
   int ny,		/* nb of rows (input image) */
   int nz);		/* nb of slices (input image) */ 
     
     
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
   int *outz);	    /* nb of slices (output) */
     
  
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
   int bgval);	/* value of the background */ 
   
   
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
   double gamma);	/* shear factor in radians */
   
  
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
   double c);
   
   
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
   int fillval);		/* fill the image with this value */  
   

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
   int fillval);		/* fill the image with this value */   
   
   
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
   int fillval);		/* fill the image with this value */
 
   
   
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
   int fillval);		/* fill the image with this value */  
   
   
   
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
   int rotz);		/* rotational angle about Z */  
   
   
   
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
   int slice0);               /* col center of rot */ 
   
     


#endif // ROTATE3D_GENERIC_HPP
