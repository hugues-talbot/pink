/*
  Functions to be added to mcimage.h (or not)

  Author: Andre Vital Saude, 2005
*/

//Datatypes
typedef struct Coordinates {
    int32_t x, y, z;
} Coordinates;
typedef Coordinates Point;

typedef struct PointSet {
  int32_t size;
  Point points[48];
} PointSet;

//Image data access
#define AVS_point2d(I, p, rs) (I[(p.y)*rs + (p.x)])
#define AVS_point3d(I, p, ps, rs) (I[(p.z)*ps + (p.y)*rs + (p.x)])
#define AVS_pixel(I, x, y, rs) (I[(y)*(rs) + (x)])
#define AVS_voxel(I, x, y, z, ps, rs) (I[(z)*(ps) + (y)*(rs) + (x)])

//Additional functions
extern int32_t findMaxLong(uint32_t *gg, int32_t n);
extern int32_t samedimsimages(struct xvimage *im1, struct xvimage *im2);
