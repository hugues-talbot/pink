/* $Id: mcfahdouble.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* 
   Librairie mcfahdouble :

   fonctions pour la gestion d'une file d'attente hierarchique
   politique "pure" priorite

   Michel Couprie 1999
*/

/* d'apres F. Meyer: "Un Algorithme Optimal de Ligne de Partage des Eaux" */

/* traitement des points "attardes" (ceux dont le niveau d'immersion a */
/* ete depasse: on utilise la variante suggeree par F. Meyer consistant */
/* a gerer une file speciale pour ces cas d'urgence, file plus prioritaire */
/* que toutes */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcfahdouble.h>

/* #define TESTFahd */
/* #define VERBOSE */

/* ==================================== */
Fahd * CreeFahdVide(
  int32_t taillemax)
/* ==================================== */
{
  int32_t i;
  Fahd * L = (Fahd *)calloc(1,4 * sizeof(int32_t) + 
                          (NPRIO+4)*sizeof(FahdElt *) + 
                          taillemax*sizeof(FahdElt));
  if (L == NULL)
  {
    fprintf(stderr, "erreur allocation Fahd\n");
    exit(1);
  }
  L->Max = taillemax;
  L->Util = 0;
  L->Maxutil = 0;
  L->Niv = 0;
  for (i = 0; i < taillemax - 1; i++)
    L->Elts[i].Next = &(L->Elts[i+1]);
  L->Elts[taillemax - 1].Next = NULL;
  L->Libre = &(L->Elts[0]);
  for (i = 0; i < NPRIO; i++)
    L->Tete[i]= NULL;
  L->Queue = NULL;
  return L;
} /* CreeFahdVide() */

/* ==================================== */
void FahdFlush(
  Fahd * L)
/* ==================================== */
{
  int32_t i;
  L->Niv = 0;
  L->Util = 0;
  for (i = 0; i < L->Max - 1; i++)
    L->Elts[i].Next = &(L->Elts[i+1]);
  L->Elts[L->Max - 1].Next = NULL;
  L->Libre = &(L->Elts[0]);
  for (i = 0; i < NPRIO; i++)
    L->Tete[i]= NULL;
  L->Queue = NULL;
} /* FahdFlush() */

/* ==================================== */
int32_t FahdVide(
  Fahd * L)
/* ==================================== */
{
  return (L->Queue == NULL);
} /* FahdVide() */

/* ==================================== */
int32_t FahdVideNiveau(
  Fahd * L,
  int32_t niv)
/* ==================================== */
{
  if (L->Queue == NULL) return 1;
  if (L->Niv > niv) return 1;
  return 0;
} /* FahdVideNiveau() */

/* ==================================== */
int32_t FahdNiveau(
  Fahd * L)
/* ==================================== */
{
  if (L->Queue == NULL)
  {
    fprintf(stderr, "erreur Fahd vide\n");
    exit(1);
  }
  return L->Niv;
} /* FahdNiveau() */

/* ==================================== */
void FahdPop(
  Fahd * L,
  int32_t *P1,
  int32_t *P2)
/* ==================================== */
{
  FahdElt * FE;
  if (L->Queue == NULL)
  {
    fprintf(stderr, "erreur Fahd vide\n");
    exit(1);
  }

  L->Util--;

  if (L->Queue == L->Tete[L->Niv]) /* on se prepare a effacer le dernier */
  {                                /* element du niveau courant: il faut */
    L->Tete[L->Niv] = NULL;        /* annuler le pointeur de tete */
    do (L->Niv)++;                 /* et incrementer le niveau */
    while ((L->Niv < NPRIO)          
            && (L->Tete[L->Niv] == NULL));
  }
  *P1 = L->Queue->Point1;
  *P2 = L->Queue->Point2;
  FE = L->Queue->Next;
  L->Queue->Next = L->Libre;
  L->Libre = L->Queue;
  L->Queue = FE;
} /* FahdPop() */
  
/* ==================================== */
void FahdFirst(
  Fahd * L,
  int32_t *P1,
  int32_t *P2)
/* ==================================== */
{
  FahdElt * FE;
  if (L->Queue == NULL)
  {
    fprintf(stderr, "erreur Fahd vide\n");
    exit(1);
  }
  *P1 = L->Queue->Point1;
  *P2 = L->Queue->Point2;
} /* FahdFirst() */
  
/* ==================================== */
void FahdPush(
  Fahd * L,
  int32_t P1,
  int32_t P2,
  int32_t Ni)
/* ==================================== */
{
  if (L->Libre == NULL)
  {
    fprintf(stderr, "erreur Fahd pleine\n");
    exit(1);
  }
  if (Ni >= NPRIO)
  {
    fprintf(stderr, "erreur niveau = %d; max autorise = %d\n", Ni, NPRIO-1);
    exit(1);
  }

  L->Util++;
  if (L->Util > L->Maxutil) L->Maxutil = L->Util;
  if (L->Queue == NULL)    /* insertion dans une Fahd vide */
  {
    L->Queue = L->Libre;
    L->Libre = L->Libre->Next;
    L->Queue->Point1 = P1;
    L->Queue->Point2 = P2;
    L->Niv = Ni;
    L->Queue->Next = NULL;
    L->Tete[Ni] = L->Queue;
  } /* if Fahd Vide */
  else if (L->Tete[Ni] != NULL)   /* insertion dans la liste de niveau Ni non vide */
  {
    FahdElt * FE = L->Tete[Ni]->Next;
    L->Tete[Ni]->Next = L->Libre;
    L->Libre = L->Libre->Next;
    L->Tete[Ni] = L->Tete[Ni]->Next;
    L->Tete[Ni]->Next = FE;
    L->Tete[Ni]->Point1 = P1;      
    L->Tete[Ni]->Point2 = P2;
  }
  else /* (L->Tete[Ni] == NULL) */
  {
    FahdElt * FE;
    int32_t NiPrec = Ni;
    while ((NiPrec >= 0) && (L->Tete[NiPrec] == NULL)) NiPrec--; 
    if (NiPrec < 0)              /* creation d'un niveau inferieur au niveau courant */
    {
      FE = L->Queue;
      L->Queue = L->Libre;
      L->Libre = L->Libre->Next;
      L->Queue->Point1 = P1;
      L->Queue->Point2 = P2;
      L->Niv = Ni;
      L->Queue->Next = FE;
      L->Tete[Ni] = L->Queue;
    }
    else
    {
      L->Tete[Ni] = L->Libre;
      L->Libre = L->Libre->Next;
      L->Tete[Ni]->Point1 = P1;
      L->Tete[Ni]->Point2 = P2;
      FE = L->Tete[NiPrec]->Next;
      L->Tete[NiPrec]->Next = L->Tete[Ni];
      L->Tete[Ni]->Next = FE;
    }
  }
} /* FahdPush() */

/* ==================================== */
void FahdTermine(
  Fahd * L)
/* ==================================== */
{
#ifdef VERBOSE
  printf("Fahd: taux d'utilisation: %g\n", (double)L->Maxutil / (double)L->Max);
#endif
  free(L);
} /* FahdTermine() */

/* ==================================== */
void FahdPrint(
  Fahd * L)
/* ==================================== */
{
  int32_t i;
  FahdElt * FE;
  if (FahdVide(L)) {printf("[]\n"); return;}
  FE = L->Queue;
  for (i = L->Niv; i < NPRIO; i++)
    if (L->Tete[i] != NULL) 
    {
      printf("%d [ ", i);
      for (;FE != L->Tete[i]; FE = FE->Next)
        printf("(%d,%d) ", FE->Point1, FE->Point2);
      printf("(%d,%d) ]\n", FE->Point1, FE->Point2);
      FE = FE->Next;
    }
} /* FahdPrint() */

#ifdef TESTFahd
void main()
{
  Fahd * L = CreeFahdVide(5);
  char r[80];
  int32_t p, n;

  do
  {
    printf("commande (qUIT, PuSH, PoP, pRINT, TESTvIDE) > ");
    scanf("%s", r);
    switch (r[0])
    {
      case 'u':
        printf("niveau > ");
        scanf("%d", &n);
        printf("valeur > ");
        scanf("%d", &p);
        FahdPush(L, p, n);
        break;
      case 'o': 
        printf("pop: %d\n", FahdPop(L));
        break;
      case 'p': FahdPrint(L); break;
      case 'v': 
        printf("vide: %d\n", FahdVide(L));
        break;
      case 'q': break;
    }
  } while (r[0] != 'q');
  FahdTermine(L);
}
#endif TESTFahd
