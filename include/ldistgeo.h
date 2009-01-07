/* $Id: ldistgeo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldistgeo(
  struct xvimage *imgx, 
  struct xvimage *imgy, 
  int32_t connex,
  struct xvimage *res
);

extern int32_t ldistgeo2d(struct xvimage *imgx,   /* donnee: image binaire */       
               struct xvimage *imgy,   /* donnee: image binaire (masque) */
               int32_t connex,
               struct xvimage *res     /* resultat: distances (doit être allouée) */
		   );
extern int32_t ldistgeo3d(struct xvimage *imgx,   /* donnee: image binaire */       
               struct xvimage *imgy,   /* donnee: image binaire (masque) */
               int32_t connex,
               struct xvimage *res     /* resultat: distances (doit être allouée) */
		   );
#ifdef __cplusplus
}
#endif
