/* $Id: mclistechainee.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/***********************************/
/*   module de gestion de liste    */
/* Michel COUPRIE  -  Janvier 1990 */
/***********************************/

/* ATTENTION : allocation dynamique à chaque ajout d'élément - très inefficace
   utiliser plutôt mcliste.c
 */

/* ce module implemente le type abstrait:

   TYPE_ABSTRAIT Liste
   OPERATIONS
     ListeVide : -> Liste
     Tete      : Liste -> Element
     Suite     : Liste -> Liste
     Cons      : Element * Liste -> Liste
     Vide      : Liste -> Boolean
   RESTRICTIONS
     Tete(ListeVide)   = ERROR
     Suite(ListeVide)  = ERROR
   AXIOMES
     Tete(Cons(e,l))   = e
     Suite(Cons(e,l))  = l         
     Vide(ListeVide)   = TRUE
     Vide(Cons(e,l))   = FALSE
     
On pourra enrichir ce type par les operations:

     AfficheListe : Liste ->    (utilise AfficheElement)
     DetruitListe : Liste ->    (utilise DetruitElement)
     Longueur     : Liste -> Entier
     Concat       : Liste * Liste -> Liste
     Renverse     : Liste -> Liste
     InListe      : Element * Liste -> Liste
     Union        : Liste * Liste -> Liste
     etc...
     
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

//#define TEST_LISTE            /* pour compiler la fonction de test */

/*************************/
/* structures de donnees */
/*************************/

#include "mclistechainee.h"

/*****************************/
/* definition des operations */
/*****************************/

/*************************************************/
TypListe * ListeVide() 
{   /* retourne une liste vide */
  return(NULL);
} /* ListeVide() */
  
/*************************************************/
int32_t Vide(TypListe * l) 
{ 
                        /* retourne 1 si liste vide */
  if (l == NULL) return(1); else return(0);
} /* Vide() */
  
/*************************************************/
TypElement Tete (TypListe * lis) 
{
                        /* retourne l'element en tete
                           de la liste lis */
  return(lis->elt);
} /* Tete() */
  
/*************************************************/
TypListe * Suite (TypListe * lis) 
{
                        /* retourne la Suite de la liste lis,
                                 privee de son premier element */
  return(lis->suite);
} /* Suite() */

/*************************************************/
TypListe * Cons (TypElement el, TypListe * lis) 
{
                              /* retourne la liste dont la tete est el
                                 et dont la Suite est lis */
  TypListe * temp;
  temp = (TypListe *) malloc (sizeof (TypListe));
  temp->elt = el;
  temp->suite = lis;
  return(temp);
} /* Cons() */

/*************************************************/
void AfficheListe(TypListe * lis) 
{
                              /* affiche les elements de la liste */
  for (; lis != NULL; lis = lis->suite) 
    printf("%d ", lis->elt);
} /* AfficheListe() */

/*************************************************/
void DetruitListe(TypListe * lis) 
{
                              /* recupere la place occupee par les
                                 elements de la liste et par les
                                 liens du chainage. Attention: 
                                 ne modifie pas le contenu de lis */
  TypListe *temp, *temp1;
  for (temp = lis; temp != NULL; ) {
    temp1 = temp;
    temp = temp->suite;
    free(temp1);
  } /* for */
} /* DetruitListe() */

/*************************************************/
TypListe * InListe(TypElement el, TypListe * lis) 
{
                              /* retourne la premiere sous-liste qui a el en tete */
  for (; lis != NULL; lis = lis->suite)
    if (lis->elt == el) return lis;                        
  return NULL;
} /* InListe() */

/*************************************************/
TypListe * Union (TypListe * lis1, TypListe * lis2) 
{
                              /* retourne la liste lis1 qui contient, si lis1 et lis2
                                 representent des ensembles, l'union des elements */
  for (; lis2 != NULL; lis2 = lis2->suite)
    if (InListe(lis2->elt, lis1) == NULL)
      lis1 = Cons(lis2->elt, lis1);
  return lis1;
} /* Union() */

/*******************************/
/* programme principal de test */
#ifdef TEST_LISTE
main () {
  int32_t erreurs = 0;
  TypListe *liste1, *liste2;
                  
  liste1 = ListeVide();
  if (!Vide(liste1)) {
    printf("Erreur 1: Vide(ListeVide()) != 1\n");
    erreurs++;  
  }
  liste2 = Cons(1, liste1); /* (1) */
  if (Vide(liste2)) {
    printf("Erreur 2: Vide(Cons()) == 1\n");
    erreurs++;  
  }
  if (Tete(liste2) != 1) {
    printf("Erreur 3: Tete(Cons(e,l)) != e\n");
    erreurs++;
  }
  if (Suite(liste2) != liste1) {
    printf("Erreur 4: Suite(Cons(e,l)) != l\n");
    erreurs++;
  }
  liste1 = liste2;                 /* (1) */
  liste2 = Cons(2, liste1); /* (2 1) */
  if (Tete(liste2) != 2) {
    printf("Erreur 5: Tete(Cons(e,l)) != e\n");
    erreurs++;
  }
  if (Suite(liste2) != liste1) {
    printf("Erreur 6: Suite(Cons(e,l)) != l\n");
    erreurs++;
  }                    
  printf("affichage attendu = 2 1 ----> ");
  AfficheListe(liste2);                    
  printf("\n");

  liste1 = ListeVide();
  liste1 = Union(liste1, liste2);
  printf("affichage attendu = 2 1 ----> ");
  AfficheListe(liste1);                    
  printf("\n");

  liste1 = ListeVide();
  liste1 = Cons(1, liste1);
  liste1 = Union(liste1, liste2);
  printf("affichage attendu = 2 1 ----> ");
  AfficheListe(liste1);                    
  printf("\n");

  liste1 = ListeVide();
  liste1 = Cons(3, liste1);
  liste1 = Union(liste1, liste2);
  printf("affichage attendu = 3 2 1 ----> ");
  AfficheListe(liste1);                    
  printf("\n");

  DetruitListe(liste2);
  printf("destruction liste effectuee\n");
  printf("%d erreurs\n", erreurs);
} /* main() */
#endif /* TEST_LISTE */
