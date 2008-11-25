/* $Id: mclifo.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* 
   Librairie mclifo :

   fonctions pour la gestion d'une liste lifo

   Michel Couprie 1996
*/

/* #define TESTLifo */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mclifo.h>

/* ==================================== */
Lifo * CreeLifoVide(
  int32_t taillemax)
/* ==================================== */
{
  Lifo * L = (Lifo *)calloc(1,sizeof(Lifo) + sizeof(int32_t) * (taillemax-1));
  if (L == NULL)
  {   
    fprintf(stderr, "CreeLifoVide() : malloc failed : %d bytes\n", 
            sizeof(Lifo) + sizeof(int32_t) * (taillemax-1));
    return NULL;
  }
  L->Max = taillemax;
  L->Sp = 0;
  return L;
}

/* ==================================== */
void LifoFlush(
  Lifo * L)
/* ==================================== */
{
  L->Sp = 0;
}

/* ==================================== */
int32_t LifoVide(
  Lifo * L)
/* ==================================== */
{
  return (L->Sp == 0);
}

/* ==================================== */
int32_t LifoPop(
  Lifo * L)
/* ==================================== */
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "erreur Lifo vide\n");
    exit(1);
  }
  L->Sp -= 1;
  return L->Pts[L->Sp];
}

/* ==================================== */
int32_t LifoHead(
  Lifo * L)
/* ==================================== */
{
  if (L->Sp == 0)
  {
    fprintf(stderr, "erreur Lifo vide\n");
    exit(1);
  }
  return L->Pts[L->Sp-1];
}
  
/* ==================================== */
void LifoPush(Lifo * L, int32_t V)
/* ==================================== */
{
  if (L->Sp > L->Max - 1)
  {
    fprintf(stderr, "erreur Lifo pleine\n");
    exit(1);
  }
  L->Pts[L->Sp] = V;
  L->Sp += 1;
}

/* ==================================== */
void LifoPrint(Lifo * L)
/* ==================================== */
{
  int32_t i;
  if (LifoVide(L)) {printf("[]"); return;}
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]");
}

/* ==================================== */
void LifoPrintLine(Lifo * L)
/* ==================================== */
{
  int32_t i;
  if (LifoVide(L)) {printf("[]\n"); return;}
/*
  printf("Max = %d ; Sp = %d \n", L->Max, L->Sp);
*/
  printf("[ ");
  for (i = 0; i < L->Sp; i++)
    printf("%d ", L->Pts[i]);
  printf("]\n");
}

/* ==================================== */
void LifoTermine(
  Lifo * L)
/* ==================================== */
{
  free(L);
}

#ifdef TESTLifo
void main()
{
  Lifo * L = CreeLifoVide(3);
  LifoPrint(L);
  if (LifoVide(L)) printf("LifoVide OUI\n");
  LifoPush(L,1);
  LifoPrint(L);
  if (!LifoVide(L)) printf("LifoVide NON\n");
  LifoPush(L,2);
  LifoPrint(L);
  LifoPush(L,3);
  LifoPrint(L);
  printf("LifoPop %d attendu 3\n", LifoPop(L));
  LifoPrint(L);
  LifoPush(L,4);
  LifoPrint(L);
  printf("LifoPop %d attendu 4\n", LifoPop(L));
  LifoPrint(L);
  printf("LifoPop %d attendu 2\n", LifoPop(L));
  LifoPrint(L);
  printf("LifoPop %d attendu 1\n", LifoPop(L));
  LifoPrint(L);
  if (LifoVide(L)) printf("LifoVide OUI\n");
  printf("maintenant sortie attendue sur lifo pleine :\n");
  LifoPush(L,3);
  LifoPush(L,3);
  LifoPush(L,3);
  LifoPush(L,3);  
}
#endif

