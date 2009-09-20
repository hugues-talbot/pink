/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/* 
   Algorithmes 3D "fully parallel" de squelettisation
   
   Algo. de Palagyi
*/

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lskelpar3d_others.h>

#define VERBOSE
//#define DEBUG
#ifdef DEBUG
int32_t trace = 1;
#endif

typedef uint32_t mat[3][3];
typedef uint32_t vec[3];

// matrices pour les 8 symétries de type 1
// et translations associées

mat R0 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0,  1}
};
vec T0 = {0,0,0};

mat R1 = {
  { -1,  0,  0},
  {  0,  1,  0},
  {  0,  0,  1}
};
vec T1 = {-4,0,0};

mat R2 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0,  1}
};
vec T2 = {0,-4,0};

mat R3 = {
  { -1,  0,  0},
  {  0, -1,  0},
  {  0,  0,  1}
};
vec T3 = {-4,-4,0};

mat R4 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};
vec T4 = {0,0,-4};

mat R5 = {
  { -1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};
vec T5 = {-4,0,-4};

mat R6 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0, -1}
};
vec T6 = {0,-4,-4};

mat R7 = {
  { -1,  0,  0},
  {  0, -1,  0},
  {  0,  0, -1}
};
vec T7 = {-4,-4,-4};

// matrices pour les 6 symétries de type 2

mat S0 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0,  1}
};

mat S1 = {
  {  1,  0,  0},
  {  0,  0,  1},
  {  0,  1,  0}
};

mat S2 = {
  {  0,  1,  0},
  {  1,  0,  0},
  {  0,  0,  1}
};

mat S3 = {
  {  0,  1,  0},
  {  0,  0,  1},
  {  1,  0,  0}
};

mat S4 = {
  {  0,  0,  1},
  {  1,  0,  0},
  {  0,  1,  0}
};

mat S5 = {
  {  0,  0,  1},
  {  0,  1,  0},
  {  1,  0,  0}
};

/* ==================================== */
static void multmatvec(
  mat m,
  vec v,
  vec sol)
/* ==================================== */
{
  int32_t i, j, t;
  for (i = 0; i < 3; i++)
  {
    t = 0;
    for (j = 0; j < 3; j++)  
      t += m[i][j] * v[j];
    sol[i] = t;    
  }          
} // multmatvec()

/* ==================================== */
static int32_t isometry_coord(mat s1, vec t1, mat s2, vec x, vec tmp)
/* ==================================== */
{
  multmatvec(s1,x,tmp);
  tmp[0] -= t1[0];
  tmp[1] -= t1[1];
  tmp[2] -= t1[2];
  multmatvec(s2,tmp,x);
  return x[2]*25 + x[1]*5 + x[0];
} // isometry_coord()

/* ==================================== */
static void isometry(uint8_t *dest, uint8_t *sour, mat s1, vec t1, mat s2)
/* ==================================== */
{
  vec x, tmp = {0,0,0};
  int32_t i, j, k, p;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
      {
	x[0] = i; x[1] = j; x[2] = k;
	p = isometry_coord(s1, t1, s2, x, tmp);
	dest[p] = sour[k*25 + j*5 + i];
      }
} // isometry()

/* ==================================== */
static void extract_vois(
  uint8_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)    
/* 
  retourne dans "vois" les valeurs des 125 voisins de p, dans l'ordre suivant: 

  0   1   2   3   4
  5   6   7   8   9
 10  11  12  13  14
 15  16  17  18  19
 20  21  22  23  24

 25  26  27  28  29
 30  31  32  33  34
 35  36  37  38  39
 40  41  42  43  44
 45  46  47  48  49

 50  51  52  53  54
 55  56  57  58  59
 60  61  62  63  64
 65  66  67  68  69
 70  71  72  73  74

 75  76  77  78  79
 80  81  82  83  84
 85  86  87  88  89
 90  91  92  93 ...

 le tableau vois doit avoir été alloué

*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois"
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-2) || (p%ps<rs-1) || (p%rs<=1) || (p%ps>=ps-rs-rs) || 
      (p < ps-1) || (p >= N-ps-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
      {
	vois[(k * 25) + (j * 5) + i] = 
	  img[((zz-2+k) * ps) + ((yy-2+j) * rs) + xx-2+i];
      }

} /* extract_vois() */

/* ==================================== */
int32_t pala_match1(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && 
      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] && 
      !v[91] && !v[92] && !v[93])
    return 1;
  else
    return 0;
} // palagyi_match1()

/* ==================================== */
int32_t pala_match2(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && 
      v[67] && (!v[61] || !v[63]) && !v[86] && !v[87] && !v[88] && 
      !v[81] && !v[82] && !v[83])
    return 1;
  else
    return 0;
} // palagyi_match2()

/* ==================================== */
int32_t pala_match3(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && 
      v[67] && v[61] && !v[57] && !v[63] && !v[87] && !v[88] && 
      !v[82] && !v[83])
    return 1;
  else
    return 0;
} // palagyi_match3()

/* ==================================== */
int32_t pala_match4(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && 
      v[61] && v[63] && v[67] && 
      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] && !v[92])
    return 1;
  else
    return 0;
} // palagyi_match4()

/* ==================================== */
int32_t pala_match5(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && 
      v[61] && v[63] && v[66] && v[67] && v[68] && 
      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88])
    return 1;
  else
    return 0;
} // palagyi_match5()

/* ==================================== */
int32_t pala_match6(uint8_t *v)
/* ==================================== */
{
  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && v[57] && v[67] && 
      !v[61] && !v[63] && !v[86] && !v[87] && !v[88])
    return 1;
  else
    return 0;
} // palagyi_match6()

/* ==================================== */
int32_t palagyi_match1(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match1(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match1(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match1(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match1(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match1(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match1(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match1(vv)) return 1;

  return 0;
} // palagyi_match1()

/* ==================================== */
int32_t palagyi_match2(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match2(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match2(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match2(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match2(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match2(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match2(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match2(vv)) return 1;

  return 0;
} // palagyi_match2()

/* ==================================== */
int32_t palagyi_match3(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match3(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match3(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match3(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match3(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match3(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match3(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match3(vv)) return 1;

  return 0;
} // palagyi_match3()

/* ==================================== */
int32_t palagyi_match4(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match4(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match4(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match4(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match4(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match4(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match4(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match4(vv)) return 1;

  return 0;
} // palagyi_match4()

/* ==================================== */
int32_t palagyi_match5(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match5(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match5(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match5(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match5(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match5(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match5(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match5(vv)) return 1;

  return 0;
} // palagyi_match5()

/* ==================================== */
int32_t palagyi_match6(uint8_t *v, uint8_t *vv)
/* ==================================== */
{
  isometry(vv, v, R0, T0, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S0); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S0); if (pala_match6(vv)) return 1;

  isometry(vv, v, R0, T0, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S1); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S1); if (pala_match6(vv)) return 1;

  isometry(vv, v, R0, T0, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S2); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S2); if (pala_match6(vv)) return 1;

  isometry(vv, v, R0, T0, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S3); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S3); if (pala_match6(vv)) return 1;

  isometry(vv, v, R0, T0, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S4); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S4); if (pala_match6(vv)) return 1;

  isometry(vv, v, R0, T0, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R1, T1, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R2, T2, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R3, T3, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R4, T4, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R5, T5, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R6, T6, S5); if (pala_match6(vv)) return 1;
  isometry(vv, v, R7, T7, S5); if (pala_match6(vv)) return 1;

  return 0;
} // palagyi_match6()

/* ==================================== */
int32_t lpalagyi(struct xvimage *image, 
		 int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyi"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  int32_t step, nonstab;
  uint8_t v[125];
  uint8_t vv[125];

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    for (k = 2; k < ds-2; k++)
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (T[x])
	  {
	    extract_vois(T, x, rs, ps, N, v);
	    if (palagyi_match1(v, vv) || palagyi_match2(v, vv) || 
		palagyi_match3(v, vv) || palagyi_match4(v, vv) || 
		palagyi_match5(v, vv) || palagyi_match6(v, vv))
	    {
	      S[x] = 0;
	      nonstab = 1;
	    }
	  }
	} // for k,j,i
    memcpy(T, S, N);
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  return(1);
} /* lpalagyi() */

#ifdef PROBE
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int32_t main() 
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i, j, k;
  int32_t rs, cs, ds, ps, N, valmax;
  uint8_t *Im;
  struct xvimage * kh;
  int32_t nbcc1, nbcav1, nbtun1, euler1;
  int32_t nbcc2, nbcav2, nbtun2, euler2;

  rs = 9;
  cs = 9;
  ds = 9;
  ps = rs * cs;
  N = ps * ds;
  valmax = 5;
  srand(time(NULL));
  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  Im = UCHARDATA(image);
  memset(Im, 0, N);

  while(1)
  {
    for (k = 2; k < 7; k++)
    for (j = 2; j < 7; j++)
    for (i = 2; i < 7; i++)
    {
      Im[k*ps + j*rs + i] = (int8_t)((valmax+1.0)*rand() / (RAND_MAX + 1.0));
    }
    l3dkhalimskize(image, &kh, 0); 
    l3dinvariants(kh, &nbcc1, &nbcav1, &nbtun1, &euler1);
    freeimage(kh);
    lpalagyi(image, -1);
    l3dkhalimskize(image, &kh, 0); 
    l3dinvariants(kh, &nbcc2, &nbcav2, &nbtun2, &euler2);
    freeimage(kh);
    if ((nbcc1 != nbcc2) || (nbcav1 != nbcav2) || (nbtun1 != nbtun2))
    {
      printf("BINGO !\n");
      writeimage(image,"contre_exemple.pgm");
      return 1;
    }
    printf("%d composantes connexes, ", nbcc1);
    printf("%d cavites, ", nbcav1);
    printf("%d tunnels\n", nbtun1);
  }
  return 0;
} /* main */
#endif
