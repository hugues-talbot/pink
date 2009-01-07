/* $Id: lvoronoilabelling.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lvoronoilabelling(struct xvimage *img,   /* donnee: image binaire */
			  struct xvimage *dist,  /* resultat: SEDT */
			  struct xvimage *vor    /* resultat: labelling */
			  );
extern int32_t lmedax_Hesselink(struct xvimage* f, struct xvimage* res, double gamma);
#ifdef __cplusplus
}
#endif
