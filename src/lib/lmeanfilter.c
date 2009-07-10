/* $Id: lmeanfilter.c,v 1.1 2009-07-10 05:43:29 mcouprie Exp $ */
/*
Mean Filter

Let F be the function stored in image \b in.pgm.
Let R be the function stored in image \b roi.pgm (region of interest).
Let I be the function stored in image \b inhibit.pgm (if any).
The algorithm is the following:

Repeat niter times
  G = F
  For each image point x
    If R[x] and not I[x] then
      S = G[x]; C = 1;
      For each connex-neighbour y of x
        If R[y] then S = S + G[y]; C = C + 1;
      F[x] = S / C;
Result: F

 */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lmeanfilter.h>

/* ==================================== */
int32_t lmeanfilter(
		    struct xvimage *image,  /* donnee: image ndg */       
		    struct xvimage *roi,    /* donnee: image binaire */
		    struct xvimage *inhib,  /* donnee: image binaire */
		    int32_t niter           /* donnee: entier */
		   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lmeanfilter"
{ 
  int32_t rs = rowsize(image);
  int32_t cs = colsize(image);
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ds * ps;
  int32_t x, y, k, C;
  struct xvimage *auximage;
  float *F;
  float *G;
  uint8_t *R;
  uint8_t *I = NULL;
  float S;
  
  if ((rowsize(roi) != rs) || (colsize(roi) != cs) || (depth(roi) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    return 0;
  }
  
  if ((inhib != NULL) && 
      ((rowsize(inhib) != rs) || (colsize(inhib) != cs) || (depth(inhib) != ds)))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    return 0;
  }

  if (datatype(image) != VFF_TYP_FLOAT)
  {
    fprintf(stderr, "%s: image type must be float\n", F_NAME);
    return 0;
  }

  if (datatype(roi) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: roi image type must be int8_t\n", F_NAME);
    return 0;
  }

  if ((inhib != NULL) && 
      (datatype(inhib) != VFF_TYP_1_BYTE))
  {
    fprintf(stderr, "%s: inhib image type must be int8_t\n", F_NAME);
    return 0;
  }
  
  auximage = copyimage(image);

  F = FLOATDATA(image);
  G = FLOATDATA(auximage);
  R = UCHARDATA(roi);
  if (inhib != NULL) I = UCHARDATA(inhib);

  while (niter > 0)
  {
    niter--;
    copy2image(auximage, image);
    if (ds == 1)
    {
      for (x = 0; x < N; x++) 
      {
	if (R[x] && ((I == NULL) || !I[x]))
	{
	  S = G[x]; C = 1;
	  for (k = 0; k < 8; k += 2)
	  {
	    y = voisin(x, k, rs, N);
	    if ((y != -1) && R[y])
	    {
	      S = S + G[y]; 
	      C = C + 1;
	    }
	  }
	  F[x] = S / C;
	} // if (R[x] && !I[x])
      } // for (i = 0; i < N; i++)
    }
    else // 3D
    {
      printf("3D NYI\n");
      exit(0);
    }
  } // while (niter > 0)

  freeimage(auximage);
  return(1);
} // lmeanfilter()
