/* $Id: avsimage.h,v 1.4 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/*
  Functions to be added to mcimage.h (or not)

  Author: Andre Vital Saude, 2005
*/

//Datatypes
typedef struct Coordinates {
    int32_t x, y, z;
} Coordinates;
typedef Coordinates AVS_Point;

typedef struct AVS_PointSet {
  int32_t size;
  AVS_Point points[48];
} AVS_PointSet;

//Image data access
#define AVS_point2d(I, p, rs) (I[(p.y)*rs + (p.x)])
#define AVS_point3d(I, p, ps, rs) (I[(p.z)*ps + (p.y)*rs + (p.x)])
#define AVS_pixel(I, x, y, rs) (I[(y)*(rs) + (x)])
#define AVS_voxel(I, x, y, z, ps, rs) (I[(z)*(ps) + (y)*(rs) + (x)])

//Additional functions
extern int32_t findMaxLong(uint32_t *gg, int32_t n);
extern int32_t samedimsimages(struct xvimage *im1, struct xvimage *im2);
#ifdef __cplusplus
}
#endif
