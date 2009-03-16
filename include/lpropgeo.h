/* $Id: lpropgeo.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lpropgeo.h */
/* ============== */

#undef MIN
#undef MAX
#define MIN  0
#define MAX  1
#define MOY  2
#define MIN1 3
#define MAX1 4
#define MOY1 5
#define MINB 6
#define MAXB 7
#define MOYB 8
#define RANDB 9

extern int32_t lpropgeo(
        struct xvimage *img1,
        struct xvimage *mask, 
        int32_t connex, 
        int32_t function
);

extern int32_t lpropgeolong(
        struct xvimage *img1,
        struct xvimage *mask, 
        int32_t connex, 
        int32_t function
	);
#ifdef __cplusplus
}
#endif
