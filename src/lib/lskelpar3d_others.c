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
   Algorithmes 3D de squelettisation

   Algo. de Palagyi (curviligne 6 subiterations, PRL 1998. Adapté par John Chaussard)
   Algo. de Lohou et Bertrand (curviligne symmétrique, Pat. Rec. 2007) 
   Algo. de Ma, Wan & Chang (curviligne 2 subfields, PRL 2002)
   Algo. de Tsao & Fu (curviligne 6 subiterations, IEEE PRIP 1982)
   Algo. de Ma & Sonka (curviligne fully parallel, CVIU 1996)
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
#ifdef UNIXIO
#  include <unistd.h>
#else /* NOT UNIXIO */
#  include <stdlib.h>
#  include <io.h>
#endif /* NOT UNIXIO */

#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcindic.h>
#include <mctopo3d.h>
#include <lskelpar3d_others.h>

#define VERBOSE
#define DEBUG

/* ==================================== */
static void extract_vois(
  uint8_t *img,          /* pointeur base image */
  index_t p,                       /* index du point */
  index_t rs,                      /* taille rangee */
  index_t ps,                      /* taille plan */
  index_t N,                       /* taille image */
  uint8_t *vois)    
/* 
  retourne dans "vois" les valeurs des 27 voisins de p, dans l'ordre suivant: 

               12      11      10       
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  vois[ 0] = *(ptr+1);
  vois[ 1] = *(ptr+1-rs);
  vois[ 2] = *(ptr-rs);
  vois[ 3] = *(ptr-rs-1);
  vois[ 4] = *(ptr-1);
  vois[ 5] = *(ptr-1+rs);
  vois[ 6] = *(ptr+rs);
  vois[ 7] = *(ptr+rs+1);

  vois[ 8] = *(ptr-ps);
  vois[ 9] = *(ptr-ps+1);
  vois[10] = *(ptr-ps+1-rs);
  vois[11] = *(ptr-ps-rs);
  vois[12] = *(ptr-ps-rs-1);
  vois[13] = *(ptr-ps-1);
  vois[14] = *(ptr-ps-1+rs);
  vois[15] = *(ptr-ps+rs);
  vois[16] = *(ptr-ps+rs+1);

  vois[17] = *(ptr+ps);
  vois[18] = *(ptr+ps+1);
  vois[19] = *(ptr+ps+1-rs);
  vois[20] = *(ptr+ps-rs);
  vois[21] = *(ptr+ps-rs-1);
  vois[22] = *(ptr+ps-1);
  vois[23] = *(ptr+ps-1+rs);
  vois[24] = *(ptr+ps+rs);
  vois[25] = *(ptr+ps+rs+1);

  vois[26] = *(ptr);
} /* extract_vois() */

/* ==================================== */
static index_t return_vois(
  index_t p,
  int32_t dir,
  index_t rs,                      /* taille rangee */
  index_t ps)                      /* taille plan */
/* 
  retourne l'index du point voisin q de p dans la direction dir

               12      11      10       
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "return_vois"
  switch (dir)
  {
  case 0: return p+1;
  case 1: return p+1-rs;
  case 2: return p-rs;
  case 3: return p-rs-1;
  case 4: return p-1;
  case 5: return p-1+rs;
  case 6: return p+rs;
  case 7: return p+rs+1;

  case 8: return p-ps;
  case 9: return p-ps+1;
  case 10: return p-ps+1-rs;
  case 11: return p-ps-rs;
  case 12: return p-ps-rs-1;
  case 13: return p-ps-1;
  case 14: return p-ps-1+rs;
  case 15: return p-ps+rs;
  case 16: return p-ps+rs+1;

  case 17: return p+ps;
  case 18: return p+ps+1;
  case 19: return p+ps+1-rs;
  case 20: return p+ps-rs;
  case 21: return p+ps-rs-1;
  case 22: return p+ps-1;
  case 23: return p+ps-1+rs;
  case 24: return p+ps+rs;
  case 25: return p+ps+rs+1;
  case 26: return p;
  default: 
    printf("%s: ERREUR: mauvais code dir %d\n", F_NAME, dir);
    exit(0);
  }
} /* return_vois() */

#ifdef DEBUG
/* ==================================== */
static index_t print_vois(uint8_t *v)
/* ==================================== */
{
  printf("%d %d %d\n", v[12], v[11], v[10]);
  printf("%d %d %d\n", v[13], v[8], v[9]);
  printf("%d %d %d\n\n", v[15], v[15], v[16]);
  printf("%d %d %d\n", v[3], v[2], v[1]);
  printf("%d %d %d\n", v[4], v[26], v[0]);
  printf("%d %d %d\n\n", v[5], v[6], v[7]);
  printf("%d %d %d\n", v[21], v[20], v[19]);
  printf("%d %d %d\n", v[22], v[17], v[18]);
  printf("%d %d %d\n\n", v[23], v[24], v[25]);
} // print_vois()
#endif

/* ==================================== */
static int32_t direction(
  uint8_t *img,          /* pointeur base image */
  index_t p,             /* index du point */
  int32_t dir,           /* indice direction */
  index_t rs,            /* taille rangee */
  index_t ps,            /* taille plan */
  index_t N              /* taille image */
)    
/* 
  retourne 1 si p a un voisin nul dans la direction dir, 0 sinon :

#ifdef DIRTOURNE
                .       .       .       
                .       2       .       
                .       .       .       

		.	1	.			
		0       x	3
                .       4       .       

                .       .       .       
                .       5       .       
                .       .       .       
#else
                .       .       .       
                .       4       .       
                .       .       .       

		.	2	.			
		0       x	1
                .       3       .       

                .       .       .       
                .       5       .       
                .       .       .       
#endif
  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "direction"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  switch (dir)
  {
#ifdef DIRTOURNE
  case 0: if (*(ptr-1)) return 0; else return 1;
  case 1: if (*(ptr-rs)) return 0; else return 1;
  case 2: if (*(ptr-ps)) return 0; else return 1;

  case 3: if (*(ptr+1)) return 0; else return 1;
  case 4: if (*(ptr+rs)) return 0; else return 1;
  case 5: if (*(ptr+ps)) return 0; else return 1;
#else
  case 0: if (*(ptr-1)) return 0; else return 1;
  case 1: if (*(ptr+1)) return 0; else return 1;

  case 2: if (*(ptr-rs)) return 0; else return 1;
  case 3: if (*(ptr+rs)) return 0; else return 1;

  case 4: if (*(ptr-ps)) return 0; else return 1;
  case 5: if (*(ptr+ps)) return 0; else return 1;
#endif
  default:
    printf("%s: ERREUR: bad dir = %d\n", F_NAME, dir);
    exit(0);
  } // switch (dir)
} /* direction() */

/* ==================================== */
static void isometrieXZ_vois(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
/* ==================================== */
{
  uint8_t v[27];
  int32_t i;
  v[ 0] = vois[17];  v[ 1] = vois[20];  v[ 2] = vois[ 2];  v[ 3] = vois[11];
  v[ 4] = vois[ 8];  v[ 5] = vois[15];  v[ 6] = vois[ 6];  v[ 7] = vois[24];
  v[ 8] = vois[ 4];  v[ 9] = vois[22];  v[10] = vois[21];  v[11] = vois[ 3];
  v[12] = vois[12];  v[13] = vois[13];  v[14] = vois[14];  v[15] = vois[ 5];
  v[16] = vois[23];  v[17] = vois[ 0];  v[18] = vois[18];  v[19] = vois[19];
  v[20] = vois[ 1];  v[21] = vois[10];  v[22] = vois[ 9];  v[23] = vois[16];
  v[24] = vois[ 7];  v[25] = vois[25];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieXZ_vois() */

/* ==================================== */
static void isometrieYZ_vois(uint8_t *vois)
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
/* ==================================== */
{
  uint8_t v[27];
  int32_t i;
  v[ 0] = vois[ 0];  v[ 1] = vois[18];  v[ 2] = vois[17];  v[ 3] = vois[22];
  v[ 4] = vois[ 4];  v[ 5] = vois[13];  v[ 6] = vois[ 8];  v[ 7] = vois[ 9];
  v[ 8] = vois[ 6];  v[ 9] = vois[ 7];  v[10] = vois[25];  v[11] = vois[24];
  v[12] = vois[23];  v[13] = vois[ 5];  v[14] = vois[14];  v[15] = vois[15];
  v[16] = vois[16];  v[17] = vois[ 2];  v[18] = vois[ 1];  v[19] = vois[19];
  v[20] = vois[20];  v[21] = vois[21];  v[22] = vois[ 3];  v[23] = vois[12];
  v[24] = vois[11];  v[25] = vois[10];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieYZ_vois() */

static void rotate_90_Z(uint8_t *v)
{
  uint8_t t;
  t = v[ 9]; v[ 9] = v[11]; v[11] = v[13]; v[13] = v[15]; v[15] = t;
  t = v[10]; v[10] = v[12]; v[12] = v[14]; v[14] = v[16]; v[16] = t;
  t = v[ 0]; v[ 0] = v[ 2]; v[ 2] = v[ 4]; v[ 4] = v[ 6]; v[ 6] = t;
  t = v[ 1]; v[ 1] = v[ 3]; v[ 3] = v[ 5]; v[ 5] = v[ 7]; v[ 7] = t;
  t = v[18]; v[18] = v[20]; v[20] = v[22]; v[22] = v[24]; v[24] = t;
  t = v[19]; v[19] = v[21]; v[21] = v[23]; v[23] = v[25]; v[25] = t;
} // rotate_90_Z()

static void swap_U_L(uint8_t *v)
{
  uint8_t t;
  t = v[ 8]; v[ 8] = v[17]; v[17] = t;
  t = v[ 9]; v[ 9] = v[18]; v[18] = t;
  t = v[10]; v[10] = v[19]; v[19] = t;
  t = v[11]; v[11] = v[20]; v[20] = t;
  t = v[12]; v[12] = v[21]; v[21] = t;
  t = v[13]; v[13] = v[22]; v[22] = t;
  t = v[14]; v[14] = v[23]; v[23] = t;
  t = v[15]; v[15] = v[24]; v[24] = t;
  t = v[16]; v[16] = v[25]; v[25] = t;
} // swap_U_L()

/* ==================================== */
static void extract_vois125(
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
 
 le point central est le 62, ses voisins sont :

  31  32  33
  36  37  38
  41  42  43

  56  57  58
  61  62  63
  66  67  68

  81  82  83
  86  87  88
  91  92  93

*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois125"
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

} /* extract_vois125() */

/* ==================================== */
static void extract_vois27(
  uint8_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 27 voisins de p, dans l'ordre suivant:

  0   1   2   
  3   4   5
  6   7   8   

  9   10  11
  12  13  14
  15  16  17

  18  19  20
  21  22  23
  24  25  26

  le tableau vois doit avoir été alloué
  le point central est le 13
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois25"
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-1) || (p%ps<rs) || (p%rs<=0) || (p%ps>=ps-rs) ||
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
      {
	vois[(k * 9) + (j * 3) + i] =
	  img[((zz-1+k) * ps) + ((yy-1+j) * rs) + xx-1+i];
      }

} /* extract_vois27() */

/* ==================================== */
static index_t return_vois27(
  index_t p,
  int32_t dir,
  index_t rs,                      /* taille rangee */
  index_t ps)                      /* taille plan */
/* 
  retourne l'index du point voisin q de p dans la direction dir

  0   1   2   
  3   4   5
  6   7   8   

  9   10  11
  12  13  14
  15  16  17

  18  19  20
  21  22  23
  24  25  26

  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "return_vois27"
  switch (dir)
  {
  case  0: return p-ps-rs-1;
  case  1: return p-ps-rs;
  case  2: return p-ps+1-rs;
  case  3: return p-ps-1;
  case  4: return p-ps;
  case  5: return p-ps+1;
  case  6: return p-ps-1+rs;
  case  7: return p-ps+rs;
  case  8: return p-ps+rs+1;

  case  9: return p-rs-1;
  case 10: return p-rs;
  case 11: return p+1-rs;
  case 12: return p-1;
  case 13: return p;
  case 14: return p+1;
  case 15: return p-1+rs;
  case 16: return p+rs;
  case 17: return p+rs+1;

  case 18: return p+ps-rs-1;
  case 19: return p+ps-rs;
  case 20: return p+ps+1-rs;
  case 21: return p+ps-1;
  case 22: return p+ps;
  case 23: return p+ps+1;
  case 24: return p+ps-1+rs;
  case 25: return p+ps+rs;
  case 26: return p+ps+rs+1;
  default: 
    printf("%s: ERREUR: mauvais code dir %d\n", F_NAME, dir);
    exit(0);
  }
} /* return_vois27() */

#ifdef DEBUG
/* ==================================== */
static index_t print_vois27(uint8_t *v)
/* ==================================== */
{
  printf("%d %d %d\n", v[0], v[1], v[2]);
  printf("%d %d %d\n", v[3], v[4], v[5]);
  printf("%d %d %d\n\n", v[6], v[7], v[8]);
  printf("%d %d %d\n", v[9], v[10], v[11]);
  printf("%d %d %d\n", v[12], v[13], v[14]);
  printf("%d %d %d\n\n", v[15], v[16], v[17]);
  printf("%d %d %d\n", v[18], v[19], v[20]);
  printf("%d %d %d\n", v[21], v[22], v[23]);
  printf("%d %d %d\n\n", v[24], v[25], v[26]);
} // print_vois27()
#endif

/* ==================================== */
static void isometrieXZ_vois27(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois27()
/* ==================================== */
{
  uint8_t v[27];
  int32_t i;
  v[14] = vois[22];  v[11] = vois[19];  v[10] = vois[10];  v[ 9] = vois[ 1];
  v[12] = vois[ 4];  v[15] = vois[ 7];  v[16] = vois[16];  v[17] = vois[25];
  v[ 4] = vois[12];  v[ 5] = vois[21];  v[ 2] = vois[18];  v[ 1] = vois[ 9];
  v[ 0] = vois[ 0];  v[ 3] = vois[ 3];  v[ 6] = vois[ 6];  v[ 7] = vois[15];
  v[ 8] = vois[24];  v[22] = vois[14];  v[23] = vois[23];  v[20] = vois[20];
  v[19] = vois[11];  v[18] = vois[ 2];  v[21] = vois[ 5];  v[24] = vois[ 8];
  v[25] = vois[17];  v[26] = vois[26];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieXZ_vois27() */

/* ==================================== */
static void isometrieYZ_vois27(uint8_t *vois)
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois27()
/* ==================================== */
{
  uint8_t v[27];
  int32_t i;
  v[14] = vois[14];  v[11] = vois[23];  v[10] = vois[22];  v[ 9] = vois[21];
  v[12] = vois[12];  v[15] = vois[ 3];  v[16] = vois[ 4];  v[17] = vois[ 5];
  v[ 4] = vois[16];  v[ 5] = vois[17];  v[ 2] = vois[26];  v[ 1] = vois[25];
  v[ 0] = vois[24];  v[ 3] = vois[15];  v[ 6] = vois[ 6];  v[ 7] = vois[ 7];
  v[ 8] = vois[ 8];  v[22] = vois[10];  v[23] = vois[11];  v[20] = vois[20];
  v[19] = vois[19];  v[18] = vois[18];  v[21] = vois[ 9];  v[24] = vois[ 0];
  v[25] = vois[ 1];  v[26] = vois[ 2];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieYZ_vois27() */

static void rotate_90_Z_27(uint8_t *v)
{
// version pour la numérotation de extract_vois27()
  uint8_t t;
  t = v[ 5]; v[ 5] = v[ 1]; v[ 1] = v[ 3]; v[ 3] = v[ 7]; v[ 7] = t;
  t = v[ 2]; v[ 2] = v[ 0]; v[ 0] = v[ 6]; v[ 6] = v[ 8]; v[ 8] = t;
  t = v[14]; v[14] = v[10]; v[10] = v[12]; v[12] = v[16]; v[16] = t;
  t = v[11]; v[11] = v[ 9]; v[ 9] = v[15]; v[15] = v[17]; v[17] = t;
  t = v[23]; v[23] = v[19]; v[19] = v[21]; v[21] = v[25]; v[25] = t;
  t = v[20]; v[20] = v[18]; v[18] = v[24]; v[24] = v[26]; v[26] = t;
} // rotate_90_Z_27()

static void swap_U_L_27(uint8_t *v)
{
// version pour la numérotation de extract_vois27()
  uint8_t t;
  t = v[ 4]; v[ 4] = v[22]; v[22] = t;
  t = v[ 5]; v[ 5] = v[23]; v[23] = t;
  t = v[ 2]; v[ 2] = v[20]; v[20] = t;
  t = v[ 1]; v[ 1] = v[19]; v[19] = t;
  t = v[ 0]; v[ 0] = v[18]; v[18] = t;
  t = v[ 3]; v[ 3] = v[21]; v[21] = t;
  t = v[ 6]; v[ 6] = v[24]; v[24] = t;
  t = v[ 7]; v[ 7] = v[25]; v[25] = t;
  t = v[ 8]; v[ 8] = v[26]; v[26] = t;
} // swap_U_L_27()

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma & Sonka (curviligne fully parallel, CVIU 1996)
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define MS_OBJECT      1
#define MS_DELETABLE   2
#define IS_MS_DELETABLE(f) (f&MS_DELETABLE)
#define SET_MS_DELETABLE(f) (f|=MS_DELETABLE)

#define AVEC_EXTRACT_VOIS27
#ifdef AVEC_EXTRACT_VOIS27
/* ==================================== */
int32_t ma_sonka_match_A(uint8_t *v)
/* ==================================== */
{
  if (v[13] && v[22] &&
      !v[0] && !v[1] && !v[2] && !v[3] && !v[4] && !v[5] &&
      !v[6] && !v[7] && !v[8])
  {
    printf("ma_sonka_match_A : MATCH\n"); // DEBUG
    return 1;
  }
  printf("ma_sonka_match_A : no match\n"); // DEBUG
  return 0;
} // ma_sonka_match_A()

/* ==================================== */
int32_t ma_sonka_match_B(uint8_t *v)
/* ==================================== */
{
  if (v[13] && v[22] && v[12] &&
      !v[1] && !v[2] && !v[4] && !v[5] && !v[7] && !v[8] &&
      !v[11] && !v[14] && !v[17])
    return 1;
  else
    return 0;
} // ma_sonka_match_B()

/* ==================================== */
int32_t ma_sonka_match_C(uint8_t *v)
/* ==================================== */
{
  if (v[13] && v[22] && v[12] && v[16] &&
      !v[1] && !v[2] && !v[4] && !v[5] &&
      !v[10] && !v[11] && !v[14])
    return 1;
  else
    return 0;
} // ma_sonka_match_C()

/* ==================================== */
int32_t ma_sonka_match_D(uint8_t *v)
/* ==================================== */
{
  if (v[13] && v[21] &&
      !v[12] && !v[3] && !v[4] && !v[5] && 
      !v[14] && !v[23] && !v[22])
    return 1;
  else
    return 0;
} // ma_sonka_match_D()
#else
(avec extract_vois - obsolete !)
/* ==================================== */
int32_t ma_sonka_match_A(uint8_t *v)
/* ==================================== */
{
  if (v[26] && v[17] &&
      !v[8] && !v[9] && !v[10] && !v[11] && !v[12] && !v[13] &&
      !v[14] && !v[15] && !v[16])
    return 1;
  else
    return 0;
} // ma_sonka_match_A()

/* ==================================== */
int32_t ma_sonka_match_B(uint8_t *v)
/* ==================================== */
{
  if (v[26] && v[17] && v[4] &&
      !v[8] && !v[9] && !v[10] && !v[11] && !v[15] && !v[16] &&
      !v[0] && !v[1] && !v[7])
    return 1;
  else
    return 0;
} // ma_sonka_match_B()

/* ==================================== */
int32_t ma_sonka_match_C(uint8_t *v)
/* ==================================== */
{
  if (v[26] && v[17] && v[4] && v[6] &&
      !v[8] && !v[9] && !v[10] && !v[11] &&
      !v[0] && !v[1] && !v[2])
    return 1;
  else
    return 0;
} // ma_sonka_match_C()
#endif

#define MS_S  16
#define MS_W  12
#define MS_D  22
#define MS_U   4
#define MS_E  14
#define MS_N  10
#define MS_SU  7
#define MS_SD 25
#define MS_SE 17
#define MS_SW 15
#define MS_WU  3
#define MS_WD 21

/* ==================================== */
int32_t ma_sonka_dir_ABC(uint8_t *v, uint8_t *S, index_t p, index_t rs, index_t ps)
/* ==================================== */
{
  index_t s, w, d;
  int32_t x, y, z;

  printf("DEBUG: ma_sonka_dir_ABC begins - p = %d\n", p);
  x = p % rs; y = (p % ps) / rs; z = p / ps;

  s = return_vois27(13, MS_S, 3, 9);
  if (v[s])
  {
    int32_t dxs, dys, dzs; index_t ss;
    // coord. relatives de s par rapport au point central de v
    dxs = -1 + (s % 3); dys = -1 + ((s % 9) / 3); dzs = -1 + (s / 9); 
    // calcule ss
    ss = (z+dzs+dzs)*ps + (y+dys+dys)*rs + (x+dxs+dxs);
    printf("ma_sonka_match_ABC : s=%d objet, dxs=%d, dys=%d, dzs=%d\n", s, dxs, dys, dzs); // DEBUG
    print_vois27(v);  // DEBUG
    printf("ma_sonka_match_ABC : ss=%d (%d,%d,%d)\n", ss, ss%rs, (ss%ps)/rs, ss/ps); // DEBUG
    if (!S[ss]) 
    {
      printf("ma_sonka_match_ABC : no match\n"); // DEBUG
      return 0;
    }
  }

  w = return_vois27(13, MS_W, 3, 9);
  if (v[w])
  {
    int32_t dxw, dyw, dzw; index_t ww;
    // coord. relatives de w par rapport au point central de v
    dxw = -1 + (w % 3); dyw = -1 + ((w % 9) / 3); dzw = -1 + (w / 9); 
    // calcule ww
    ww = (z+dzw+dzw)*ps + (y+dyw+dyw)*rs + (x+dxw+dxw);
    printf("ma_sonka_match_ABC : w=%d objet, dxw=%d, dyw=%d, dzw=%d\n", w, dxw, dyw, dzw); // DEBUG
    print_vois27(v);  // DEBUG
    printf("ma_sonka_match_ABC : ww=%d (%d,%d,%d)\n", ww, ww%rs, (ww%ps)/rs, ww/ps); // DEBUG
    if (!S[ww]) 
    {
      printf("ma_sonka_match_ABC : no match\n"); // DEBUG
      return 0;
    }
  }

  d = return_vois27(13, MS_D, 3, 9);
  if (v[d])
  {
    int32_t dxd, dyd, dzd; index_t dd;
    // coord. relatives de d par rapport au point central de v
    dxd = -1 + (d % 3); dyd = -1 + ((d % 9) / 3); dzd = -1 + (d / 9); 
    // calcule dd
    dd = (z+dzd+dzd)*ps + (y+dyd+dyd)*rs + (x+dxd+dxd);
    printf("ma_sonka_match_ABC : d=%d objet, dxd=%d, dyd=%d, dzd=%d\n", d, dxd, dyd, dzd); // DEBUG
    print_vois27(v);  // DEBUG
    printf("ma_sonka_match_ABC : dd=%d (%d,%d,%d)\n", dd, dd%rs, (dd%ps)/rs, dd/ps); // DEBUG
    if (!S[dd])
    {
      printf("ma_sonka_match_ABC : no match\n"); // DEBUG
      return 0;
    }
  }

  return 1;
} // ma_sonka_dir_ABC()

/* ==================================== */
int32_t ma_sonka_dir_D(uint8_t *v, uint8_t *S, index_t p, index_t rs, index_t ps)
/* ==================================== */
{
  index_t q;
  int32_t x, y, z, xq, yq, zq;

  //printf("DEBUG: ma_sonka_dir_D begins\n");
  x = p % rs;
  y = (p % ps) / rs;
  z = p / ps;

  q = return_vois27(13, MS_SU, 3, 9);
  if (v[q])
  {
    int32_t dxs, dys, dzs, dxu, dyu, dzu, dxsu, dysu, dzsu;
    index_t s, u, ss, ssu, susu, usu, uu; // su = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    s = return_vois27(13, MS_S, 3, 9);
    u = return_vois27(13, MS_U, 3, 9);
    // coord. relatives de s par rapport au point central de v
    dxs = -1 + (s % 3); dys = -1 + ((s % 9) / 3); dzs = -1 + (s / 9); 
    // coord. relatives de u par rapport au point central de v
    dxu = -1 + (u % 3); dyu = -1 + ((u % 9) / 3); dzu = -1 + (u / 9); 
    // coord. relatives de su par rapport au point central de v
    dxsu = -1 + (q % 3); dysu = -1 + ((q % 9) / 3); dzsu = -1 + (q / 9); 
    // calcule ss, ssu, susu, usu, uu
    ss = (z+dzs+dzs)*ps + (y+dys+dys)*rs + (x+dxs+dxs);
    ssu = (zq+dzs)*ps + (yq+dys)*rs + (xq+dxs);
    susu = (zq+dzsu)*ps + (yq+dysu)*rs + (xq+dxsu);
    usu = (zq+dzu)*ps + (yq+dyu)*rs + (xq+dxu);
    uu = (z+dzu+dzu)*ps + (y+dyu+dyu)*rs + (x+dxu+dxu);
    if (!S[ss] && !S[ssu] && !S[susu] && !S[usu] && !S[uu]) return 0;
  }

  q = return_vois27(13, MS_SD, 3, 9);
  if (v[q])
  {
    int32_t dxs, dys, dzs, dxd, dyd, dzd, dxsd, dysd, dzsd;
    index_t s, d, ss, ssd, sdsd, dsd, dd; // sd = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    s = return_vois27(13, MS_S, 3, 9);
    d = return_vois27(13, MS_D, 3, 9);
    // coord. relatives de s par rapport au point central de v
    dxs = -1 + (s % 3); dys = -1 + ((s % 9) / 3); dzs = -1 + (s / 9); 
    // coord. relatives de d par rapport au point central de v
    dxd = -1 + (d % 3); dyd = -1 + ((d % 9) / 3); dzd = -1 + (d / 9); 
    // coord. relatives de sd par rapport au point central de v
    dxsd = -1 + (q % 3); dysd = -1 + ((q % 9) / 3); dzsd = -1 + (q / 9); 
    // calcule ss, ssd, sdsd, dsd, dd
    ss = (z+dzs+dzs)*ps + (y+dys+dys)*rs + (x+dxs+dxs);
    ssd = (zq+dzs)*ps + (yq+dys)*rs + (xq+dxs);
    sdsd = (zq+dzsd)*ps + (yq+dysd)*rs + (xq+dxsd);
    dsd = (zq+dzd)*ps + (yq+dyd)*rs + (xq+dxd);
    dd = (z+dzd+dzd)*ps + (y+dyd+dyd)*rs + (x+dxd+dxd);
    if (!S[ss] && !S[ssd] && !S[sdsd] && !S[dsd] && !S[dd]) return 0;
  }

  q = return_vois27(13, MS_SW, 3, 9);
  if (v[q])
  {
    int32_t dxs, dys, dzs, dxw, dyw, dzw, dxsw, dysw, dzsw;
    index_t s, w, ss, ssw, swsw, wsw, ww; // sw = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    s = return_vois27(13, MS_S, 3, 9);
    w = return_vois27(13, MS_W, 3, 9);
    // coord. relatives de s par rapport au point central de v
    dxs = -1 + (s % 3); dys = -1 + ((s % 9) / 3); dzs = -1 + (s / 9); 
    // coord. relatives de w par rapport au point central de v
    dxw = -1 + (w % 3); dyw = -1 + ((w % 9) / 3); dzw = -1 + (w / 9); 
    // coord. relatives de sw par rapport au point central de v
    dxsw = -1 + (q % 3); dysw = -1 + ((q % 9) / 3); dzsw = -1 + (q / 9); 
    // calcule ss, ssw, swsw, wsw, ww
    ss = (z+dzs+dzs)*ps + (y+dys+dys)*rs + (x+dxs+dxs);
    ssw = (zq+dzs)*ps + (yq+dys)*rs + (xq+dxs);
    swsw = (zq+dzsw)*ps + (yq+dysw)*rs + (xq+dxsw);
    wsw = (zq+dzw)*ps + (yq+dyw)*rs + (xq+dxw);
    ww = (z+dzw+dzw)*ps + (y+dyw+dyw)*rs + (x+dxw+dxw);
    if (!S[ss] && !S[ssw] && !S[swsw] && !S[wsw] && !S[ww]) return 0;
  }

  q = return_vois27(13, MS_SE, 3, 9);
  if (v[q])
  {
    int32_t dxs, dys, dzs, dxe, dye, dze, dxse, dyse, dzse;
    index_t s, e, ss, sse, sese, ese, ee; // se = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    s = return_vois27(13, MS_S, 3, 9);
    e = return_vois27(13, MS_E, 3, 9);
    // coord. relatives de s par rapport au point central de v
    dxs = -1 + (s % 3); dys = -1 + ((s % 9) / 3); dzs = -1 + (s / 9); 
    // coord. relatives de e par rapport au point central de v
    dxe = -1 + (e % 3); dye = -1 + ((e % 9) / 3); dze = -1 + (e / 9); 
    // coord. relatives de se par rapport au point central de v
    dxse = -1 + (q % 3); dyse = -1 + ((q % 9) / 3); dzse = -1 + (q / 9); 
    // calcule ss, sse, sese, ese, ee
    ss = (z+dzs+dzs)*ps + (y+dys+dys)*rs + (x+dxs+dxs);
    sse = (zq+dzs)*ps + (yq+dys)*rs + (xq+dxs);
    sese = (zq+dzse)*ps + (yq+dyse)*rs + (xq+dxse);
    ese = (zq+dze)*ps + (yq+dye)*rs + (xq+dxe);
    ee = (z+dze+dze)*ps + (y+dye+dye)*rs + (x+dxe+dxe);
    if (!S[ss] && !S[sse] && !S[sese] && !S[ese] && !S[ee]) return 0;
  }

  q = return_vois27(13, MS_WU, 3, 9);
  if (v[q])
  {
    int32_t dxw, dyw, dzw, dxu, dyu, dzu, dxwu, dywu, dzwu;
    index_t w, u, ww, wwu, wuwu, uwu, uu; // wu = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    w = return_vois27(13, MS_W, 3, 9);
    u = return_vois27(13, MS_U, 3, 9);
    // coord. relatives de w par rapport au point central de v
    dxw = -1 + (w % 3); dyw = -1 + ((w % 9) / 3); dzw = -1 + (w / 9); 
    // coord. relatives de u par rapport au point central de v
    dxu = -1 + (u % 3); dyu = -1 + ((u % 9) / 3); dzu = -1 + (u / 9); 
    // coord. relatives de wu par rapport au point central de v
    dxwu = -1 + (q % 3); dywu = -1 + ((q % 9) / 3); dzwu = -1 + (q / 9); 
    // calcule ww, wwu, wuwu, uwu, uu
    ww = (z+dzw+dzw)*ps + (y+dyw+dyw)*rs + (x+dxw+dxw);
    wwu = (zq+dzw)*ps + (yq+dyw)*rs + (xq+dxw);
    wuwu = (zq+dzwu)*ps + (yq+dywu)*rs + (xq+dxwu);
    uwu = (zq+dzu)*ps + (yq+dyu)*rs + (xq+dxu);
    uu = (z+dzu+dzu)*ps + (y+dyu+dyu)*rs + (x+dxu+dxu);
    if (!S[ww] && !S[wwu] && !S[wuwu] && !S[uwu] && !S[uu]) return 0;
  }

  q = return_vois27(13, MS_WD, 3, 9);
  if (v[q])
  {
    int32_t dxw, dyw, dzw, dxd, dyd, dzd, dxwd, dywd, dzwd;
    index_t w, d, ww, wwd, wdwd, dwd, dd; // wd = q
    xq = q % rs;
    yq = (q % ps) / rs;
    zq = q / ps;
    w = return_vois27(13, MS_W, 3, 9);
    d = return_vois27(13, MS_D, 3, 9);
    // coord. relatives de w par rapport au point central de v
    dxw = -1 + (w % 3); dyw = -1 + ((w % 9) / 3); dzw = -1 + (w / 9); 
    // coord. relatives de d par rapport au point central de v
    dxd = -1 + (d % 3); dyd = -1 + ((d % 9) / 3); dzd = -1 + (d / 9); 
    // coord. relatives de wd par rapport au point central de v
    dxwd = -1 + (q % 3); dywd = -1 + ((q % 9) / 3); dzwd = -1 + (q / 9); 
    // calcule ww, wwd, wdwd, dwd, dd
    ww = (z+dzw+dzw)*ps + (y+dyw+dyw)*rs + (x+dxw+dxw);
    wwd = (zq+dzw)*ps + (yq+dyw)*rs + (xq+dxw);
    wdwd = (zq+dzwd)*ps + (yq+dywd)*rs + (xq+dxwd);
    dwd = (zq+dzd)*ps + (yq+dyd)*rs + (xq+dxd);
    dd = (z+dzd+dzd)*ps + (y+dyd+dyd)*rs + (x+dxd+dxd);
    if (!S[ww] && !S[wwd] && !S[wdwd] && !S[dwd] && !S[dd]) return 0;
  }

  return 1;
} // ma_sonka_dir_D()

/* ==================================== */
int32_t ma_sonka_deletable(uint8_t *v, uint8_t *S, index_t p, index_t rs, index_t ps)
/* ==================================== */
{
  printf("DEBUG: ma_sonka_deletable begins - p = %d\n", p);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // U
  swap_U_L_27(v);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // D
  swap_U_L_27(v); // retour

  isometrieXZ_vois27(v);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // E
  swap_U_L_27(v);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // W
  swap_U_L_27(v); // retour
  isometrieXZ_vois27(v); // retour

  isometrieYZ_vois27(v);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // N
  swap_U_L_27(v);
  if (ma_sonka_match_A(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1; // S
  swap_U_L_27(v); // retour
  isometrieYZ_vois27(v); // retour

  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);

  swap_U_L_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_C(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  swap_U_L_27(v); // retour

  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);

  swap_U_L_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  swap_U_L_27(v); // retour

  isometrieYZ_vois27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  swap_U_L_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  rotate_90_Z_27(v);
  rotate_90_Z_27(v);
  if (ma_sonka_match_B(v) && ma_sonka_dir_ABC(v, S, p, rs, ps)) return 1;
  if (ma_sonka_match_D(v) && ma_sonka_dir_D(v, S, p, rs, ps)) return 1;
  swap_U_L_27(v); // retour
  isometrieYZ_vois27(v); // retour

  return 0;
} // ma_sonka_deletable()

/* ==================================== */
int32_t ma_sonka_nontail(uint8_t *v, uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  //printf("DEBUG: ma_sonka_nontail begins\n");
  if (mctopo3d_nbvoiso26(S, p, rs, ps, N) == 1) // line_end point = tail
    return 0;
  if (mctopo3d_nbvoiso26(S, p, rs, ps, N) == 2)
  {
    if (v[MS_S] && v[MS_E]) return 0; // near_line_end point = tail
    if (v[MS_S] && v[MS_U]) return 0;
    if (v[MS_N] && v[MS_W]) return 0;
    if (v[MS_U] && v[MS_W]) return 0;
    if (v[MS_N] && v[MS_D]) return 0;
    if (v[MS_E] && v[MS_D]) return 0;
  }
  return 1;
} // ma_sonka_nontail()

/* ==================================== */
int32_t lmasonka1996(
		     struct xvimage *image,
		     int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmasonka1996"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif  
  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    step++;
    nonstab = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MS_OBJECT;

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x] && mctopo3d_simple26(S, x, rs, ps, N))
      {
	extract_vois27(S, x, rs, ps, N, v);
	printf("%s: point %d,%d,%d (%d)\n",F_NAME, i, j, k, x); // DEBUG
	if (ma_sonka_nontail(v,S,x,rs,ps,N) && ma_sonka_deletable(v,S,x,rs,ps))
	{
	  printf("%s: point %d,%d,%d (%d) nontail and deletable\n", F_NAME, i, j, k, x); // DEBUG
	  SET_MS_DELETABLE(S[x]);
	}
      } // if (S[x])
    } // for i, j, k

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (IS_MS_DELETABLE(S[x]))
      {
	S[x] = 0;
	nonstab = 1;
      }
    } // for i, j, k
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  return(1);
} /* lmasonka1996() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Tsao & Fu (curviligne 6 subiterations, IEEE PRIP 1982)
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define T_OBJECT      1
#define T_SIMPLE      2
#define T_NONEND      4

#define IS_T_OBJECT(f)     (f&T_OBJECT)
#define IS_T_SIMPLE(f)     (f&T_SIMPLE)
#define IS_T_NONEND(f)     (f&T_NONEND)

#define SET_T_OBJECT(f)    (f|=T_OBJECT)
#define SET_T_SIMPLE(f)    (f|=T_SIMPLE)
#define SET_T_NONEND(f)    (f|=T_NONEND)

#define UNSET_T_OBJECT(f)  (f&=~T_OBJECT)
#define UNSET_T_SIMPLE(f)  (f&=~T_SIMPLE)

/* ==================================== */
int32_t ltsaofu6dircurv1982(struct xvimage *image,
			    struct xvimage *inhibit,
			    int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "ltsaofu6dircurv1982"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL;
  int32_t step, nonstab, d;

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = T_OBJECT;

  if (inhibit != NULL) I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("%s: step %d\n", F_NAME, step);
#endif

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE
      printf("%s: substep %d\n", F_NAME, d);
#endif
      // PREMIERE PASSE : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_T_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  {
	    SET_T_SIMPLE(S[i]);
	  }
      // DEUXIEME PASSE : MARQUE LES POINTS SIMPLES NONEND
      for (i = 0; i < N; i++) 
	if (IS_T_SIMPLE(S[i]))
	{ 
	  if (mctopo3d_tsao_fu_nonend(S, i, T_OBJECT|T_SIMPLE, rs, ps, N))
	    SET_T_NONEND(S[i]);
	}

      // TROISIEME PASSE : EFFACE LES POINTS SIMPLES NONEND
      for (i = 0; i < N; i++)
	if (IS_T_NONEND(S[i]))
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}

      for (i = 0; i < N; i++) if (S[i]) S[i] = T_OBJECT;
#ifdef DEBUG_ltsaofu6dircurv1982
      if (d==0) writeimage(image, "_ss0");
      if (d==1) writeimage(image, "_ss1");
      if (d==2) writeimage(image, "_ss2");
      if (d==3) writeimage(image, "_ss3");
      if (d==4) writeimage(image, "_ss4");
      if (d==5) writeimage(image, "_ss5");
#endif
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // ltsaofu6dircurv1982()

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma, Wan & Chang (curviligne 2 subfields, PRL 2002)
// M. Couprie, dec. 2011
/* ============================================================ */
/* ============================================================ */

#define M_OBJECT      1
#define M_ORTH        2
#define M_DIAG        4
#define M_DIAGPRES    8
#define M_TWIG        16

#define IS_ORTH(f) (f&M_ORTH)
#define SET_ORTH(f) (f|=M_ORTH)
#define IS_DIAG(f) (f&M_DIAG)
#define SET_DIAG(f) (f|=M_DIAG)
//#define UNSET_DIAG(f) (f&=~M_DIAG)
#define IS_DIAGPRES(f) (f&M_DIAGPRES)
#define SET_DIAGPRES(f) (f|=M_DIAGPRES)
#define IS_TWIG(f) (f&M_TWIG)
#define SET_TWIG(f) (f|=M_TWIG)

static int32_t ORTH_U_deletable(uint8_t *v)
{
  if (v[8]) return 0;
  if (!v[17]) return 0;
  if (v[9] && !v[0]) return 0;
  if (v[16] && !v[0] && !v[6]) return 0;
  if (v[11] && !v[2]) return 0;
  if (v[10] && !v[2] && !v[0]) return 0;
  if (v[13] && !v[4]) return 0;
  if (v[12] && !v[4] && !v[2]) return 0;
  if (v[15] && !v[6]) return 0;
  if (v[14] && !v[6] && !v[4]) return 0;
  return 1;
} // ORTH_U_deletable()

static int32_t ORTH_deletable(uint8_t *v)
{
  if (ORTH_U_deletable(v)) return 1; // U-deletable
  swap_U_L(v);
  if (ORTH_U_deletable(v)) return 1; // L-deletable

  isometrieXZ_vois(v);
  if (ORTH_U_deletable(v)) return 1; // E-deletable
  swap_U_L(v);
  if (ORTH_U_deletable(v)) return 1; // W-deletable
  isometrieXZ_vois(v);

  isometrieYZ_vois(v);
  if (ORTH_U_deletable(v)) return 1; // N-deletable
  swap_U_L(v);
  if (ORTH_U_deletable(v)) return 1; // S-deletable
  isometrieYZ_vois(v);

  return 0;
} // ORTH_deletable()

static int32_t DIAG_UE_deletable(uint8_t *v)
{
  //#define DEBUG_lmawanchangcurv2subfields2002
#ifdef DEBUG_lmawanchangcurv2subfields2002
  print_vois(v);
#endif
  if (!v[22]) return 0;
  if (v[4] || v[13] || v[8] || v[9] || v[0] || v[18] || v[17]) return 0;

  if (v[16] && !v[15] && !v[7] && !v[6]) return 0; // a1
  if (v[10] && !v[11] && !v[1] && !v[2]) return 0; // a2

  if ((v[15] || v[14]) && !v[6] && !v[5]) return 0; // b1 or c1
  if ((v[11] || v[12]) && !v[2] && !v[3]) return 0; // b2 or c2

  if ((v[7] || v[25]) && !v[6] && !v[24]) return 0; // d1 or g1
  if ((v[1] || v[19]) && !v[2] && !v[20]) return 0; // d2 or g2

  if (!v[6] && v[5] && v[7] && v[24] && v[16]) return 0; // e1
  if (!v[2] && v[1] && v[3] && v[20] && v[11]) return 0; // e2

  return 1;
} // DIAG_UE_deletable()

static int32_t DIAG_deletable(uint8_t *v)
{
  if (DIAG_UE_deletable(v)) return 1; // UE
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // UN
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // UW
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // US
  rotate_90_Z(v);
  swap_U_L(v);
  if (DIAG_UE_deletable(v)) return 1; // LE
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // LN
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // LW
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1; // LS
  rotate_90_Z(v);
  swap_U_L(v);
  isometrieYZ_vois(v);
  if (DIAG_UE_deletable(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1;
  swap_U_L(v);
  if (DIAG_UE_deletable(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (DIAG_UE_deletable(v)) return 1;
  
  return 0;
} // DIAG_deletable()

#ifdef DEBUG_lmawanchangcurv2subfields2002
static void TEST_DIAG_deletable()
{
  int i; uint8_t v[27];  
  for (i = 0; i < 27; i++) v[i] = 10+i;
  (void)DIAG_deletable(v);
}
#endif

static void DIAG_preserve(uint8_t *v, index_t p, uint8_t *S, index_t rs, index_t ps)
{
  index_t q;
  //#define VARIANT_DIAG_preserve
#ifndef VARIANT_DIAG_preserve
  if (v[13] && !v[8] && !v[4]) // UE
  { q = return_vois(p, 13, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[11] && !v[8] && !v[2]) // UN
  { q = return_vois(p, 11, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[9] && !v[8] && !v[0])  // UW
  { q = return_vois(p, 9, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[15] && !v[8] && !v[6]) // US
  { q = return_vois(p, 15, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[1] && !v[2] && !v[0])  // NW
  { q = return_vois(p, 1, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[7] && !v[6] && !v[0])  // SW
  { q = return_vois(p, 7, rs, ps); SET_DIAGPRES(S[q]); }
#else
  if (v[22] && !v[17] && !v[4]) // LE
  { q = return_vois(p, 22, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[20] && !v[17] && !v[2]) // LN
  { q = return_vois(p, 20, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[18] && !v[17] && !v[0]) // LW
  { q = return_vois(p, 18, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[24] && !v[17] && !v[6]) // LS
  { q = return_vois(p, 24, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[5] && !v[4] && !v[6]) // SE
  { q = return_vois(p, 5, rs, ps); SET_DIAGPRES(S[q]); }
  if (v[3] && !v[4] && !v[2]) // NE
  { q = return_vois(p, 3, rs, ps); SET_DIAGPRES(S[q]); }
#endif
} // DIAG_preserve()

static int32_t TWIG(  
  uint8_t *B,
  index_t x,
  index_t rs,
  index_t ps,
  index_t N)
{
  int32_t n = 0;
  index_t y = -1, z;
  if (((x<N-ps)&&(x%rs!=rs-1)) && B[ps+x+1]) { n++; if (y==-1) y=ps+x+1; else z=ps+x+1; }
  if (((x<N-ps)&&(x%rs!=rs-1)&&(x%ps>=rs)) && B[ps+x+1-rs]) { n++; if (y==-1) y=ps+x+1-rs; else z=ps+x+1-rs; }
  if (((x<N-ps)&&(x%ps>=rs)) && B[ps+x-rs]) { n++; if (y==-1) y=ps+x-rs; else z=ps+x-rs; }
  if (((x<N-ps)&&(x%ps>=rs)&&(x%rs!=0)) && B[ps+x-rs-1]) { n++; if (y==-1) y=ps+x-rs-1; else z=ps+x-rs-1; }
  if (((x<N-ps)&&(x%rs!=0)) && B[ps+x-1]) { n++; if (y==-1) y=ps+x-1; else z=ps+x-1; }
  if (((x<N-ps)&&(x%rs!=0)&&(x%ps<ps-rs)) && B[ps+x-1+rs]) { n++; if (y==-1) y=ps+x-1+rs; else z=ps+x-1+rs; }
  if (((x<N-ps)&&(x%ps<ps-rs)) && B[ps+x+rs]) { n++; if (y==-1) y=ps+x+rs; else z=ps+x+rs; }
  if (((x<N-ps)&&(x%ps<ps-rs)&&(x%rs!=rs-1)) && B[ps+x+rs+1]) { n++; if (y==-1) y=ps+x+rs+1; else z=ps+x+rs+1; }
  if (((x<N-ps)) && B[ps+x]) { n++; if (y==-1) y=ps+x; else z=ps+x; }
  if (((x%rs!=rs-1)) && B[x+1]) { n++; if (y==-1) y=x+1; else z=x+1; }
  if (((x%rs!=rs-1)&&(x%ps>=rs)) && B[x+1-rs]) { n++; if (y==-1) y=x+1-rs; else z=x+1-rs; }
  if (((x%ps>=rs)) && B[x-rs]) { n++; if (y==-1) y=x-rs; else z=x-rs; }
  if (((x%ps>=rs)&&(x%rs!=0)) && B[x-rs-1]) { n++; if (y==-1) y=x-rs-1; else z=x-rs-1; }
  if (((x%rs!=0)) && B[x-1]) { n++; if (y==-1) y=x-1; else z=x-1; }
  if (((x%rs!=0)&&(x%ps<ps-rs)) && B[x-1+rs]) { n++; if (y==-1) y=x-1+rs; else z=x-1+rs; }
  if (((x%ps<ps-rs)) && B[x+rs]) { n++; if (y==-1) y=x+rs; else z=x+rs; }
  if (((x%ps<ps-rs)&&(x%rs!=rs-1)) && B[x+rs+1]) { n++; if (y==-1) y=x+rs+1; else z=x+rs+1; }
  if (((x>=ps)&&(x%rs!=rs-1)) && B[-ps+x+1]) { n++; if (y==-1) y=-ps+x+1; else z=-ps+x+1; }
  if (((x>=ps)&&(x%rs!=rs-1)&&(x%ps>=rs)) && B[-ps+x+1-rs]) { n++; if (y==-1) y=-ps+x+1-rs; else z=-ps+x+1-rs; }
  if (((x>=ps)&&(x%ps>=rs)) && B[-ps+x-rs]) { n++; if (y==-1) y=-ps+x-rs; else z=-ps+x-rs; }
  if (((x>=ps)&&(x%ps>=rs)&&(x%rs!=0)) && B[-ps+x-rs-1]) { n++; if (y==-1) y=-ps+x-rs-1; else z=-ps+x-rs-1; }
  if (((x>=ps)&&(x%rs!=0)) && B[-ps+x-1]) { n++; if (y==-1) y=-ps+x-1; else z=-ps+x-1; }
  if (((x>=ps)&&(x%rs!=0)&&(x%ps<ps-rs)) && B[-ps+x-1+rs]) { n++; if (y==-1) y=-ps+x-1+rs; else z=-ps+x-1+rs; }
  if (((x>=ps)&&(x%ps<ps-rs)) && B[-ps+x+rs]) { n++; if (y==-1) y=-ps+x+rs; else z=-ps+x+rs; }
  if (((x>=ps)&&(x%ps<ps-rs)&&(x%rs!=rs-1)) && B[-ps+x+rs+1]) { n++; if (y==-1) y=-ps+x+rs+1; else z=-ps+x+rs+1; }
  if (((x>=ps)) && B[-ps+x]) { n++; if (y==-1) y=-ps+x; else z=-ps+x; }
  if (n == 1)
  {
    int32_t ny = 0;
    B[x] = 0;
    if (((y<N-ps)&&(y%rs!=rs-1)) && B[ps+y+1]) { ny++; z=ps+y+1; }
    if (((y<N-ps)&&(y%rs!=rs-1)&&(y%ps>=rs)) && B[ps+y+1-rs]) { ny++; z=ps+y+1-rs; }
    if (((y<N-ps)&&(y%ps>=rs)) && B[ps+y-rs]) { ny++; z=ps+y-rs; }
    if (((y<N-ps)&&(y%ps>=rs)&&(y%rs!=0)) && B[ps+y-rs-1]) { ny++; z=ps+y-rs-1; }
    if (((y<N-ps)&&(y%rs!=0)) && B[ps+y-1]) { ny++; z=ps+y-1; }
    if (((y<N-ps)&&(y%rs!=0)&&(y%ps<ps-rs)) && B[ps+y-1+rs]) { ny++; z=ps+y-1+rs; }
    if (((y<N-ps)&&(y%ps<ps-rs)) && B[ps+y+rs]) { ny++; z=ps+y+rs; }
    if (((y<N-ps)&&(y%ps<ps-rs)&&(y%rs!=rs-1)) && B[ps+y+rs+1]) { ny++; z=ps+y+rs+1; }
    if (((y<N-ps)) && B[ps+y]) { ny++; z=ps+y; }
    if (((y%rs!=rs-1)) && B[y+1]) { ny++; z=y+1; }
    if (((y%rs!=rs-1)&&(y%ps>=rs)) && B[y+1-rs]) { ny++; z=y+1-rs; }
    if (((y%ps>=rs)) && B[y-rs]) { ny++; z=y-rs; }
    if (((y%ps>=rs)&&(y%rs!=0)) && B[y-rs-1]) { ny++; z=y-rs-1; }
    if (((y%rs!=0)) && B[y-1]) { ny++; z=y-1; }
    if (((y%rs!=0)&&(y%ps<ps-rs)) && B[y-1+rs]) { ny++; z=y-1+rs; }
    if (((y%ps<ps-rs)) && B[y+rs]) { ny++; z=y+rs; }
    if (((y%ps<ps-rs)&&(y%rs!=rs-1)) && B[y+rs+1]) { ny++; z=y+rs+1; }
    if (((y>=ps)&&(y%rs!=rs-1)) && B[-ps+y+1]) { ny++; z=-ps+y+1; }
    if (((y>=ps)&&(y%rs!=rs-1)&&(y%ps>=rs)) && B[-ps+y+1-rs]) { ny++; z=-ps+y+1-rs; }
    if (((y>=ps)&&(y%ps>=rs)) && B[-ps+y-rs]) { ny++; z=-ps+y-rs; }
    if (((y>=ps)&&(y%ps>=rs)&&(y%rs!=0)) && B[-ps+y-rs-1]) { ny++; z=-ps+y-rs-1; }
    if (((y>=ps)&&(y%rs!=0)) && B[-ps+y-1]) { ny++; z=-ps+y-1; }
    if (((y>=ps)&&(y%rs!=0)&&(y%ps<ps-rs)) && B[-ps+y-1+rs]) { ny++; z=-ps+y-1+rs; }
    if (((y>=ps)&&(y%ps<ps-rs)) && B[-ps+y+rs]) { ny++; z=-ps+y+rs; }
    if (((y>=ps)&&(y%ps<ps-rs)&&(y%rs!=rs-1)) && B[-ps+y+rs+1]) { ny++; z=-ps+y+rs+1; }
    if (((y>=ps)) && B[-ps+y]) { ny++; z=-ps+y; }
    B[x] = M_OBJECT;
    if (ny >= 2) return 1;
    if ((ny == 1) && (mctopo3d_nbvoiso26(B, z, rs, ps, N) >= 3)) return 1;
    return 0;
  } // if (n == 1)
  else if ((n == 2) && voisins26(y, z, rs, ps))
  {
    if (mctopo3d_nbvoiso26(B, y, rs, ps, N) >= 3) return 1;
    if (mctopo3d_nbvoiso26(B, z, rs, ps, N) >= 3) return 1;
  }
  return 0;
} // TWIG()

/* ==================================== */
int32_t lmawanchangcurv2subfields2002(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmawanchangcurv2subfields2002"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[2];
  uint8_t v[27];

#ifdef DEBUG_lmawanchangcurv2subfields2002
  TEST_DIAG_deletable();
#endif

#define PATCH_MC
#ifdef PATCH_MC
  mctopo3d_init_topo3d();
#endif

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#define INITSTEP -1
  //#define INITSTEP 0
  step = INITSTEP;
  nonstab[0] = nonstab[1] = 1;
  while ((nonstab[0] || nonstab[1]) && (step < nsteps+INITSTEP))
  {
    step++;
    nonstab[step%2] = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = M_OBJECT;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (mctopo3d_nbvoisc6(S, x, rs, ps, N) >= 1))
	{
#ifdef DEBUG_lmawanchangcurv2subfields2002
printf("point %d %d %d\n", i, j, k);	  
#endif
	  extract_vois(S, x, rs, ps, N, v);
	  if (ORTH_deletable(v)) SET_ORTH(S[x]);
	  if (DIAG_deletable(v)) SET_DIAG(S[x]);
	  if (TWIG(S, x, rs, ps, N)) SET_TWIG(S[x]);
	} // if (S[x])
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && IS_DIAG(S[x]))
	{
	  extract_vois(S, x, rs, ps, N, v);
	  DIAG_preserve(v, x, S, rs, ps);
	} // if (S[x])
      } // for i, j, k

    //if (step%2) writeimage(image, "_S1"); else writeimage(image, "_S0");

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (IS_ORTH(S[x]) || (IS_DIAG(S[x]) && !IS_DIAGPRES(S[x])) || IS_TWIG(S[x]))
	{
#ifdef PATCH_MC
	  if (mctopo3d_simple26(S, x, rs, ps, N))
#endif
	  {
	    S[x] = 0;
	    nonstab[step%2] = 1;
	  }
	}
      } // for i, j, k

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  return(1);
} /* lmawanchangcurv2subfields2002() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Lohou et Bertrand (curviligne symmétrique, Pat. Rec. 2007) 
// M. Couprie, dec. 2011
/* ============================================================ */
/* ============================================================ */

/* ==================================== */
int32_t llohoubertrandsymcurv2007(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "llohoubertrandsymcurv2007"
{
  int32_t i, j, k, x, y, v;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); assert(t != NULL);
  uint8_t *T = UCHARDATA(t);
  struct xvimage *c = copyimage(image); assert(c != NULL);
  uint8_t *C = UCHARDATA(c);
  uint8_t *I = NULL;
  int32_t step, nonstab, top, topbar;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  if (inhibit != NULL) I = UCHARDATA(inhibit);

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
    
    razimage(c);
    for (k = 2; k < ds-2; k++) // prépare l'ensemble C des points candidats
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (T[x] && ((I==NULL) || !I[x]))
	  {
	    mctopo3d_top26(T, x, rs, ps, N, &top, &topbar);
	    if (topbar == 1)
	    {
	      for (v = 0; v < 12; v += 2)
	      {
		y = voisin6(x, v, rs, ps, N);
		if ((y != -1) && (T[y]))
		{
		  mctopo3d_top26(T, y, rs, ps, N, &top, &topbar);
		  if (topbar == 1)
		  {
		    T[y] = 0;
		    mctopo3d_top26(T, x, rs, ps, N, &top, &topbar);
		    T[y] = 1;
		    if (topbar != 1) break;
		  } // if (topbar y == 1)
		} // if ((y != -1) && (T[y]))
	      } // for (v = 0; v <= 10; v += 2)
	      if (v == 12) C[x] = 1;
	    } // if (topbar x == 1)
	  } // if (T[x])
	} // for i, j, k

    //#define DEBUG_llohoubertrandsymcurv2007
#ifdef DEBUG_llohoubertrandsymcurv2007
    writeimage(c, "_c");
#endif

    for (k = 2; k < ds-2; k++) // retire en parallèle les points PC_simples
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (C[x] && ((I==NULL) || !I[x]) && P_simple26(T, C, x, rs, ps, N))
	  {
	    S[x] = 0;
	    nonstab = 1;
	  }
	} // for k,j,i

    memcpy(T, S, N);
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  freeimage(t);
  freeimage(c);
  return(1);
} /* llohoubertrandsymcurv2007() */

/* ============================================================ */
/* ============================================================ */


/* global variables */
  long int           long_mask[26];
  unsigned char      char_mask[8];
  unsigned long int  neighbours;
  unsigned long int  neighbours_UD;
  unsigned long int  neighbours_NS;
  unsigned long int  neighbours_EW;
  unsigned long int  GLOBAL_direction;
  unsigned char     *lut_deletable;
  unsigned long int  size_x;
  unsigned long int  size_y;
  unsigned long int  size_z;
  unsigned long int  size_xy;
  unsigned long int  size_xyz;
  unsigned char     *image;
  List               SurfaceVoxels;
  List               BorderVoxels;
  unsigned long int  act_addr;
  struct xvimage *pgmimage;
  Bordercell *borderlist;
  int                DEL6NEI;
  unsigned long int  endflag;
  unsigned char      *lut_simple;
  unsigned long int  z_size_xy, zm_size_xy, zp_size_xy;
  unsigned long int  y_size_x, ym_size_x, yp_size_x;


//#define DEBUG
//#ifdef DEBUG
//int32_t trace = 1;
//#endif
//
//typedef uint32_t mat[3][3];
//typedef uint32_t vec[3];
//
//// matrices pour les 8 symétries de type 1
//// et translations associées
//
//mat R0 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//vec T0 = {0,0,0};
//
//mat R1 = {
//  { -1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//vec T1 = {-4,0,0};
//
//mat R2 = {
//  {  1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0,  1}
//};
//vec T2 = {0,-4,0};
//
//mat R3 = {
//  { -1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0,  1}
//};
//vec T3 = {-4,-4,0};
//
//mat R4 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0, -1}
//};
//vec T4 = {0,0,-4};
//
//mat R5 = {
//  { -1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0, -1}
//};
//vec T5 = {-4,0,-4};
//
//mat R6 = {
//  {  1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0, -1}
//};
//vec T6 = {0,-4,-4};
//
//mat R7 = {
//  { -1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0, -1}
//};
//vec T7 = {-4,-4,-4};
//
//// matrices pour les 6 symétries de type 2
//
//mat S0 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//
//mat S1 = {
//  {  1,  0,  0},
//  {  0,  0,  1},
//  {  0,  1,  0}
//};
//
//mat S2 = {
//  {  0,  1,  0},
//  {  1,  0,  0},
//  {  0,  0,  1}
//};
//
//mat S3 = {
//  {  0,  1,  0},
//  {  0,  0,  1},
//  {  1,  0,  0}
//};
//
//mat S4 = {
//  {  0,  0,  1},
//  {  1,  0,  0},
//  {  0,  1,  0}
//};
//
//mat S5 = {
//  {  0,  0,  1},
//  {  0,  1,  0},
//  {  1,  0,  0}
//};

///* ==================================== */
//static void multmatvec(
//  mat m,
//  vec v,
//  vec sol)
///* ==================================== */
//{
//  int32_t i, j, t;
//  for (i = 0; i < 3; i++)
//  {
//    t = 0;
//    for (j = 0; j < 3; j++)
//      t += m[i][j] * v[j];
//    sol[i] = t;
//  }
//} // multmatvec()
//
///* ==================================== */
//static int32_t isometry_coord(mat s1, vec t1, mat s2, vec x, vec tmp)
///* ==================================== */
//{
//  multmatvec(s1,x,tmp);
//  tmp[0] -= t1[0];
//  tmp[1] -= t1[1];
//  tmp[2] -= t1[2];
//  multmatvec(s2,tmp,x);
//  return x[2]*25 + x[1]*5 + x[0];
//} // isometry_coord()
//
///* ==================================== */
//static void isometry(uint8_t *dest, uint8_t *sour, mat s1, vec t1, mat s2)
///* ==================================== */
//{
//  vec x, tmp = {0,0,0};
//  int32_t i, j, k, p;
//  for (k = 0; k < 5; k++)
//    for (j = 0; j < 5; j++)
//      for (i = 0; i < 5; i++)
//      {
//	x[0] = i; x[1] = j; x[2] = k;
//	p = isometry_coord(s1, t1, s2, x, tmp);
//	dest[p] = sour[k*25 + j*5 + i];
//      }
//} // isometry()
//
///* ==================================== */
//static void extract_vois(
//  uint8_t *img,                    /* pointeur base image */
//  int32_t p,                       /* index du point */
//  int32_t rs,                      /* taille rangee */
//  int32_t ps,                      /* taille plan */
//  int32_t N,                       /* taille image */
//  uint8_t *vois)
///*
//  retourne dans "vois" les valeurs des 125 voisins de p, dans l'ordre suivant:
//
//  0   1   2   3   4
//  5   6   7   8   9
// 10  11  12  13  14
// 15  16  17  18  19
// 20  21  22  23  24
//
// 25  26  27  28  29
// 30  31  32  33  34
// 35  36  37  38  39
// 40  41  42  43  44
// 45  46  47  48  49
//
// 50  51  52  53  54
// 55  56  57  58  59
// 60  61  62  63  64
// 65  66  67  68  69
// 70  71  72  73  74
//
// 75  76  77  78  79
// 80  81  82  83  84
// 85  86  87  88  89
// 90  91  92  93 ...
//
// le tableau vois doit avoir été alloué
//
//*/
///* ==================================== */
//{
//#undef F_NAME
//#define F_NAME "extract_vois"
//  uint32_t xx, yy, zz, i, j, k;
//  if ((p%rs>=rs-2) || (p%ps<rs-1) || (p%rs<=1) || (p%ps>=ps-rs-rs) ||
//      (p < ps-1) || (p >= N-ps-ps)) /* point de bord */
//  {
//    printf("%s: ERREUR: point de bord\n", F_NAME);
//    exit(0);
//  }
//
//  zz = p / ps;
//  yy = (p % ps) / rs;
//  xx = p % rs;
//  for (k = 0; k < 5; k++)
//    for (j = 0; j < 5; j++)
//      for (i = 0; i < 5; i++)
//      {
//	vois[(k * 25) + (j * 5) + i] =
//	  img[((zz-2+k) * ps) + ((yy-2+j) * rs) + xx-2+i];
//      }
//
//} /* extract_vois() */
//
///* ==================================== */
//int32_t pala_match1(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] &&
//      !v[91] && !v[92] && !v[93])
//    return 1;
//  else
//    return 0;
//} // palagyi_match1()
//
///* ==================================== */
//int32_t pala_match2(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[67] && (!v[61] || !v[63]) && !v[86] && !v[87] && !v[88] &&
//      !v[81] && !v[82] && !v[83])
//    return 1;
//  else
//    return 0;
//} // palagyi_match2()
//
///* ==================================== */
//int32_t pala_match3(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[67] && v[61] && !v[57] && !v[63] && !v[87] && !v[88] &&
//      !v[82] && !v[83])
//    return 1;
//  else
//    return 0;
//} // palagyi_match3()
//
///* ==================================== */
//int32_t pala_match4(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[61] && v[63] && v[67] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] && !v[92])
//    return 1;
//  else
//    return 0;
//} // palagyi_match4()
//
///* ==================================== */
//int32_t pala_match5(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[61] && v[63] && v[66] && v[67] && v[68] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88])
//    return 1;
//  else
//    return 0;
//} // palagyi_match5()
//
///* ==================================== */
//int32_t pala_match6(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && v[57] && v[67] &&
//      !v[61] && !v[63] && !v[86] && !v[87] && !v[88])
//    return 1;
//  else
//    return 0;
//} // palagyi_match6()
//
///* ==================================== */
//int32_t palagyi_match1(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match1(vv)) return 1;
//
//  return 0;
//} // palagyi_match1()
//
///* ==================================== */
//int32_t palagyi_match2(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match2(vv)) return 1;
//
//  return 0;
//} // palagyi_match2()
//
///* ==================================== */
//int32_t palagyi_match3(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match3(vv)) return 1;
//
//  return 0;
//} // palagyi_match3()
//
///* ==================================== */
//int32_t palagyi_match4(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match4(vv)) return 1;
//
//  return 0;
//} // palagyi_match4()
//
///* ==================================== */
//int32_t palagyi_match5(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match5(vv)) return 1;
//
//  return 0;
//} // palagyi_match5()
//
///* ==================================== */
//int32_t palagyi_match6(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match6(vv)) return 1;
//
//  return 0;
//} // palagyi_match6()
//
///* ==================================== */
//int32_t lpalagyi(struct xvimage *image,
//		 int32_t nsteps)
///* ==================================== */
//#undef F_NAME
//#define F_NAME "lpalagyi"
//{
//  int32_t i, j, k, x;
//  int32_t rs = rowsize(image);     /* taille ligne */
//  int32_t cs = colsize(image);     /* taille colonne */
//  int32_t ds = depth(image);       /* nb plans */
//  int32_t ps = rs * cs;            /* taille plan */
//  int32_t N = ps * ds;             /* taille image */
//  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
//  struct xvimage *t = copyimage(image);
//  uint8_t *T = UCHARDATA(t);
//  int32_t step, nonstab;
//  uint8_t v[125];
//  uint8_t vv[125];
//
//#ifdef VERBOSE
//    printf("Warning: this is a very slow implementation. I might take time...\n");
//#endif
//
//  if (nsteps == -1) nsteps = 1000000000;
//
//  /* ================================================ */
//  /*               DEBUT ALGO                         */
//  /* ================================================ */
//
//  step = 0;
//  nonstab = 1;
//  while (nonstab && (step < nsteps))
//  {
//    nonstab = 0;
//    step++;
//#ifdef VERBOSE
//    printf("step %d\n", step);
//#endif
//    for (k = 2; k < ds-2; k++)
//      for (j = 2; j < cs-2; j++)
//	for (i = 2; i < rs-2; i++)
//	{
//	  x = k*ps + j*rs + i;
//	  if (T[x])
//	  {
//	    extract_vois(T, x, rs, ps, N, v);
//	    if (palagyi_match1(v, vv) || palagyi_match2(v, vv) ||
//		palagyi_match3(v, vv) || palagyi_match4(v, vv) ||
//		palagyi_match5(v, vv) || palagyi_match6(v, vv))
//	    {
//	      S[x] = 0;
//	      nonstab = 1;
//	    }
//	  }
//	} // for k,j,i
//    memcpy(T, S, N);
//  } // while (nonstab && (step < nsteps))
//
//#ifdef VERBOSE1
//    printf("number of steps: %d\n", step);
//#endif
//
//  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
//
//  freeimage(t);
//  return(1);
//} /* lpalagyi() */
//
//#ifdef PROBE
//#include <mckhalimsky3d.h>
//#include <l3dkhalimsky.h>
//
///* =============================================================== */
//int32_t main()
///* =============================================================== */
//{
//  struct xvimage * image;
//  int32_t i, j, k;
//  int32_t rs, cs, ds, ps, N, valmax;
//  uint8_t *Im;
//  struct xvimage * kh;
//  int32_t nbcc1, nbcav1, nbtun1, euler1;
//  int32_t nbcc2, nbcav2, nbtun2, euler2;
//
//  rs = 9;
//  cs = 9;
//  ds = 9;
//  ps = rs * cs;
//  N = ps * ds;
//  valmax = 5;
//  srand(time(NULL));
//  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
//  Im = UCHARDATA(image);
//  memset(Im, 0, N);
//
//  while(1)
//  {
//    for (k = 2; k < 7; k++)
//    for (j = 2; j < 7; j++)
//    for (i = 2; i < 7; i++)
//    {
//      Im[k*ps + j*rs + i] = (int8_t)((valmax+1.0)*rand() / (RAND_MAX + 1.0));
//    }
//    l3dkhalimskize(image, &kh, 0);
//    l3dinvariants(kh, &nbcc1, &nbcav1, &nbtun1, &euler1);
//    freeimage(kh);
//    lpalagyi(image, -1);
//    l3dkhalimskize(image, &kh, 0);
//    l3dinvariants(kh, &nbcc2, &nbcav2, &nbtun2, &euler2);
//    freeimage(kh);
//    if ((nbcc1 != nbcc2) || (nbcav1 != nbcav2) || (nbtun1 != nbtun2))
//    {
//      printf("BINGO !\n");
//      writeimage(image,"contre_exemple.pgm");
//      return 1;
//    }
//    printf("%d composantes connexes, ", nbcc1);
//    printf("%d cavites, ", nbcav1);
//    printf("%d tunnels\n", nbtun1);
//  }
//  return 0;
//} /* main */
//#endif

/*******************************************************************************
*
*       Author:              K. Palagyi (adapted by J. Chaussard for PINK)
*
*******************************************************************************/

/*========= function read_image =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      pgmimage
      size_x
      size_y
      size_z
      size_xy
      size_xyz
      image
----------------*/
void read_image()
{
	unsigned long int  i;

	image=(unsigned char*)pgmimage->image_data;
	size_x=rowsize(pgmimage);
	size_y=colsize(pgmimage);
	size_z=depth(pgmimage);
	size_xy=size_x*size_y;
	size_xyz=size_xy*size_z;

	if ( size_z <= 1 )
      	{
        	printf("ERROR: The image data is not a 3D one. Number of slices is %lu \n", size_z);
        	exit(-1);
      	}

	for(i=0; i<size_xyz; i++)
		if(UCHARDATA(pgmimage)[i]!=0) UCHARDATA(pgmimage)[i]=1; //Threshold
}
/*========= end of function read_image =========*/



/*========= function write_image =========*/
/* -------------
  considered global variables:
      pgmimage
      size_xyz
  changed global variables:
      image (data)
----------------*/
void write_image()
{
	unsigned long int  i;

	for(i=0; i<size_xyz; i++)
		if(UCHARDATA(pgmimage)[i]!=0) UCHARDATA(pgmimage)[i]=255; //Threshold
}

/*========= end of function write_image =========*/


/*========= function set_long_mask =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      long_mask
----------------*/
void set_long_mask( void )
  {
    long_mask[ 0] = 0x00000001;
    long_mask[ 1] = 0x00000002;
    long_mask[ 2] = 0x00000004;
    long_mask[ 3] = 0x00000008;
    long_mask[ 4] = 0x00000010;
    long_mask[ 5] = 0x00000020;
    long_mask[ 6] = 0x00000040;
    long_mask[ 7] = 0x00000080;
    long_mask[ 8] = 0x00000100;
    long_mask[ 9] = 0x00000200;
    long_mask[10] = 0x00000400;
    long_mask[11] = 0x00000800;
    long_mask[12] = 0x00001000;
    long_mask[13] = 0x00002000;
    long_mask[14] = 0x00004000;
    long_mask[15] = 0x00008000;
    long_mask[16] = 0x00010000;
    long_mask[17] = 0x00020000;
    long_mask[18] = 0x00040000;
    long_mask[19] = 0x00080000;
    long_mask[20] = 0x00100000;
    long_mask[21] = 0x00200000;
    long_mask[22] = 0x00400000;
    long_mask[23] = 0x00800000;
    long_mask[24] = 0x01000000;
    long_mask[25] = 0x02000000;
  }
/*========= end of function set_char_mask =========*/


/*========= function set_char_mask =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      char_mask
----------------*/
void set_char_mask( void )
  {
    char_mask[0] = 0x01;
    char_mask[1] = 0x02;
    char_mask[2] = 0x04;
    char_mask[3] = 0x08;
    char_mask[4] = 0x10;
    char_mask[5] = 0x20;
    char_mask[6] = 0x40;
    char_mask[7] = 0x80;
  }
/*========= end of function set_long_mask =========*/


/*============= function init_lut =============*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      lut_deletable
      long_mask
      char_mask
----------------*/
void init_lut( char* mask_name )
{
  char  lutname [100];
  FILE  *lutfile;

  /* alloc lut_deletable */
    lut_deletable = (unsigned char *)malloc(0x00400000);    /* 4 Mbyte */
    if ( lut_deletable == NULL)
      {
        printf("\n Alloc error!!!\n");
        exit(1);
      }  /* end if */

  /* open lutfile */
    strcpy( lutname, mask_name);
    lutfile = fopen( lutname, "rb");
    if ( lutfile == NULL)
      {
        printf("\n\n file %s is not found!!!\n", mask_name);
        exit(1);
      }  /* end if */

  /* reading lutfile */
    if ( fread( lut_deletable, 1, 0x00400000, lutfile) != 0x00400000 )      /* 4 Mbyte */
      {
         printf("ERROR: Couldn't read LUT\n");
         exit(1);
      }

    fclose(lutfile);

  /* setting masks */
    set_long_mask();
    set_char_mask();

}
/*=========== end of function init_lut ===========*/



/*==================================*/
/*========= list functions =========*/
/*==================================*/

void NewSurfaceVoxel( void ) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).addr=act_addr;
	(*LE).next=NULL;
	(*LE).prev=SurfaceVoxels.last;
	if (SurfaceVoxels.last!=NULL) (*((ListElement*)(SurfaceVoxels.last))).next=LE;
	SurfaceVoxels.last=LE;
	if (SurfaceVoxels.first==NULL) SurfaceVoxels.first=LE;
}

void RemoveSurfaceVoxel(ListElement * LE) {
ListElement * LE2;
	if (SurfaceVoxels.first==LE) SurfaceVoxels.first=(*LE).next;
	if (SurfaceVoxels.last==LE) SurfaceVoxels.last=(*LE).prev;
	if ((*LE).next!=NULL) {
		LE2=(ListElement*)((*LE).next);
		(*LE2).prev=(*LE).prev;
	}
	if ((*LE).prev!=NULL) {
		LE2=(ListElement*)((*LE).prev);
		(*LE2).next=(*LE).next;
	}
	free(LE);
}

void CreatePointList(PointList *s) {
	s->Head=NULL;
	s->Tail=NULL;
	s->Length=0;
}

void AddToList(PointList *s, ListElement * ptr) {
Cell * newcell;
	newcell=(Cell *)malloc(sizeof(Cell));
	newcell->addr=act_addr;
	newcell->ptr=ptr;
	newcell->next=NULL;
	if (s->Head==NULL) {
		s->Head=newcell;
		s->Tail=newcell;
		s->Length=1;
	}
	else {
		s->Tail->next=newcell;
		s->Tail=newcell;
		s->Length++;
	}
}

unsigned long int GetFromList(PointList *s, ListElement **ptr) {
unsigned long int curr_addr;
Cell *tmp;
	(*ptr)=NULL;
	if(s->Length==0) return 0;
	else {
	        curr_addr = s->Head->addr;
		(*ptr)=s->Head->ptr;
		tmp=(Cell *)s->Head->next;
		free(s->Head);
		s->Head=tmp;
		s->Length--;
		if(s->Length==0) {
			s->Head=NULL;
			s->Tail=NULL;
		}
                return curr_addr;
	}
}

void DestroyPointList(PointList *s) {
ListElement * ptr;
	while(s->Length>0) GetFromList(s, &ptr);
}

void CollectSurfaceVoxels(void) {
unsigned long int x,y,z, num;
unsigned long int z_size_xy, y_size_x;

  SurfaceVoxels.first = NULL;
  SurfaceVoxels.last  = NULL;
  num = 0;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          for(x=1; x<size_x-1; x++)
	    {
	      act_addr = x + y_size_x + z_size_xy;
              if ( *(image + act_addr ) )
                {
                  if (  ( *(image + act_addr     - size_x           ) ==0 ) ||
                        ( *(image + act_addr     + size_x           ) ==0 ) ||
                        ( *(image + act_addr              - size_xy ) ==0 ) ||
                        ( *(image + act_addr              + size_xy ) ==0 ) ||
                        ( *(image + act_addr + 1                    ) ==0 ) ||
                        ( *(image + act_addr - 1                    ) ==0 )    )
                     {
                        *(image + act_addr ) = 2;    /* surface point */
                        NewSurfaceVoxel();
		        num ++;
                     } /* endif */
                } /* endif */
            } /* endfor x */
        } /* endfor y */
    } /* endfor z */
#ifdef VERBOSE
  printf("\n Number of surface/border points in the original image: %lu\n", num);
#endif
}

/*===============================================================*/
/*========= functions concerning topological properties =========*/
/*===============================================================*/


/*========= function collect_26_neighbours =========*/
void collect_26_neighbours( void )
  {

    neighbours = 0x00000000;

    if ( GLOBAL_direction == PALAGYI_U )
      {
        /*  U
          indices in "neighbours":
           0  1  2     8  9 10     16  17  18   y-1
           3  .  4    11  . 12     19  20  21   y
           5  6  7    13 14 15     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[15];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[24];
      } /* endif U */
   else
      { /* not-U */
       if ( GLOBAL_direction == PALAGYI_D )
      {
        /*  D
          indices in "neighbours":
           16  17  18     8  9 10     0  1  2   y-1
           19  20  21    11  . 12     3  .  4   y
           22  23  24    13 14 15     5  6  7   y+1
          x-1 x x+1     x-1 x x+1   x-1  x  x+1
              z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[15];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 7];
      } /* endif D */
    else
      { /* not-D */
    if ( GLOBAL_direction == PALAGYI_N )
      {
        /*  N
          indices in "neighbours":
           5  6  7     3  .  4      0   1   2   y-1
          13 14 15    11  . 12      8   9  10   y
          22 23 24    19 20 21     16  17  18   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[21];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[18];
      } /* endif N */
    else
      { /* not-N */
     if ( GLOBAL_direction == PALAGYI_S )
      {
        /*  S
          indices in "neighbours":
          22 23 24    19 20 21     16  17  18   y-1
          13 14 15    11  . 12      8   9  10   y
           5  6  7     3  .  4      0   1   2   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[24];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[ 4];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 2];
      } /* endif S */
    else
      { /* not-S */
     if ( GLOBAL_direction == PALAGYI_E )
      {
        /*  E
          indices in "neighbours":
           2 10 18     1  9 17      0   8  16   y-1
           4 12 21     .  . 20      3  11  19   y
           7 15 24     6 14 23      5  13  22   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[23];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
      } /* endif E */
    else
      { /* not-E */
     if ( GLOBAL_direction == PALAGYI_W )
      {
        /*  W
          indices in "neighbours":
          18 10  2    17  9  1     16   8  0   y-1
          21 12  4    20  .  .     19  11  3   y
          24 15  7    23 14  6     22  13  5   y+1
          x-1 x x+1   x-1 x x+1    x-1  x x+1
             z-1          z           z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[ 6];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
      } /* endif W */
      } /* endelse not-E */
      } /* endelse not-S */
      } /* endelse not-N */
      } /* endelse not-D */
      } /* endelse not-U */


  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable( void )
{
  collect_26_neighbours();
  return ( ( *(lut_deletable + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function deletable =========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints(PointList *s) {
ListElement * LE3;
unsigned char maybe;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;

      maybe = 0;

      if ( GLOBAL_direction == PALAGYI_U )
        {
          if ( *(image + act_addr - size_xy ) == 0 )
            maybe = 1;
	} /* endif U */
      else
        { /* not-U */
      if ( GLOBAL_direction == PALAGYI_D )
        {
          if ( *(image + act_addr + size_xy ) == 0 )
            maybe = 1;
	}  /* endif D */
      else
        { /* not-D */
      if ( GLOBAL_direction == PALAGYI_N )
        {
          if ( *(image + act_addr - size_x  ) == 0 )
            maybe = 1;
	} /* endif N */
      else
        { /* not-N */
      if ( GLOBAL_direction == PALAGYI_S )
        {
          if ( *(image + act_addr + size_x  ) == 0 )
            maybe = 1;
	} /* endif S */
      else
        { /* not-S */
      if ( GLOBAL_direction == PALAGYI_E )
        {
          if ( *(image + act_addr - 1       ) == 0 )
            maybe = 1;
	} /* endif E */
      else
        { /* not_E */
      if ( GLOBAL_direction == PALAGYI_W )
        {
          if ( *(image + act_addr + 1       ) == 0 )
            maybe = 1;
	} /* endif W */
	} /* endelse not-E */
	} /* endelse not-S */
	} /* endelse not-N */
	} /* endelse not-D */
	} /* endelse not-U */

      if ( maybe )
        {
          if ( deletable() )
            {
	       AddToList(s,LE3);
            } /* endif */
        } /* endif */

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step(void)
{
  unsigned long int i, changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  for ( i=0; i<6; i++ )                          /* 6-subiteration alg. */
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectDeletablePoints(&s);
      while ( s.Length > 0 )
        {
           curr_addr = GetFromList( &s, &ptr );
           *(image + curr_addr ) = 0;            /* deleted */
	  changed ++;
           /* investigating v's 6-neighbours */
	      act_addr = curr_addr - 1;          /* (x-1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + 1;          /* (x+1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_x;     /* (x,y-1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_x;     /* (x,y+1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
            RemoveSurfaceVoxel(ptr);
        } /* endwhile */
      DestroyPointList(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step();
      iter++;
#ifdef VERBOSE
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
#endif
    }

}
/*========= end of function thinning =========*/


int palagyi_skelpar_curv_98(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_curv_98"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_curv_98.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: 6-subiteration directional curve-thinning ...\n", F_NAME);
#endif
    thinning();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}
/*===========================================================================
    end of function    m a i n
  ===========================================================================*/


/*============= function init_lut_simple =============*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      lut_simple
      long_mask
      char_mask
----------------*/
void init_lut_simple( char* mask_name )
{
  char  lutname [100];
  FILE  *lutfile;

  /* alloc lut_simple */
    lut_simple = (unsigned char *)malloc(0x00800000);
    if ( lut_simple == NULL)
      {
        printf("\n Alloc error!!!\n");
        exit(1);
      }  /* end if */

  /* open lutfile */
    strcpy( lutname, mask_name);
    lutfile = fopen( lutname, "rb");
    if ( lutfile == NULL)
      {
        printf("\n\n file %s is not found!!!\n", mask_name);
        exit(1);
      }  /* end if */

  /* reading lutfile */
    fread( lut_simple, 1, 0x00800000, lutfile);
    fclose(lutfile);

  /* setting masks */
    set_long_mask();
    set_char_mask();

}
/*=========== end of function init_lut_simple ===========*/


void NewSurfaceVoxel2(unsigned long int x,
                     unsigned long int y,
		     unsigned long int z) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).x=x;
	(*LE).y=y;
	(*LE).z=z;
	(*LE).next=NULL;
	(*LE).prev=SurfaceVoxels.last;
	if (SurfaceVoxels.last!=NULL) (*((ListElement*)(SurfaceVoxels.last))).next=LE;
	SurfaceVoxels.last=LE;
	if (SurfaceVoxels.first==NULL) SurfaceVoxels.first=LE;
}


void AddToList2(PointList *s,Voxel e, ListElement * ptr) {
Cell * newcell;
	newcell=(Cell *)malloc(sizeof(Cell));
	newcell->v=e;
	newcell->ptr=ptr;
	newcell->next=NULL;
	if (s->Head==NULL) {
		s->Head=newcell;
		s->Tail=newcell;
		s->Length=1;
	}
	else {
		s->Tail->next=newcell;
		s->Tail=newcell;
		s->Length++;
	}
}

Voxel GetFromList2(PointList *s, ListElement **ptr) {
Voxel R;
Cell *tmp;
        R.i = -1;
        R.j = -1;
        R.k = -1;
        R.oldnei = 0;    /*** +field ***/
	(*ptr)=NULL;
	if(s->Length==0) return R;
	else {
		R=s->Head->v;
		(*ptr)=s->Head->ptr;
		tmp=(Cell *)s->Head->next;
		free(s->Head);
		s->Head=tmp;
		s->Length--;
		if(s->Length==0) {
			s->Head=NULL;
			s->Tail=NULL;
		}
		return R;
	}
}

void CollectSurfaceVoxels2(void) {
unsigned long int x,y,z;

  SurfaceVoxels.first = NULL;
  SurfaceVoxels.last  = NULL;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      zm_size_xy = z_size_xy - size_xy;
      zp_size_xy = z_size_xy + size_xy;
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          ym_size_x  = y_size_x - size_x;
          yp_size_x  = y_size_x + size_x;
          for(x=1; x<size_x-1; x++)
            if ( *(image + x + y_size_x + z_size_xy ) )
              {
                if (  ( *(image +   x + ym_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image +   x + yp_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image +   x +  y_size_x + zm_size_xy ) ==0 ) ||
                      ( *(image +   x +  y_size_x + zp_size_xy ) ==0 ) ||
                      ( *(image + x+1 +  y_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image + x-1 +  y_size_x +  z_size_xy ) ==0 )    )
                   {
                      *(image + x + y_size_x + z_size_xy ) = 2;
                      NewSurfaceVoxel2(x,y,z);
                   } /* endif */
              } /* endif */
        } /* endfor y */
    } /* endfor z */

}

void DestroyPointList2(PointList *s) {
ListElement * ptr;
	while(s->Length>0) GetFromList2(s, &ptr);
}

/*===============================================================*/
/*========= functions concerning topological properties =========*/
/*===============================================================*/

/*========= function collect_26_neighbours =========*/
void collect_26_neighbours2(unsigned long int x,
                           unsigned long int y,
                           unsigned long int z )
  {
    /*
      indices in "neighbours":
      0  1  2     9 10 11     17 18 19    y-1
      3  4  5    12    13     20 21 22     y
      6  7  8    14 15 16     23 24 25    y+1
     x-1 x x+1   x-1 x x+1    x-1 x x+1
        z-1          z           z+1
    */

    z_size_xy  = z*size_xy;
    zm_size_xy = z_size_xy - size_xy;
    zp_size_xy = z_size_xy + size_xy;
    y_size_x   = y*size_x;
    ym_size_x  = y_size_x  - size_x;
    yp_size_x  = y_size_x  + size_x;

    neighbours = 0x00000000;
    endflag = 0;

    if ( *(image + x-1 + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 0];
        endflag++;
      }
    if ( *(image +   x + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 1];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 2];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 3];
        endflag++;
      }
    if ( *(image +   x +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 4];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 5];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 6];
        endflag++;
      }
    if ( *(image +   x + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 7];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 8];
        endflag++;
      }

    if ( *(image + x-1 + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[ 9];
        endflag++;
      }
    if ( *(image +   x + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[10];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[11];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[12];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[13];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[14];
        endflag++;
      }
    if ( *(image +   x + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[15];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[16];
        endflag++;
      }

    if ( *(image + x-1 + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[17];
        endflag++;
      }
    if ( *(image +   x + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[18];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[19];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[20];
        endflag++;
      }
    if ( *(image +   x +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[21];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[22];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[23];
        endflag++;
      }
    if ( *(image +   x + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[24];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[25];
        endflag++;
      }

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function simple_26_6 =========*/
int simple_26_62(unsigned long int x,
                unsigned long int y,
                unsigned long int z )
{
  collect_26_neighbours2(x,y,z);
  return ( ( *(lut_simple + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function simple_26_6 =========*/


/*=========== function DetectSimpleBorderPoints ===========*/
void DetectSimpleBorderPoints2(PointList *s) {
unsigned char value;
Voxel v;
ListElement * LE3;
unsigned long int x, y, z;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {
      x = (*LE3).x;
      y = (*LE3).y;
      z = (*LE3).z;
      y_size_x   = y*size_x;
      ym_size_x  = y_size_x - size_x;
      yp_size_x  = y_size_x + size_x;
      z_size_xy  = z*size_xy;
      zm_size_xy = z_size_xy - size_xy;
      zp_size_xy = z_size_xy + size_xy;
      switch( GLOBAL_direction )
        {
          case PALAGYI_U: value = *(image + x   + ym_size_x + z_size_xy  );
                  break;
          case PALAGYI_D: value = *(image + x   + yp_size_x + z_size_xy  );
                  break;
          case PALAGYI_N: value = *(image + x   + y_size_x  + zm_size_xy );
                  break;
          case PALAGYI_S: value = *(image + x   + y_size_x  + zp_size_xy );
                  break;
          case PALAGYI_E: value = *(image + x+1 + y_size_x  + z_size_xy  );
                  break;
          case PALAGYI_W: value = *(image + x-1 + y_size_x  + z_size_xy  );
                  break;
        } /* endswitch */
      if( value == 0 )
        {
          if ( simple_26_62(x, y, z) )
            {
              v.i = x;
              v.j = y;
              v.k = z;
              v.oldnei = neighbours;     /*** storing oldnei ***/
              AddToList2(s,v,LE3);
            } /* endif */
        } /* endif */
      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectSimpleBorderPoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step2(void)
{
  unsigned long int i, changed, deletable, diffnei, del6nei;
  ListElement * ptr;
  PointList s;
  Voxel v;

  changed = 0;
  for ( i=0; i<6; i++ )
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectSimpleBorderPoints2(&s);
      while ( s.Length > 0 )
        {
           v = GetFromList2( &s, &ptr );
           deletable = 0;
           if ( simple_26_62( v.i, v.j, v.k ) )
             {
               if ( endflag > 1 )
                 {
                   deletable = 1;    /*** simple and not endpoint ***/
                 }
               else                  /*** endpoint ***/
                 {
                   diffnei = v.oldnei ^ neighbours;
                   del6nei = 0;
                   if ( diffnei & long_mask[ 4] )
                     del6nei++;
                   if ( diffnei & long_mask[10] )
                     del6nei++;
                   if ( diffnei & long_mask[12] )
                     del6nei++;
                   if ( diffnei & long_mask[13] )
                     del6nei++;
                   if ( diffnei & long_mask[15] )
                     del6nei++;
                   if ( diffnei & long_mask[21] )
                     del6nei++;
                   if ( del6nei >= DEL6NEI )
                     deletable = 1;          /*** deletable endpoint found ***/
                 }
               if ( deletable )
                 {
                    z_size_xy = (v.k)*size_xy;
                    y_size_x  = (v.j)*size_x;
                    *(image + v.i + y_size_x + z_size_xy ) = 0; /* simple point is deleted */
                    changed ++;
                    /* investigating v's 6-neighbours */
                      if (*(image + v.i-1 + y_size_x + z_size_xy                )==1)
                        {
                          NewSurfaceVoxel2( v.i-1, v.j  , v.k   );
                          *(image + v.i-1 + y_size_x + z_size_xy                ) = 2;
                        }
                      if (*(image + v.i+1 + y_size_x        + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i+1, v.j  , v.k   );
                          *(image + v.i+1 + y_size_x        + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x-size_x + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j-1, v.k   );
                          *(image + v.i   + y_size_x-size_x + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x+size_x + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j+1, v.k   );
                          *(image + v.i   + y_size_x+size_x + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x        + z_size_xy-size_xy )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j  , v.k-1 );
                          *(image + v.i   + y_size_x        + z_size_xy-size_xy ) = 2;
                        }
                      if (*(image + v.i   + y_size_x        + z_size_xy+size_xy )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j  , v.k+1 );
                          *(image + v.i   + y_size_x        + z_size_xy+size_xy ) = 2;
                        }
                    RemoveSurfaceVoxel(ptr);
                 } /* endif */
             } /* endif */
        } /* endwhile */
      DestroyPointList2(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function sequential_thinning =========*/
void sequential_thinning2(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels2();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step2();
      iter++;
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
    }

}
/*========= end of function sequential_thinning =========*/


int palagyi_skelpar_curv_06(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_curv_06"
	char tablefilename[512];
	pgmimage=input;
	DEL6NEI = 1;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_curv_06.dat", getenv("PINK"));
    init_lut_simple(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: Centerline extraction by thinning ...\n", F_NAME);
#endif
    sequential_thinning2();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_simple);


    return(0);
}

/*========= function collect_26_neighbours =========*/
void collect_26_neighbours3( void )
  {

    neighbours = 0x00000000;

    if ( GLOBAL_direction == PALAGYI_UD )
      {
        /*  UD
          indices in "neighbours":
           0  1  2     9 10 11     17  18  19   y-1
           3  4  5    12    13     20      21   y
           6  7  8    14 15 16     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[16];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[24];

      } /* endif UD */

    else
      {

    if ( GLOBAL_direction == PALAGYI_NS )
      {
        /*  NS
          indices in "neighbours":
          6   7   8      3   4  5      0   1   2    y-1
         14  15  16     12     13      9  10  11     y
         22  23  24     20     21     17  18  19    y+1
         x-1  x x+1     x-1 x  x+1     x-1  x x+1
            z-1              z              z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr              - size_xy ) )
	  neighbours |= long_mask[15];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[21];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[19];

      } /* endif NS */

    else
      {

    if ( GLOBAL_direction == PALAGYI_EW )
      {
        /*  EW
          indices in "neighbours":
          2  11  19     1  10  18      0   9  17    y-1
          5  13  21     4              3  12  20     y
          8  16  24     7  15  23      6  14  22    y+1
         x-1  x x+1    x-1  x x+1     x-1  x  x+1
            z-1             z              z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr              - size_xy ) )
	  neighbours |= long_mask[13];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr     - size_x           ) )
	  neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr     + size_x           ) )
	  neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[23];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];

      } /* endif EW */
      } /* endelse */
      } /* endelse */

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable3( void )
{
  collect_26_neighbours3();
  return ( ( *(lut_deletable + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function deletable =========*/


/*=========== function UpdateSurfaceList ===========*/
void UpdateSurfaceList(PointList *r) {
ListElement *LE3;
ListElement *ptr;
unsigned char surface_end;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;
      surface_end = 0;

      if (  ( *(image + act_addr - 1 ) == 0 ) &&
            ( *(image + act_addr + 1 ) == 0 )     )
        {
           surface_end = 1;
	}
      else
        {  /* 1 */
          if (  ( *(image + act_addr - size_x ) == 0 ) &&
                ( *(image + act_addr + size_x ) == 0 )     )
            {
               surface_end = 1;
	    }
          else
            {  /* 2*/
              if (  ( *(image + act_addr - size_xy ) == 0 ) &&
                    ( *(image + act_addr + size_xy ) == 0 )     )
                {
                   surface_end = 1;
	        }
	    }  	/* 2 */
	}  /* 1 */

      if ( surface_end  )
        AddToList(r,LE3);

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

  while ( r->Length > 0 )
    {
       GetFromList( r, &ptr );
       RemoveSurfaceVoxel(ptr);
    } /* endwhile */

}
/*=========== end of function UpdateSurfaceList ===========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints3(PointList *s) {
ListElement * LE3;
unsigned char maybe;
PointList r;

  /* update suurface list */
    CreatePointList(&r);
    UpdateSurfaceList(&r);
    DestroyPointList(&r);


  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;

      maybe = 0;
      if ( GLOBAL_direction == PALAGYI_UD )
        {
           if ( ( ( *(image + act_addr - size_xy           ) == 0 ) &&
	          ( *(image + act_addr + size_xy           )      ) &&
		  ( *(image + act_addr + size_xy + size_xy )      )     )
		                                                                  ||
		( ( *(image + act_addr + size_xy           ) == 0 ) &&
	          ( *(image + act_addr - size_xy           )      )     )    )
          maybe = 1;
	}
      else
        {
      if ( GLOBAL_direction == PALAGYI_NS )
        {
	   if ( ( ( *(image + act_addr - size_x            ) == 0 ) &&
	          ( *(image + act_addr + size_x            )      ) &&
		  ( *(image + act_addr + size_x + size_x   )      )     )
		                                                                  ||
		( ( *(image + act_addr + size_x            ) == 0 ) &&
	          ( *(image + act_addr - size_x            )      )     )    )
          maybe = 1;
	}
      else
        {
      if ( GLOBAL_direction == PALAGYI_EW )
        {
           if ( ( ( *(image + act_addr - 1                 ) == 0 ) &&
	          ( *(image + act_addr + 1                 )      ) &&
		  ( *(image + act_addr + 2                 )      )     )
		                                                                  ||
		( ( *(image + act_addr + 1                 ) == 0 ) &&
	          ( *(image + act_addr - 1                 )      )     )    )
          maybe = 1;
	}
	}  /* endelse */
	}  /* endelse */

      if ( maybe )
        {
          if ( deletable3() )
            {
               AddToList(s,LE3);
            } /* endif */
        } /* endif */

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step3(void)
{
  unsigned long int i, changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  for ( i=0; i<3; i++ )   /* 3-subiteration alg. */
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectDeletablePoints3(&s);
      while ( s.Length > 0 )
        {
           curr_addr = GetFromList( &s, &ptr );
           *(image + curr_addr ) = 0;            /* deleted */
           changed ++;
           /* investigating v's 6-neighbours */
	      act_addr = curr_addr - 1;          /* (x-1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + 1;          /* (x+1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_x;     /* (x,y-1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_x;     /* (x,y+1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
            RemoveSurfaceVoxel(ptr);
        } /* endwhile */
      DestroyPointList(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning3(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step3();
      iter++;
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
    }

}
/*========= end of function thinning =========*/



int palagyi_skelpar_surf_02(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_surf_02"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_surf_02.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: 3-subiteration directional surface-thinning ...\n", F_NAME);
#endif
    thinning3();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}
/*===========================================================================
    end of function    m a i n
  ===========================================================================*/


void NewBorderVoxel( void ) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).addr=act_addr;
	(*LE).next=NULL;
	(*LE).prev=BorderVoxels.last;
	if (BorderVoxels.last!=NULL) (*((ListElement*)(BorderVoxels.last))).next=LE;
	BorderVoxels.last=LE;
	if (BorderVoxels.first==NULL) BorderVoxels.first=LE;
}

void RemoveBorderVoxel(ListElement * LE) {
ListElement * LE2;
	if (BorderVoxels.first==LE) BorderVoxels.first=(*LE).next;
	if (BorderVoxels.last==LE) BorderVoxels.last=(*LE).prev;
	if ((*LE).next!=NULL) {
		LE2=(ListElement*)((*LE).next);
		(*LE2).prev=(*LE).prev;
	}
	if ((*LE).prev!=NULL) {
		LE2=(ListElement*)((*LE).prev);
		(*LE2).next=(*LE).next;
	}
	free(LE);
}

void CollectBorderVoxels(void) {
unsigned long int x,y,z, num;
unsigned long int z_size_xy, y_size_x;

  BorderVoxels.first = NULL;
  BorderVoxels.last  = NULL;
  num = 0;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          for(x=1; x<size_x-1; x++)
	    {
	      act_addr = x + y_size_x + z_size_xy;
              if ( *(image + act_addr ) )
                {
                  if (  ( *(image + act_addr     - size_x           ) ==0 ) ||
                        ( *(image + act_addr     + size_x           ) ==0 ) ||
                        ( *(image + act_addr              - size_xy ) ==0 ) ||
                        ( *(image + act_addr              + size_xy ) ==0 ) ||
                        ( *(image + act_addr + 1                    ) ==0 ) ||
                        ( *(image + act_addr - 1                    ) ==0 )    )
                     {
                        *(image + act_addr ) = 2;    /* Border point */
                        NewBorderVoxel();
		        num ++;
                     } /* endif */
                } /* endif */
            } /* endfor x */
        } /* endfor y */
    } /* endfor z */

  printf("\n Number of border points in the original image: %lu\n", num);
}


/*========= function collect_26_neighbours =========*/
void collect_26_neighbours4( void )
  {

    neighbours_UD = 0x00000000;
    neighbours_NS = 0x00000000;
    neighbours_EW = 0x00000000;

        /*

          indices in "neighbours_UD":
           0  1  2     9 10 11     17  18  19   y-1
           3  4  5    12    13     20   *  21   y
           6  7  8    14 15 16     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

          indices in "neighbours_NS":
           6  7  8     3  4  5      0   1   2    y-1
          14 15 16    12    13      9  10  11    y
          22 23 24    20  * 21     17  18  19    y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

          indices in "neighbours_EW":
          17  9  0    18 10  1     19  11   2    y-1
          20 12  3     *     4     21  13   5    y
          22 14  6    23 15  7     24  10   8    y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 0];
	     neighbours_NS |= long_mask[ 6];
	     neighbours_EW |= long_mask[17];
	  }
        if ( *(image + act_addr     - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 1];
	     neighbours_NS |= long_mask[ 7];
	     neighbours_EW |= long_mask[ 9];
	  }
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 2];
	     neighbours_NS |= long_mask[ 8];
	     neighbours_EW |= long_mask[ 0];
	  }
        if ( *(image + act_addr - 1          - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 3];
	     neighbours_NS |= long_mask[14];
	     neighbours_EW |= long_mask[20];
	  }
        if ( *(image + act_addr              - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 4];
	     neighbours_NS |= long_mask[15];
	     neighbours_EW |= long_mask[12];
	  }
        if ( *(image + act_addr + 1          - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 5];
	     neighbours_NS |= long_mask[16];
	     neighbours_EW |= long_mask[ 3];
	  }
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 6];
	     neighbours_NS |= long_mask[22];
	     neighbours_EW |= long_mask[22];
	  }
        if ( *(image + act_addr     + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 7];
	     neighbours_NS |= long_mask[23];
	     neighbours_EW |= long_mask[14];
	  }
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 8];
	     neighbours_NS |= long_mask[24];
	     neighbours_EW |= long_mask[ 6];
	  }
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
	  {
             neighbours_UD |= long_mask[ 9];
	     neighbours_NS |= long_mask[ 3];
	     neighbours_EW |= long_mask[18];
	  }
        if ( *(image + act_addr     - size_x           ) )
	  {
             neighbours_UD |= long_mask[10];
	     neighbours_NS |= long_mask[ 4];
	     neighbours_EW |= long_mask[10];
	  }
        if ( *(image + act_addr + 1 - size_x           ) )
	  {
             neighbours_UD |= long_mask[11];
	     neighbours_NS |= long_mask[ 5];
	     neighbours_EW |= long_mask[ 1];
	  }
        if ( *(image + act_addr - 1                    ) )
	  {
             neighbours_UD |= long_mask[12];
	     neighbours_NS |= long_mask[12];
	   /*neighbours_EW  not considered*/
	  }
        if ( *(image + act_addr + 1                    ) )
	  {
             neighbours_UD |= long_mask[13];
	     neighbours_NS |= long_mask[13];
	     neighbours_EW |= long_mask[ 4];
	  }
        if ( *(image + act_addr - 1 + size_x           ) )
	  {
             neighbours_UD |= long_mask[14];
	     neighbours_NS |= long_mask[20];
	     neighbours_EW |= long_mask[23];
	  }
        if ( *(image + act_addr     + size_x           ) )
	  {
             neighbours_UD |= long_mask[15];
	   /*neighbours_NS not considered*/
	     neighbours_EW |= long_mask[15];
	  }
        if ( *(image + act_addr + 1 + size_x           ) )
	  {
             neighbours_UD |= long_mask[16];
	     neighbours_NS |= long_mask[21];
	     neighbours_EW |= long_mask[ 7];
	  }
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[17];
	     neighbours_NS |= long_mask[ 0];
	     neighbours_EW |= long_mask[19];
	  }
        if ( *(image + act_addr     - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[18];
	     neighbours_NS |= long_mask[ 1];
	     neighbours_EW |= long_mask[11];
	  }
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[19];
	     neighbours_NS |= long_mask[ 2];
	     neighbours_EW |= long_mask[ 2];
	  }
        if ( *(image + act_addr - 1          + size_xy ) )
	  {
             neighbours_UD |= long_mask[20];
	     neighbours_NS |= long_mask[ 9];
	     neighbours_EW |= long_mask[21];
	  }
        if ( *(image + act_addr              + size_xy ) )
	  {
           /*neighbours_UD not considered*/
	     neighbours_NS |= long_mask[10];
	     neighbours_EW |= long_mask[13];
	  }
        if ( *(image + act_addr + 1          + size_xy ) )
	  {
             neighbours_UD |= long_mask[21];
	     neighbours_NS |= long_mask[11];
	     neighbours_EW |= long_mask[ 5];
	  }
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[22];
	     neighbours_NS |= long_mask[17];
	     neighbours_EW |= long_mask[24];
	  }
        if ( *(image + act_addr     + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[23];
	     neighbours_NS |= long_mask[18];
	     neighbours_EW |= long_mask[16];
	  }
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[24];
	     neighbours_NS |= long_mask[19];
	     neighbours_EW |= long_mask[ 8];
	  }

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable4( void )
{
  collect_26_neighbours4();


  /* UD test */
    if  (  ( (
               ( *(image + act_addr              - size_xy )==0 )
               &&
               ( *(image + act_addr              + size_xy )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr              - size_xy )==1 )
	       &&
               ( *(image + act_addr              + size_xy )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_UD>>3) ) ) & char_mask[neighbours_UD%8]
	   )
	 )
      return ( 1 );
  /* NS test */
    if  (  ( (
               ( *(image + act_addr     - size_x           )==0 )
               &&
               ( *(image + act_addr     + size_x           )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr     - size_x           )==1 )
	       &&
               ( *(image + act_addr     + size_x           )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_NS>>3) ) ) & char_mask[neighbours_NS%8]
	   )
	 )
      return ( 1 );
  /* EW test */
    if  (  ( (
               ( *(image + act_addr - 1                    )==0 )
               &&
               ( *(image + act_addr + 1                    )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr - 1                    )==1 )
	       &&
               ( *(image + act_addr + 1                    )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_EW>>3) ) ) & char_mask[neighbours_EW%8]
	   )
	 )
      return ( 1 );

   return ( 0 );
}

/*=========== function UpdateSurfaceList ===========*/
void UpdateSurfaceList4(PointList *r) {
ListElement *LE3;
ListElement *ptr;
unsigned char int_nei_exists;

  LE3=(ListElement *)BorderVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;
      int_nei_exists = 0;

      if ( *(image + act_addr - 1 ) == 1 )
        {
           int_nei_exists = 1;
	}
      else
        {  /* 1 */
          if ( *(image + act_addr + 1 ) == 1 )
            {
              int_nei_exists = 1;
	    }
          else
            {  /* 2*/
              if ( *(image + act_addr - size_x ) == 1 )
                {
                  int_nei_exists = 1;
	        }
              else
                {  /* 3 */
                  if ( *(image + act_addr + size_x ) == 1 )
                    {
                      int_nei_exists = 1;
   	            }
                  else
                    {  /* 4 */
                      if ( *(image + act_addr - size_xy ) == 1 )
                        {
                          int_nei_exists = 1;
   	                }
                      else
                        {  /* 5 */
                          if ( *(image + act_addr + size_xy ) == 1 )
                            {
                              int_nei_exists = 1;
   	                    }
	                }  /* 5 */
	            }   /* 4 */
		}   /* 3 */
	    }  	/* 2 */
	}  /* 1 */

      if ( int_nei_exists == 0 )
        AddToList(r,LE3);

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

  while ( r->Length > 0 )
    {
       GetFromList( r, &ptr );
       RemoveBorderVoxel(ptr);
    } /* endwhile */

}
/*=========== end of function UpdateSurfaceList ===========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints4(PointList *s) {
ListElement * LE3;
PointList r;

  /* update suurface list */
    CreatePointList(&r);
    UpdateSurfaceList4(&r);
    DestroyPointList(&r);

  LE3=(ListElement *)BorderVoxels.first;

  while (LE3!=NULL)
    {
      act_addr = (*LE3).addr;

      if ( deletable4() )
        AddToList(s,LE3);

      LE3=(ListElement *)(*LE3).next;

    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step4(void)
{
  unsigned long int changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  CreatePointList(&s);
  DetectDeletablePoints4(&s);

  while ( s.Length > 0 )
    {
       curr_addr = GetFromList( &s, &ptr );
       *(image + curr_addr ) = 0;            /* deleted */
       changed ++;
       /* investigating v's 6-neighbours */
	  act_addr = curr_addr - 1;          /* (x-1,y,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr + 1;          /* (x+1,y,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr - size_x;     /* (x,y-1,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
          act_addr = curr_addr + size_x;     /* (x,y+1,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
          act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
        RemoveBorderVoxel(ptr);
    } /* endwhile */

  DestroyPointList(&s);

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning4(void)
{
  unsigned int iter, changed;

  CollectBorderVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step4();
      iter++;
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
    }

}
/*========= end of function thinning =========*/


int palagyi_skelpar_surf_08(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_surf_08"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_surf_08.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: fully parallel surface-thinning ...\n", F_NAME);
#endif
    thinning4();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}
