/* $Id: lgeo.h,v 1.4 2009-06-18 06:34:55 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _mcgeo_h
#include <mcgeo.h>
#endif

extern int32_t lellipticite(
         struct xvimage *img,    /* image de depart */
         int32_t connex);            /* 4, 8  */

extern int32_t lfenetreincluante(
        struct xvimage *img,    /* image de depart */
        int32_t connex);            /* 4, 8  */

extern int32_t lboxmin(struct xvimage *img);

extern int32_t lmatchrect(
        struct xvimage *img,    /* image de depart */
        int32_t connex,             /* 4, 8  */
        int32_t arrondi);           /* booleen */

extern int32_t lrectangleincluant(
        struct xvimage *img,    /* image de depart */
        int32_t connex);            /* 4, 8  */

extern int32_t lmatchellipse(
        struct xvimage *img,    /* image de depart */
        int32_t connex);            /* 4, 8  */

extern struct xvimage *lcar2pol(struct xvimage *img, double xc, double yc);

extern int32_t lpol2car(struct xvimage *img, struct xvimage *res, double xc, double yc);

extern struct xvimage *lsection(struct xvimage *img, 
                         double x0, double y0, double z0,
                         double x1, double y1, double z1,
                         double x2, double y2, double z2
				);

extern rectangle *AjusteRectangle(int32_t *pb, int32_t npb, double xc, double yc, double theta, double h, double w, int32_t rs);

extern rectangle *RectangleIncluant(int32_t *pb, int32_t npb, double xc, double yc, double theta, int32_t rs);

extern rectanglearrondi *AjusteRectangleArrondi(int32_t *pb, int32_t npb, double xc, double yc, 
						double theta, double h, double w, int32_t rs);

extern cercle *AjusteCercle(struct xvimage *img, double xc, double yc, double r);

extern cercle *AjusteCercle2(struct xvimage *img, double xc, double yc, double r);

#ifdef __cplusplus
}
#endif
