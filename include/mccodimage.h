/* $Id: mccodimage.h,v 1.4 2009-01-06 13:18:06 mcouprie Exp $ */
#define NDG_MAX 255            /* niveau de gris max */
#define NDG_MIN 0              /* niveau de gris min */

/* definitions for data storage type,
   uint32_t data_storage_type; */
#define	VFF_TYP_BIT		0	/* pixels are on or off (binary image)*/
                                        /* Note: This is an X11 XBitmap 
					   with bits packed into a byte and
					   padded to a byte */
#define	VFF_TYP_1_BYTE		1	/* pixels are byte (uint8_t) */
#define	VFF_TYP_2_BYTE		2	/* pixels are two byte (int16_t) */
#define	VFF_TYP_4_BYTE		4	/* pixels are four byte (integer) */
#define	VFF_TYP_FLOAT		5	/* pixels are float (single precision)*/
#define VFF_TYP_DOUBLE		9	/* pixels are float (double precision)*/

struct xvimage {
  char *name;
  uint32_t row_size;                    /* Size of a row (number of columns) */
  uint32_t col_size;                    /* Size of a column (number of rows) */
  uint32_t depth_size;                  /* Number of planes (for 3d images) */
  uint32_t time_size;                   /* Number of (2d or 3d) images */
  uint32_t num_data_bands;	        /* Number of bands per data pixel,
					   or number of bands per image, or
					   dimension of vector data, or
					   number of elements in a vector */
  uint32_t data_storage_type;           /* storage type for disk data */
  double xdim, ydim, zdim;              /* voxel dimensions in real world */
  uint32_t xmin, xmax;                  /* region of interest: x coordinates */
  uint32_t ymin, ymax;                  /* region of interest: y coordinates */
  uint32_t zmin, zmax;                  /* region of interest: z coordinates */
  void * image_data;                    /* pointer on raw data */
};

#define UCHARDATA(I)   ((uint8_t*)((I)->image_data))
#define USHORTDATA(I)  ((uint16_t*)((I)->image_data))
#define ULONGDATA(I)   ((uint32_t*)((I)->image_data))
#define FLOATDATA(I)   ((float*)((I)->image_data))
#define DOUBLEDATA(I)  ((double*)((I)->image_data))
#define colsize(I)     ((I)->col_size)
#define rowsize(I)     ((I)->row_size)
#define depth(I)       ((I)->depth_size)
#define tsize(I)       ((I)->time_size)
#define nbands(I)      ((I)->num_data_bands)
#define datatype(I)    ((I)->data_storage_type)
#define pixel(I,x,y)   (((uint8_t*)((I)->image_data))[(y)*(I)->row_size+(x)])
#define voxel(I,x,y,z) (((uint8_t*)((I)->image_data))[((z)*(I)->col_size+(y))*(I)->row_size+(x)])
#define lpixel(I,x,y)   (((uint32_t*)((I)->image_data))[(y)*(I)->row_size+(x)])
#define lvoxel(I,x,y,z) (((uint32_t*)((I)->image_data))[((z)*(I)->col_size+(y))*(I)->row_size+(x)])

/*            
		Codage du voisinage


		3	2	1			
		4	X	0
		5	6	7
*/
#define EST 0
#define NORD 2
#define OUEST 4
#define SUD 6
#define NORD_EST 1
#define NORD_OUEST 3
#define SUD_OUEST 5
#define SUD_EST 7
#define DEVANT 8
#define DERRIERE 10

#define nonbord(p,rs,N) ((p%rs!=rs-1)&&(p>=rs)&&(p%rs!=0)&&(p<N-rs))
#define nonbord3d(p,rs,ps,N) ((p>=ps)&&(p<N-ps)&&(p%ps>=rs)&&(p%ps<ps-rs)&&(p%rs!=0)&&(p%rs!=rs-1))

/* ============== */
/* prototypes     */
/* ============== */

extern int32_t voisin(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t voisin2(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t voisin6(int32_t i, int32_t k, int32_t rs, int32_t n, int32_t nb);
extern int32_t bord(int32_t i, int32_t rs, int32_t nb);
extern int32_t bord3d(int32_t i, int32_t rs, int32_t ps, int32_t nb);
extern int32_t voisin26(int32_t i, int32_t k, int32_t rs, int32_t n, int32_t nb);
extern int32_t voisin18(int32_t i, int32_t k, int32_t rs, int32_t n, int32_t nb);
extern int32_t voisins4(int32_t i, int32_t j, int32_t rs);
extern int32_t voisins8(int32_t i, int32_t j, int32_t rs);   
extern int32_t voisins6(int32_t i, int32_t j, int32_t rs, int32_t ps);   
extern int32_t voisins18(int32_t i, int32_t j, int32_t rs, int32_t ps);   
extern int32_t voisins26(int32_t i, int32_t j, int32_t rs, int32_t ps);   
extern int32_t voisin5(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t voisin6b(int32_t i, int32_t k, int32_t rs, int32_t nb, int32_t par);
extern int32_t voisinNESO(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t voisinNOSE(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t voisin14b(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N);
extern int32_t voisinONAV(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N );
extern int32_t voisinENAR(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N );
extern int32_t voisinENAV(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N );
extern int32_t voisinONAR(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N );
extern uint32_t maskvois26(uint8_t *F, uint32_t bitmask, int32_t i, int32_t rs, int32_t ps, int32_t N);
