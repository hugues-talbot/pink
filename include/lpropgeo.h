/* $Id: lpropgeo.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for lpropgeo.h */
/* ============== */

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
