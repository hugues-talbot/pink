/* $Id: mcmedax.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* 
Librairie mcmedax : 

Fonctions pour l'axe median euclidien

D'après: 
E. Remy and E. Thiel.
Look-Up Tables for Medial Axis on Squared Euclidean Distance Transform. 
In 11th DGCI, Discrete Geometry for Computer Image, 
volume 2886 of Lectures Notes in Computer Science, pages 224-235, Naples, Italy, Nov 2003.

Michel Couprie, février 2004
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
//#include <mcmedax.h>

#define MAXDIM 100
typedef int32_t TAB2D[MAXDIM][MAXDIM];
typedef int32_t TAB1D[MAXDIM];

void CompCTg2D(int32_t L, TAB2D CTg)
{
  int32_t x, y;
  for (x = 0; x < L; x++)
    for (y = 0; y <= x; y++)
      CTg[x][y] = x*x + y*y;
}

void print2D(int32_t L, TAB2D T)
{
  int32_t x, y;
  for (x = 0; x < L; x++)
  {
    for (y = 0; y < L; y++)
      printf("%8d ", T[x][y]);
    printf("\n");
  }
}

void print1D(int32_t L, TAB1D T)
{
  int32_t x;
  for (x = 0; x < L; x++)
    printf("%8d ", T[x]);
  printf("\n");
}

void CompLutCol2D(TAB2D CTg, int32_t L, int32_t vgx, int32_t vgy, int32_t Rmax, TAB1D Lut)
{
  int32_t r, r1, r2, x, y;
  for (r = 0; r <= Rmax; r++) Lut[r] = 0;
  for (x = 0; x < L-vgx; x++)
    for (y = 0; y <= x; y++)
    {
      r1 = CTg[x][y] + 1;
      r2 = CTg[x+vgx][y+vgy] + 1;
      if ((r1 <= Rmax) && (r2 > Lut[r1])) Lut[r1] = r2;
    }
}

int32_t main()
{
  TAB2D CTg;
  TAB1D Lut;
  CompCTg2D(100, CTg);
  //  print2D(10, CTg);
  CompLutCol2D(CTg, 100, 1, 1, 20, Lut); 
  print1D(21, Lut);
  return 0;
}
