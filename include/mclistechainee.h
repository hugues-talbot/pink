/* $Id: mclistechainee.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* definitions pour le module "mclistechainee.c" */

typedef int32_t TypElement;

typedef struct LISTE {         /* definition de la structure Liste        */
  TypElement elt;              /* element en tete de liste   */
  struct LISTE   *suite;       /* pointe sur la suite de la liste         */
} TypListe;

extern TypListe * ListeVide();
extern TypElement Tete(TypListe * l);
extern TypListe * Suite(TypListe * l);
extern TypListe * Cons(TypElement e, TypListe * l);
extern void AfficheListe(TypListe * l);
extern void DetruitListe(TypListe * l);
extern int32_t Vide(TypListe * l);
#ifdef __cplusplus
}
#endif
