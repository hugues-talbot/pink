/* $Id: maxima.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file maxima.c

\brief regional maxima 

<B>Usage:</B> in.pgm connex out.pgm

<B>Description:</B> 
Selects the regional maxima of a grayscale image with connexity <B>connex</B>.

<B>Types supported:</B> byte 2d, int32_t 2d, byte 3d, int32_t 3d

<B>Category:</B> connect, topogray
\ingroup  connect, topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t nblabels, connex, i;
  struct xvimage * image;
  struct xvimage * result;
  uint8_t * I;
  uint32_t * IL;
  int32_t N;
  uint32_t * R;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm (connex = 0[max. absolus],4,8) \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "maxima: readimage failed\n");
    exit(1);
  }
  N = rowsize(image) * colsize(image) * depth(image);

  connex = atoi(argv[2]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  R = ULONGDATA(result);

  if (connex == 0)
  {
    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      uint8_t absmax;
      I = UCHARDATA(image);
      absmax = I[0];
      for (i = 1; i < N; i++) if (I[i] > absmax) absmax = I[i];
      for (i = 0; i < N; i++) if (I[i] == absmax) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      uint32_t absmax;
      IL = ULONGDATA(image);
      absmax = IL[0];
      for (i = 1; i < N; i++) if (IL[i] > absmax) absmax = IL[i];
      for (i = 0; i < N; i++) if (IL[i] == absmax) IL[i] = NDG_MAX; else IL[i] = NDG_MIN;
    }
  }
  else
  {
    if (! llabelextrema(image, connex, LABMAX, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }

    printf("%s : NOMBRE DE MAXIMA : %d\n", argv[0], nblabels-1);

    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      I = UCHARDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      IL = ULONGDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) IL[i] = (uint32_t)NDG_MAX; else IL[i] = (uint32_t)NDG_MIN;
    }
  }
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    struct xvimage *im2;
    uint8_t *I2;
    im2 = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (im2 == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    I2 = UCHARDATA(im2);
    for (i = 0; i < N; i++) I2[i] = (uint8_t)IL[i];
    writeimage(im2, argv[3]);
  }
  else
    writeimage(image, argv[3]);
  freeimage(result);
  freeimage(image);

  return 0;
} /* main */


