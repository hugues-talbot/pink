/* $Id: mcliste.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* structure de liste d'entiers */

/* #define TESTListe */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcliste.h>

/* ==================================== */
Liste * CreeListeVide(int32_t taillemax)
/* ==================================== */
{
  Liste * L = (Liste *)calloc(1,sizeof(Liste) + sizeof(int32_t) * (taillemax-1));
  L->Max = taillemax;
  L->Sp = 0;
  return L;
}

/* ==================================== */
void ListeFlush(Liste * L)
/* ==================================== */
{
  L->Sp = 0;
}

/* ==================================== */
int32_t ListeVide(Liste * L)
/* ==================================== */
{
  return (L->Sp == 0);
}

/* ==================================== */
int32_t ListePop(Liste * L)
/* ==================================== */
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "erreur Liste vide\n");
    exit(1);
  }
  L->Sp -= 1;
  return L->Pts[L->Sp];
}
  
/* ==================================== */
void ListePush(Liste * L, int32_t V)
/* ==================================== */
{
  if (L->Sp > L->Max - 1)
  {
    fprintf(stderr, "erreur Liste pleine\n");
    exit(1);
  }
  L->Pts[L->Sp] = V;
  L->Sp += 1;
}

/* ==================================== */
int32_t ListeIn(Liste * L, int32_t e)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < L->Sp; i++)
    if (L->Pts[i] == e) return 1;
  return 0;
}

/* ==================================== */
void ListePrint(Liste * L)
/* ==================================== */
{
  int32_t i;
  if (ListeVide(L)) {printf("[]"); return;}
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]");
}

/* ==================================== */
void ListePrintLine(Liste * L)
/* ==================================== */
{
  int32_t i;
  if (ListeVide(L)) {printf("[]\n"); return;}
/*
  printf("Max = %d ; Sp = %d \n", L->Max, L->Sp);
*/
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]\n");
}

/* ==================================== */
void ListeTermine(Liste * L)
/* ==================================== */
{
  free(L);
}

#ifdef TESTListe
void main()
{
  Liste * L = CreeListeVide(3);
  ListePrint(L);
  if (ListeVide(L)) printf("ListeVide OUI\n");
  ListePush(L,1);
  ListePrint(L);
  if (!ListeVide(L)) printf("ListeVide NON\n");
  ListePush(L,2);
  ListePrint(L);
  ListePush(L,3);
  ListePrint(L);
  printf("ListePop %d attendu 3\n", ListePop(L));
  ListePrint(L);
  ListePush(L,4);
  ListePrint(L);
  printf("ListePop %d attendu 4\n", ListePop(L));
  ListePrint(L);
  printf("ListePop %d attendu 2\n", ListePop(L));
  ListePrint(L);
  printf("ListePop %d attendu 1\n", ListePop(L));
  ListePrint(L);
  if (ListeVide(L)) printf("ListeVide OUI\n");
  printf("maintenant sortie attendue sur liste pleine :\n");
  ListePush(L,3);
  ListePush(L,3);
  ListePush(L,3);
  ListePush(L,3);  
}
#endif

