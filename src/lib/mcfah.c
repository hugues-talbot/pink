/* $Id: mcfah.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* 
   Librairie mcfah :

   fonctions pour la gestion d'une file d'attente hierarchique

   Michel Couprie 1996
*/

/* un point d'une image F est reperee par son index */
/* dans le tableau mono-dimension ou les valeurs sont rangees */
/* par ligne, puis par colonne */

/* d'apres F. Meyer: "Un Algorithme Optimal de Ligne de Partage des Eaux" */

/* traitement des points "attardes" (ceux dont le niveau d'immersion a */
/* ete depasse: on utilise la variante suggeree par F. Meyer consistant */
/* a gerer une file speciale pour ces cas d'urgence, file plus prioritaire */
/* que toutes */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcfah.h>

/* #define TESTFah */
/* #define VERBOSE */

/* ==================================== */
Fah * CreeFahVide(
  int32_t taillemax)
/* ==================================== */
{
  int32_t i;
  Fah * L = (Fah *)calloc(1,4 * sizeof(int32_t) + 
                          (NPRIO+4)*sizeof(FahElt *) + 
                          taillemax*sizeof(FahElt));
  if (L == NULL)
  {   fprintf(stderr, "CreeFahVide() : malloc failed\n");
      return NULL;
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
  L->QueueUrg = NULL;
  L->TeteUrg = NULL;
  return L;
} /* CreeFahVide() */

/* ==================================== */
void FahFlush(
  Fah * L)
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
  L->QueueUrg = NULL;
  L->TeteUrg = NULL;
} /* FahFlush() */

/* ==================================== */
int32_t FahVide(
  Fah * L)
/* ==================================== */
{
  return ((L->QueueUrg == NULL) && (L->Queue == NULL));
} /* FahVide() */

/* ==================================== */
int32_t FahVideUrg(
  Fah * L)
/* ==================================== */
{
  return (L->QueueUrg == NULL);
} /* FahVideUrg() */

/* ==================================== */
int32_t FahVideNiveau(
  Fah * L,
  int32_t niv)
/* ==================================== */
{
  printf("FahVideNiveau: pas de sens avec la version impure de mcfah\n");
  exit(0);
} /* FahVideNiveau() */

/* ==================================== */
int32_t FahNiveau(
  Fah * L)
/* ==================================== */
{
  if ((L->QueueUrg == NULL) && (L->Queue == NULL))
  {
    fprintf(stderr, "erreur Fah vide\n");
    exit(1);
  }
  return L->Niv;
} /* FahNiveau() */

/* ==================================== */
int32_t FahPop(
  Fah * L)
/* ==================================== */
{
  int32_t V;
  FahElt * FE;
  if ((L->QueueUrg == NULL) && (L->Queue == NULL))
  {
    fprintf(stderr, "erreur Fah vide\n");
    exit(1);
  }

  L->Util--;
  if (L->QueueUrg != NULL)
  {
    V = L->QueueUrg->Point;
    FE = L->QueueUrg->Next;
    L->QueueUrg->Next = L->Libre;
    L->Libre = L->QueueUrg;
    L->QueueUrg = FE;
    if (L->QueueUrg == NULL) L->TeteUrg = NULL;
    return V;
  }

  if (L->Queue == L->Tete[L->Niv]) /* on se prepare a effacer le dernier */
  {                                /* element du niveau courant: il faut */
    L->Tete[L->Niv] = NULL;        /* annuler le pointeur de tete */
    do (L->Niv)++;                 /* et incrementer le niveau */
    while ((L->Niv < NPRIO)          
            && (L->Tete[L->Niv] == NULL));
  }
  V = L->Queue->Point;
  FE = L->Queue->Next;
  L->Queue->Next = L->Libre;
  L->Libre = L->Queue;
  L->Queue = FE;
  return V;
} /* FahPop() */
  
/* ==================================== */
int32_t FahFirst(
  Fah * L)
/* ==================================== */
{
  if ((L->QueueUrg == NULL) && (L->Queue == NULL))
  {
    fprintf(stderr, "erreur Fah vide\n");
    exit(1);
  }

  if (L->QueueUrg != NULL) return L->QueueUrg->Point;
  return L->Queue->Point;
} /* FahFirst() */
  
/* ==================================== */
void FahPush(
  Fah * L,
  int32_t Po,
  int32_t Ni)
/* ==================================== */
{
  if (L->Libre == NULL)
  {
    fprintf(stderr, "erreur Fah pleine\n");
    exit(1);
  }
  if (Ni >= NPRIO)
  {
    fprintf(stderr, "erreur niveau = %d; max autorise = %d\n", Ni, NPRIO-1);
    exit(1);
  }

  L->Util++;
  if (L->Util > L->Maxutil) L->Maxutil = L->Util;
  if (L->Queue == NULL)    /* insertion dans une Fah vide */
  {
    L->Queue = L->Libre;
    L->Libre = L->Libre->Next;
    L->Queue->Point = Po;
    L->Niv = Ni;
    L->Queue->Next = NULL;
    L->Tete[Ni] = L->Queue;
  } /* if Fah Vide */
  else if (Ni < L->Niv)         /* si niveau depasse alors liste d'urgence */
  {
    if (L->QueueUrg == NULL)    /* insertion dans une liste d'urgence vide */
    {
      L->QueueUrg = L->Libre;
      L->Libre = L->Libre->Next;
      L->QueueUrg->Point = Po;
      L->QueueUrg->Next = NULL;
      L->TeteUrg = L->QueueUrg;
    } /* if L->QueueUrg */
    else
    {
      FahElt * FE = L->TeteUrg->Next;
      L->TeteUrg->Next = L->Libre;
      L->Libre = L->Libre->Next;
      L->TeteUrg = L->TeteUrg->Next;
      L->TeteUrg->Next = FE;
      L->TeteUrg->Point = Po;      
    }
  } /* fin traitement des cas d'urgence */
  else if (L->Tete[Ni] != NULL)   /* insertion dans la liste de niveau Ni */
  {
    FahElt * FE = L->Tete[Ni]->Next;
    L->Tete[Ni]->Next = L->Libre;
    L->Libre = L->Libre->Next;
    L->Tete[Ni] = L->Tete[Ni]->Next;
    L->Tete[Ni]->Next = FE;
    L->Tete[Ni]->Point = Po;      
  }
  else /* (L->Tete[Ni] == NULL) */
  {
    FahElt * FE;
    int32_t NiPrec = Ni;
    while ((NiPrec >= 0) && (L->Tete[NiPrec] == NULL)) NiPrec--; 
    if (NiPrec < 0)
    {
      fprintf(stderr, "erreur Fah pas de niveau precedent\n");
      exit(1);
    }
    L->Tete[Ni] = L->Libre;
    L->Libre = L->Libre->Next;
    L->Tete[Ni]->Point = Po;
    FE = L->Tete[NiPrec]->Next;
    L->Tete[NiPrec]->Next = L->Tete[Ni];
    L->Tete[Ni]->Next = FE;
  }
} /* FahPush() */

/* ==================================== */
void FahTermine(
  Fah * L)
/* ==================================== */
{
#ifdef VERBOSE
  printf("Fah: taux d'utilisation: %g\n", (double)L->Maxutil / (double)L->Max);
#endif
  free(L);
} /* FahTermine() */

/* ==================================== */
void FahPrint(
  Fah * L)
/* ==================================== */
{
  int32_t i;
  FahElt * FE;
  if (FahVide(L)) {printf("[]\n"); return;}
  FE = L->QueueUrg;
  if (FE != NULL)
  {
    printf("Urg [ ");
    for (;FE != NULL; FE = FE->Next)
      printf("%d ", FE->Point);
    printf(" ]\n");
  }
  FE = L->Queue;
  for (i = L->Niv; i < NPRIO; i++)
    if (L->Tete[i] != NULL) 
    {
      printf("%d [ ", i);
      for (;FE != L->Tete[i]; FE = FE->Next)
        printf("%d ", FE->Point);
      printf("%d ]\n", FE->Point);
      FE = FE->Next;
    }
} /* FahPrint() */

#ifdef TESTFah
void main()
{
  Fah * L = CreeFahVide(5);
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
        FahPush(L, p, n);
        break;
      case 'o': 
        printf("pop: %d\n", FahPop(L));
        break;
      case 'p': FahPrint(L); break;
      case 'v': 
        printf("vide: %d\n", FahVide(L));
        break;
      case 'q': break;
    }
  } while (r[0] != 'q');
  FahTermine(L);
}
#endif


