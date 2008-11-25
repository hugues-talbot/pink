/* $Id: mcfahsalembier.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* structure de file d'attente hierarchique de points d'image */
/* Michel Couprie */

/* un point d'une image F est reperee par son index */
/* dans le tableau mono-dimension ou les valeurs sont rangees */
/* par ligne, puis par colonne */

/* d'apres F. Meyer: "Un Algorithme Optimal de Ligne de Partage des Eaux" */
/* version "salembier" (un point insere sous le niveau courant recree une file a ce niveau, 
   de plus on peut retirer un point de n'importe quel niveau a tout moment) */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcfahsalembier.h>

/*
#define TESTFah
#define VERBOSE
*/

/* ==================================== */
Fah * CreeFahVide(
  int32_t taillemax)
/* ==================================== */
{
  int32_t i;
  Fah * L = (Fah *)calloc(1,sizeof(Fah) + (taillemax - 1) * sizeof(FahElt));
  if (L == NULL)
  {
    fprintf(stderr, "erreur allocation Fah\n");
    exit(1);
  }
  L->Max = taillemax;
  L->Util = 0;
  L->Maxutil = 0;
  L->Niv = 0;
  for (i = 0; i < taillemax - 1; i++) L->Elts[i].Next = &(L->Elts[i+1]);
  L->Elts[taillemax - 1].Next = NULL;
  L->Libre = &(L->Elts[0]);
  for (i = 0; i < NPRIO; i++) L->Tete[i]= NULL;
  for (i = 0; i < NPRIO; i++) L->Queue[i]= NULL;
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
  for (i = 0; i < L->Max - 1; i++) L->Elts[i].Next = &(L->Elts[i+1]);
  L->Elts[L->Max - 1].Next = NULL;
  L->Libre = &(L->Elts[0]);
  for (i = 0; i < NPRIO; i++) L->Tete[i]= NULL;
  for (i = 0; i < NPRIO; i++) L->Queue[i]= NULL;
} /* FahFlush() */

/* ==================================== */
int32_t FahVide(
  Fah * L)
/* ==================================== */
{
  if (L->Util == 0) return 1;
  return 0;
} /* FahVide() */

/* ==================================== */
int32_t FahVideNiveau(
  Fah * L,
  int32_t niv)
/* ==================================== */
{
  if (L->Queue[niv] == NULL) return 1;
  return 0;
} /* FahVideNiveau() */

/* ==================================== */
int32_t FahPopNiveau(
  Fah * L, 
  int32_t niv)
/* ==================================== */
{
  int32_t V;
  FahElt * FE;
  if (L->Queue[niv] == NULL)
  {
    fprintf(stderr, "erreur Fah vide au niveau %d\n", niv);
    exit(1);
  }

  L->Util--;
  V = L->Queue[niv]->Point;
  FE = L->Queue[niv]->Prev;

  L->Queue[niv]->Next = L->Libre; /* recupere la cellule dans la liste libre */
  L->Libre = L->Queue[niv];
  L->Queue[niv] = FE;

  if (FE == NULL) /* plus aucun element dans la liste */
  {
    L->Tete[niv] = NULL;
    if (niv == L->Niv)
      do (L->Niv)++;                 /* incrementer le niveau */
      while ((L->Niv < NPRIO)          
              && (L->Tete[L->Niv] == NULL));
  }
  else if (L->Tete[niv] == L->Queue[niv]) /* seul un element reste dans la liste */
  {
    L->Tete[niv]->Next = L->Tete[niv]->Prev = NULL;
  }

  return V;
} /* FahPopNiveau() */

/* ==================================== */
int32_t FahPop(
  Fah * L)
/* ==================================== */
{
  if (L->Util == 0)
  {
    fprintf(stderr, "erreur Fah vide\n");
    exit(1);
  }
  return FahPopNiveau(L, L->Niv);
} /* FahPop() */
  
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

  if ((L->Util == 0) || (Ni < L->Niv)) L->Niv = Ni;

  L->Util++;
  if (L->Util > L->Maxutil) L->Maxutil = L->Util;
  if (L->Tete[Ni] != NULL)   /* insertion dans la liste de niveau Ni non vide */
  {
    FahElt * FE = L->Tete[Ni];
    L->Tete[Ni] = L->Libre;
    L->Libre = L->Libre->Next;
    L->Tete[Ni]->Next = FE;
    L->Tete[Ni]->Prev = NULL;
    L->Tete[Ni]->Point = Po;      
    FE->Prev = L->Tete[Ni];
  }
  else /* (L->Tete[Ni] == NULL) */
  {
    L->Tete[Ni] = L->Queue[Ni] = L->Libre;
    L->Libre = L->Libre->Next;
    L->Tete[Ni]->Next = NULL;
    L->Tete[Ni]->Prev = NULL;
    L->Tete[Ni]->Point = Po;      
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
  printf("niveau courant = %d\n", L->Niv);
  for (i = 0; i < NPRIO; i++)
    if (L->Tete[i] != NULL) 
    {
      printf("%d [ ", i);
      for (FE = L->Tete[i]; FE != NULL; FE = FE->Next)
        printf("%d ", FE->Point);
      printf("]\n");
    }  
} /* FahPrint() */

#ifdef TESTFAH
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
