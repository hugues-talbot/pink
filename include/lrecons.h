/* $Id: lrecons.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lrecons(
        struct xvimage *img,    /* image de depart en ndg */
        struct xvimage *seg,    /* image binaire (segmentation max) et resultat */
        int32_t connex,             /* 4, 8  */
        int32_t memoire);

