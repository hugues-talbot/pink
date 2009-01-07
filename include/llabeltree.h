/* $Id: llabeltree.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t llabeltree(
  struct xvimage *imgx, 
  int32_t connex,
  struct xvimage *res
);

extern int32_t llabeltree2d(struct xvimage *imgx,   /* donnee: image binaire */       
                 int32_t connex,
                 struct xvimage *res     /* resultat: distances (doit être allouée) */
		     );

extern int32_t llabeltree3d(struct xvimage *imgx,   /* donnee: image binaire */       
                 int32_t connex,
                 struct xvimage *res     /* resultat: distances (doit être allouée) */
		     );

extern int32_t labelbranch2d4(uint8_t *X,  /* donnee: pointeur sur l'image x */
                   int32_t rs,            /* donnee: taille ligne */
		   int32_t N,             /* donnee: taille image */
                   int32_t current_pixel, /* donnee: numéro pixel courant */  
                   int32_t intersection,  /* donnee: flag d'intersection */  
                   uint32_t *D        /* resultat: pointeur sur distances */
); /* Etiquette les branches avec la 4-connexité */

extern int32_t labelbranch2d8(uint8_t *X,  /* donnee: pointeur sur l'image x */
                   int32_t rs,            /* donnee: taille ligne */
		   int32_t N,             /* donnee: taille image */
                   int32_t current_pixel, /* donnee: numéro pixel courant */  
                   int32_t intersection,  /* donnee: flag d'intersection */  
                   uint32_t *D        /* resultat: pointeur sur distances */
); /* Etiquette les branches avec la 8-connexité */

extern int32_t labelbranch3d6(uint8_t *X,  /* donnee: pointeur sur l'image x */
                   int32_t rs,            /* donnee: taille ligne */
                   int32_t ps,            /* donnee: taille plan */
		   int32_t N,             /* donnee: taille image */
                   int32_t current_pixel, /* donnee: numéro pixel courant */  
                   int32_t intersection,  /* donnee: flag d'intersection */  
                   uint32_t *D        /* resultat: pointeur sur distances */
); /* Etiquette les branches avec la 6-connexité */

extern int32_t labelbranch3d18(uint8_t *X,  /* donnee: pointeur sur l'image x */
                    int32_t rs,            /* donnee: taille ligne */
                    int32_t ps,            /* donnee: taille plan */
		    int32_t N,             /* donnee: taille image */
                    int32_t current_pixel, /* donnee: numéro pixel courant */  
                    int32_t intersection,  /* donnee: flag d'intersection */  
                    uint32_t *D        /* resultat: pointeur sur distances */
); /* Etiquette les branches avec la 18-connexité */

extern int32_t labelbranch3d26(uint8_t *X,  /* donnee: pointeur sur l'image x */
                    int32_t rs,            /* donnee: taille ligne */
                    int32_t ps,            /* donnee: taille plan */
		    int32_t N,             /* donnee: taille image */
                    int32_t current_pixel, /* donnee: numéro pixel courant */  
                    int32_t intersection,  /* donnee: flag d'intersection */  
                    uint32_t *D        /* resultat: pointeur sur distances */
); /* Etiquette les branches avec la 26-connexité */
#ifdef __cplusplus
}
#endif
