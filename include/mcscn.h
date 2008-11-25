#define TYP_OBJ_LINE         0
#define TYP_OBJ_CLOSEDLINE   1
#define TYP_OBJ_SPLINE       2
#define TYP_OBJ_CLOSEDSPLINE 3
#define TYP_OBJ_POLYGON      4

typedef struct {
  double x, y, z;
} point3;

typedef struct {
  int32_t typ_obj;
  int32_t npoints;
  point3 * points;
} scene_obj;

typedef struct {
  int32_t nobj;
  scene_obj *objs;
} scene3d;
