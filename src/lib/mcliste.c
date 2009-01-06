/* $Id: mcliste.c,v 1.2 2009-01-06 13:18:15 mcouprie Exp $ */
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
#undef F_NAME
#define F_NAME "CreeListeVide"
{
  Liste * L = (Liste *)calloc(1,sizeof(Liste) + sizeof(int32_t) * (taillemax-1));
  if (L == NULL)
  {
    fprintf(stderr, "%s: erreur calloc\n", F_NAME);
    exit(1);
  }
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
int32_t ListeTaille(Liste * L)
/* ==================================== */
{
  return (L->Sp);
}

/* ==================================== */
int32_t ListeVide(Liste * L)
/* ==================================== */
{
  return (L->Sp == 0);
}

/* ==================================== */
int32_t ListeElt(Liste * L, uint32_t n)
/* ==================================== */
#undef F_NAME
#define F_NAME "ListeElt"
{
  if (n >= L->Sp)
  {
    fprintf(stderr, "%s: erreur hors limite\n", F_NAME);
    exit(1);
  }
  return L->Pts[n];
}

/* ==================================== */
int32_t ListePop(Liste * L)
/* ==================================== */
#undef F_NAME
#define F_NAME "ListePop"
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "%s: erreur Liste vide\n", F_NAME);
    exit(1);
  }
  L->Sp -= 1;
  return L->Pts[L->Sp];
}
  
/* ==================================== */
int32_t ListePush(Liste * L, int32_t V)
/* ==================================== */
#undef F_NAME
#define F_NAME "ListePush"
{
  if (L->Sp > L->Max - 1)
  {
    fprintf(stderr, "%s: erreur Liste pleine\n", F_NAME);
    exit(1);
  }
  L->Pts[L->Sp] = V;
  L->Sp += 1;
  return L->Sp - 1; 
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

