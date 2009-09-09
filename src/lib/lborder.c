/* $Id: lborder.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* frontiere d'une image binaire */
/*
   Soit X un sous-ensemble de Z2, (n,nb) = (4,8) ou (8,4) ou (26,6)
   la frontiere de F(X) de X est definie par:
   F(X) = {x de X, Gamma_nb(x) inter Xbar non vide}
*/
/* Michel Couprie -  avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <lborder.h>

/* ==================================== */
int32_t lborder(struct xvimage *image, int32_t connex)
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *I = UCHARDATA(image);     /* l'image de depart */
  uint32_t *IL = ULONGDATA(image);    /* l'image de depart si 32 bits */
  struct xvimage *tmp;
  uint8_t *T;

  switch(datatype(image))
  {
    case VFF_TYP_1_BYTE:
      tmp = copyimage(image);
      if (tmp == NULL)
      {
        fprintf(stderr, "lborder: copyimage failed\n");
        return 0;
      }
      T = UCHARDATA(tmp);
      switch (connex)
      {
        case 4: 
          for (x = 0; x < N; x++)
            if (T[x] && (nbvoisc8(T,x,rs,N)) == 0) I[x] = NDG_MIN;
          break;
        case 8: 
          for (x = 0; x < N; x++)
            if (T[x] && (nbvoisc4(T,x,rs,N)) == 0) I[x] = NDG_MIN;
          break;
        case 6: 
          for (x = 0; x < N; x++)
            if (T[x] && nonbord3d(x,rs,ps,N) && (nbvoisc6(T,x,rs,ps,N)) == 0) I[x] = NDG_MIN;
          break;
        case 26: 
          for (x = 0; x < N; x++)
            if (T[x] && nonbord3d(x,rs,ps,N) && (nbvoisc26(T,x,rs,ps,N)) == 0) I[x] = NDG_MIN;
          break;
        default: 
          fprintf(stderr, "lborder: mauvaise connexite: %d\n", connex);
          return 0;
      } /* switch (connex) */  
      break;
    case VFF_TYP_4_BYTE:
      tmp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
      if (tmp == NULL)
      {
        fprintf(stderr, "lborder: allocimage failed\n");
        return 0;
      }
      T = UCHARDATA(tmp);
      memset(T, 0, N);
      switch (connex)
      {
        case 4: 
          for (x = 0; x < N; x++)
            if (nbvois8neq(IL,x,rs,N) != 0) T[x] = NDG_MAX;
          break;
        case 8: 
          for (x = 0; x < N; x++)
            if (nbvois4neq(IL,x,rs,N) != 0) T[x] = NDG_MAX;
          break;
        default: 
          fprintf(stderr, "lborder: mauvaise connexite: %d\n", connex);
          return 0;
      } /* switch (connex) */
      for (x = 0; x < N; x++) IL[x] = (uint32_t) T[x];
      break;
    default:
      fprintf(stderr,"lborder() : bad data type %d\n", datatype(image));
      return 0;
  } /* switch(datatype(image)) */

  freeimage(tmp);
  return(1);
}
/* -library_code_end */
