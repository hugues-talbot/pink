/* $Id: mclistechainee.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
