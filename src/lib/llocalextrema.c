/* $Id: llocalextrema.c,v 1.2 2008-12-15 06:38:50 mcouprie Exp $ */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros3d.h>
#include <ldilateros.h>
#include <llocalextrema.h>

/****************************************************************
*
* Routine Name: llocalextrema - library call for localextrema
*
* Purpose: etiquetage des extrema d'une image 2d (en 4 ou 8 connexite) 
*          ou 3d (en 6, 18 ou 26 connexite) par des labels differents
*          x est localmax if dilat(F)[x] == F[x]
*          x est localmin if erod(F)[x] == F[x]
*
*         Supporte VFF_TYP_1_BYTE et VFF_TYP_4_BYTE
*
* Input:
* Output:
* Written By: Laurent Najman
****************************************************************/
int32_t llocalextrema(
        struct xvimage *img, /* image de depart */
        int32_t connex,          /* 4, 8 (2d) ou 6, 18, 26 (3d) */
        int32_t minimum,         /* booleen */
        struct xvimage *max) /* resultat: image des extrema mis a 255 */
/* ==================================== */
{
  struct xvimage *imglong, *maxlong;
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t n = rs * cs;          /* taille plan */
  int32_t N = n * d;            /* taille image */
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  uint32_t *F;
  uint32_t *M;
  uint8_t *F2;
  uint8_t *M2;
  uint32_t extr;
  int32_t nbvois;
  uint8_t *mask;

  if ((rowsize(max) != rs) || (colsize(max) != cs) || (depth(max) != d))
  {
    fprintf(stderr, "llocalextrema: tailles images incompatibles\n");
    return 0;
  }

  if (datatype(img) != datatype(max))
  {
    fprintf(stderr, "llocalextrema: datatypes images incompatibles\n");
    return 0;
  }
  if (datatype(img) == VFF_TYP_4_BYTE) {
    F = ULONGDATA(img);
    M = ULONGDATA(max);
  } else if (datatype(img) == VFF_TYP_1_BYTE) {
    imglong = allocimage(NULL, rowsize(img), colsize(img), depth(img), VFF_TYP_4_BYTE);
    if (imglong == NULL)
      {   
	fprintf(stderr, "llocalextrema: allocimage failed\n");
	exit(1);
      }
    F = ULONGDATA(imglong);

    // Copy image
    F2 = UCHARDATA(img);
    for (x=0; x<N; x++)
      F[x] = (int32_t)(F2[x]);

    maxlong = allocimage(NULL, rowsize(img), colsize(img), depth(img), VFF_TYP_4_BYTE);
    if (maxlong == NULL)
      {   
	fprintf(stderr, "llocalextrema: allocimage failed\n");
	exit(1);
      }
    M = ULONGDATA(maxlong);
  } else {
    fprintf(stderr, "llocalextrema: datatype images not supported\n");
    return 0;
  }

  if (d == 1) 
    nbvois = 8;
  else nbvois = 26;

  switch(connex) {
  case 4: mask = vois4; break;
  case 8: mask = vois8; break;
  case 6: mask = vois6; break;
  case 18: mask = vois18; break;
  case 26: mask = vois26; break;
  default: fprintf(stderr, "llocalextrema: connex=%d not implemented\n", 
		   connex);
  }

  for (x = 0; x < N; x++)
  {
    extr = F[x];
    for (k = 0; k < nbvois; k += 1)
    {
      if (mask[k])                      /* element structurant */
      {
	if (d==1)
	  y = voisin(x, k, rs, N);
	else
	  y = voisin26(x, k, rs, n, N);
	if (minimum) {
	  if ((y != -1) && (F[y] < extr)) extr = F[y];
	} else {
	  if ((y != -1) && (F[y] > extr)) extr = F[y];
	}
      }
    } /* for k */
    if (F[x] == extr)
      M[x] = 255; 
    else
      M[x] = 0;
  }

  if (datatype(img) == VFF_TYP_1_BYTE) {
    M2 = UCHARDATA(max);
    for (x=0; x<N; x++)
      M2[x] = (int32_t) (M[x]);
    freeimage(maxlong);
    freeimage(imglong);
  } 
  return 1;
}
