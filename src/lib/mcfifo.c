/* $Id: mcfifo.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* 
   Librairie mcfifo :

   fonctions pour la gestion d'une liste fifo

   Michel Couprie 1996
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcfifo.h>

/* ==================================== */
Fifo * CreeFifoVide(
  int32_t taillemax)
/* ==================================== */
{
  Fifo * L = (Fifo *)calloc(1,sizeof(Fifo) + sizeof(int32_t) * taillemax); /* sic (+1) */
  if (L == NULL)
  {   
    fprintf(stderr, "CreeFifoVide() : malloc failed : %d bytes\n", 
            sizeof(Fifo) + sizeof(int32_t) * taillemax);
    return NULL;
  }
  L->Max = taillemax+1;
  L->In = 0;
  L->Out = 0;
  return L;
}

/* ==================================== */
void FifoFlush(
  Fifo * L)
/* ==================================== */
{
  L->In = 0;
  L->Out = 0;
}

/* ==================================== */
int32_t FifoVide(
  Fifo * L)
/* ==================================== */
{
  return (L->In == L->Out);
}

/* ==================================== */
int32_t FifoPop(
  Fifo * L)
/* ==================================== */
{
  int32_t V;
  if (L->In == L->Out)
  {
    fprintf(stderr, "erreur fifo vide\n");
    exit(1);
  }
  V = L->Pts[L->Out];
  L->Out = (L->Out + 1) % L->Max;
  return V;
}

/* ==================================== */
void FifoPushFirst(
  Fifo * L,
  int32_t V)
/* ==================================== */
{
  L->Out = (L->Out - 1) % L->Max;
  L->Pts[L->Out] = V;
  if (L->In == L->Out)
  {
    fprintf(stderr, "erreur fifo pleine\n");
    exit(1);
  }
}
  
/* ==================================== */
void FifoPush(
  Fifo * L,
  int32_t V)
/* ==================================== */
{
  L->Pts[L->In] = V;
  L->In = (L->In + 1) % L->Max;
  if (L->In == L->Out)
  {
    fprintf(stderr, "erreur fifo pleine\n");
    exit(1);
  }
}

/* ==================================== */
void FifoPrint(
  Fifo * L)
/* ==================================== */
{
  int32_t i;
  if (FifoVide(L)) {printf("[]\n"); return;}
  printf("Taille Fifo: %d \n",FifoTaille(L));
  printf("Max = %d ; Out = %d ; In = %d\n", L->Max, L->Out, L->In);
  printf("[ ");
  for (i = L->Out; i != L->In; i = (i+1) % L->Max)
  printf("%d ", L->Pts[i]);
  printf("]\n");
}

/* ==================================== */
void FifoTermine(
  Fifo * L)
/* ==================================== */
{
  free(L);
}

/* ==================================== */
int32_t FifoTaille(
  Fifo * L)
/* ==================================== */
{
  if (L->In < L->Out)
     return (L->Max - (L->Out-L->In));
  else
     return (L->In - L->Out);
}

#ifdef TESTFIFO
void main()
{
  Fifo * L = CreeFifoVide(3);
  FifoPrint(L);
  if (FifoVide(L)) printf("FifoVide OUI\n");
  FifoPush(L,1);
  FifoPrint(L);
  if (!FifoVide(L)) printf("FifoVide NON\n");
  FifoPush(L,2);
  FifoPrint(L);
  FifoPush(L,3);
  FifoPrint(L);
  printf("FifoPop %d attendu 1\n", FifoPop(L));
  FifoPrint(L);
  FifoPushFirst(L,225);
  FifoPrint(L);
  printf("FifoPop %d attendu par christophe 225\n", FifoPop(L));
  FifoPrint(L);
  FifoPush(L,4);
  FifoPrint(L);
  printf("FifoPop %d attendu 2\n", FifoPop(L));
  FifoPrint(L);
  printf("FifoPop %d attendu 3\n", FifoPop(L));
  FifoPrint(L);
  printf("FifoPop %d attendu 4\n", FifoPop(L));
  FifoPrint(L);
  if (FifoVide(L)) printf("FifoVide OUI\n");
}
#endif


