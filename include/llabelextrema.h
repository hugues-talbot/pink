/* $Id: llabelextrema.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define LABMAX   0
#define LABMIN   1
#define LABFGD   3
#define LABBGD   4

/* ============== */
/* prototype for llabelextrema.c */
/* ============== */

extern int32_t llabelextrema(
        struct xvimage *img,
        int32_t connex,
        int32_t minimum,
        struct xvimage *lab,
        int32_t *nlabels
);

extern int32_t llabeldil(struct xvimage *f, 
		     struct xvimage *m, 
		     int32_t xc, int32_t yc, 
		     struct xvimage *lab,
		     int32_t *nlabels);

extern int32_t llabelbin(struct xvimage *f, 
		     int32_t connex,
		     struct xvimage *lab, 
		     int32_t *nlabels);

