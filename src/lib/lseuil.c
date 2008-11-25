/* $Id: lseuil.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateurs de seuillage */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lseuil.h>

/* ==================================== */
int32_t lseuil(
        struct xvimage *f,
        double seuil)
/* ==================================== */
/* tous les pixels < seuil sont mis a 0, les autres a 255 */
{
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb. plans */
  int32_t N = rs * cs * ds;        /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++) if (F[x] < seuil) F[x] = NDG_MIN; else F[x] = NDG_MAX;  
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    uint32_t *FL = ULONGDATA(f);
    for (x = 0; x < N; x++) if (FL[x] < seuil) FL[x] = NDG_MIN; else FL[x] = NDG_MAX;  
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *FF = FLOATDATA(f);
    for (x = 0; x < N; x++) if (FF[x] < seuil) FF[x] = 0.0; else FF[x] = 1.0;  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }
  return 1;
}

/* ==================================== */
int32_t lseuil2(
        struct xvimage *f,
        uint8_t seuilmin,
        uint8_t seuilmax,
        uint8_t valmin,
        uint8_t valmax)
/* ==================================== */
/* tous les pixels < seuilmin sont mis a valmin */
/* tous les pixels >= seuilmax sont mis a valmax */
{
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t d = depth(f);            /* nb. plans */
  int32_t N = rs * cs * d;         /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = valmax;
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = (uint32_t)valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = (uint32_t)valmax;
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(f);
    for (x = 0; x < N; x++)
      if ((int32_t)(F[x]) < (int32_t)seuilmin) F[x] = (float)valmin;  
      else if ((int32_t)(F[x]) >= (int32_t)seuilmax) F[x] = (float)valmax;
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }

  return 1;
}


/* ==================================== */
int32_t lseuil3(
        struct xvimage *f,
        double seuil, double seuil2)
/* ==================================== */
/* tous les seuil <= pixels < seuil2 sont mis a 255, les autres a 0 */
{
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb. plans */
  int32_t N = rs * cs * ds;        /* taille image */

  if (datatype(f) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(f);
    for (x = 0; x < N; x++) 
      if ((F[x] < seuil2) && (F[x]>=seuil))
	F[x] = NDG_MAX; 
      else F[x] = NDG_MIN;  
  }
  else if (datatype(f) == VFF_TYP_4_BYTE)
  {
    uint32_t *FL = ULONGDATA(f);
    for (x = 0; x < N; x++) 
      if ((FL[x] < seuil2) && (FL[x]>=seuil))
	FL[x] = NDG_MAX; 
      else FL[x] = NDG_MIN;  
  }
  else if (datatype(f) == VFF_TYP_FLOAT)
  {
    float *FF = FLOATDATA(f);
    for (x = 0; x < N; x++) 
      if ((FF[x] < seuil2) && (FF[x]>=seuil))
	FF[x] = 1.0; 
      else FF[x] = 0.0;  
  }
  else
  {   fprintf(stderr,"lseuil() : bad datatype : %d\n", datatype(f));
      return 0;
  }
  return 1;
}
