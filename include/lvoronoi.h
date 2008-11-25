/* $Id: lvoronoi.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t ldelaunay(point *S, int32_t n, int32_t ***v, int32_t **nv, int32_t **ec);
extern int32_t ltriang_delaunay(point *S, int32_t n, int32_t **v, int32_t *nv, int32_t *ec);
