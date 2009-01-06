extern int32_t lvoronoilabelling(struct xvimage *img,   /* donnee: image binaire */
			  struct xvimage *dist,  /* resultat: SEDT */
			  struct xvimage *vor    /* resultat: labelling */
			  );
extern int32_t lmedax_Hesselink(struct xvimage* f, struct xvimage* res, double gamma);
