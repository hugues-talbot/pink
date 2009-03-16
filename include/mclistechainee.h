/* $Id: mclistechainee.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* definitions pour le module "mclistechainee.c" */

typedef int32_t TypElement;

typedef struct LISTECHAINEE {         /* definition de la structure Listechainee        */
  TypElement elt;              /* element en tete de listechainee   */
  struct LISTECHAINEE   *suite;       /* pointe sur la suite de la listechainee         */
} TypListechainee;

extern TypListechainee * ListechaineeVide();
extern TypElement Tete(TypListechainee * l);
extern TypListechainee * Suite(TypListechainee * l);
extern TypListechainee * Cons(TypElement e, TypListechainee * l);
extern void AfficheListechainee(TypListechainee * l);
extern void DetruitListechainee(TypListechainee * l);
extern int32_t EstVideListechainee(TypListechainee * l);
extern TypListechainee * InListechainee(TypElement el, TypListechainee * lis);
extern TypListechainee * UnionListechainee (TypListechainee * lis1, TypListechainee * lis2);

#ifdef __cplusplus
}
#endif
