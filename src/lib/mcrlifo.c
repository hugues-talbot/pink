/* $Id: mcrlifo.c,v 1.1 2009-09-02 14:23:36 mcouprie Exp $ */
/* 
   Librairie mcrlifo :

   fonctions pour la gestion d'une liste lifo
   avec réallocation en cas de répassement

   Michel Couprie 2009
*/

/* #define TESTRlifo */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcrlifo.h>

/* ==================================== */
Rlifo * CreeRlifoVide(int32_t taillemax)
/* ==================================== */
#undef F_NAME
#define F_NAME "CreeRlifoVide" 
{
  Rlifo * L = (Rlifo *)calloc(1,sizeof(Rlifo) + sizeof(int32_t) * (taillemax-1));
  if (L == NULL)
  {   
    fprintf(stderr, "%s: malloc failed : %d bytes\n", F_NAME, 
            sizeof(Rlifo) + sizeof(int32_t) * (taillemax-1));
    return NULL;
  }
  L->Max = taillemax;
  L->Sp = 0;
  return L;
}

/* ==================================== */
void RlifoFlush(Rlifo * L)
/* ==================================== */
{
  L->Sp = 0;
}

/* ==================================== */
int32_t RlifoVide(Rlifo * L)
/* ==================================== */
{
  return (L->Sp == 0);
}

/* ==================================== */
int32_t RlifoPop(Rlifo * L)
/* ==================================== */
#undef F_NAME
#define F_NAME "RlifoPop" 
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "%s: empty stack\n", F_NAME);
    exit(1);
  }
  L->Sp -= 1;
  return L->Pts[L->Sp];
}

/* ==================================== */
int32_t RlifoHead(Rlifo * L)
/* ==================================== */
#undef F_NAME
#define F_NAME "RlifoHead" 
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "%s: empty stack\n", F_NAME);
    exit(1);
  }
  return L->Pts[L->Sp-1];
}

/* ==================================== */
void RlifoPush(Rlifo ** L, int32_t V)
/* ==================================== */
#undef F_NAME
#define F_NAME "RlifoPush" 
{
  if ((*L)->Sp > (*L)->Max - 1)
  {
    int32_t newsize = (*L)->Max + (*L)->Max;
    (*L)->Max = newsize;
    (*L) = (Rlifo *)realloc((*L), sizeof(Rlifo) + sizeof(int32_t) * (newsize - 1));
    if ((*L) == NULL)
    {
      fprintf(stderr, "%s: realloc failed : %d bytes\n", F_NAME, 
	      sizeof(Rlifo) + sizeof(int32_t) * (newsize-1));
      exit(1);
    }
  }
  (*L)->Pts[(*L)->Sp] = V;
  (*L)->Sp += 1;
}

/* ==================================== */
void RlifoPrint(Rlifo * L)
/* ==================================== */
{
  int32_t i;
  if (RlifoVide(L)) {printf("[]"); return;}
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]");
}

/* ==================================== */
void RlifoPrintLine(Rlifo * L)
/* ==================================== */
{
  int32_t i;
  if (RlifoVide(L)) {printf("[]\n"); return;}
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]\n");
}

/* ==================================== */
void RlifoTermine(Rlifo * L)
/* ==================================== */
{
  free(L);
}

#ifdef TESTRlifo
void main()
{
  Rlifo * L = CreeRlifoVide(3);
  RlifoPrint(L);
  if (RlifoVide(L)) printf("RlifoVide OUI\n");
  RlifoPush(L,1);
  RlifoPrint(L);
  if (!RlifoVide(L)) printf("RlifoVide NON\n");
  RlifoPush(L,2);
  RlifoPrint(L);
  RlifoPush(L,3);
  RlifoPrint(L);
  printf("RlifoPop %d attendu 3\n", RlifoPop(L));
  RlifoPrint(L);
  RlifoPush(L,4);
  RlifoPrint(L);
  printf("RlifoPop %d attendu 4\n", RlifoPop(L));
  RlifoPrint(L);
  printf("RlifoPop %d attendu 2\n", RlifoPop(L));
  RlifoPrint(L);
  printf("RlifoPop %d attendu 1\n", RlifoPop(L));
  RlifoPrint(L);
  if (RlifoVide(L)) printf("RlifoVide OUI\n");
  printf("maintenant sortie attendue sur rlifo pleine :\n");
  RlifoPush(L,3);
  RlifoPush(L,3);
  RlifoPush(L,3);
  RlifoPush(L,3);  
}
#endif

