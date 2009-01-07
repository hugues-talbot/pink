/* $Id: lvoronoi.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldelaunay(point *S, int32_t n, int32_t ***v, int32_t **nv, int32_t **ec);
extern int32_t ltriang_delaunay(point *S, int32_t n, int32_t **v, int32_t *nv, int32_t *ec);
#ifdef __cplusplus
}
#endif
