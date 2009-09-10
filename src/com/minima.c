/* $Id: minima.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file minima.c

\brief regional minima 

<B>Usage:</B> in.pgm connex out.pgm

<B>Description:</B> 
Selects the regional minima of a grayscale image with connexity <B>connex</B>.

<B>Types supported:</B> byte 2d, int32_t 2d, byte 3d, int32_t 3d

<B>Category:</B> connect, topogray
\ingroup  connect, topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelextrema.h>

//#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, connex, i;
  struct xvimage * image;
  struct xvimage * result;
  uint8_t * I;
  int32_t * IL;
  int32_t N;
  int32_t * R;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm (connex = 0[min. absolus],4,8, b0, b1, 14[grille biconnecte 3D]) \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "minima: readimage failed\n");
    exit(1);
  }
  N = rowsize(image) * colsize(image) * depth(image);

  if (argv[2][0] == 'b') {
    connex = atoi(argv[2]+1);
  } else 
    connex = atoi(argv[2]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  R = SLONGDATA(result);

  if ((connex == 0) && strcmp(argv[2], "b0") && strcmp(argv[2], "b1"))
  {
    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      uint8_t absmin;
      I = UCHARDATA(image);
      absmin = I[0];
      for (i = 1; i < N; i++) if (I[i] < absmin) absmin = I[i];
      for (i = 0; i < N; i++) if (I[i] == absmin) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      int32_t absmin;
      IL = SLONGDATA(image);
      absmin = IL[0];
      for (i = 1; i < N; i++) if (IL[i] < absmin) absmin = IL[i];
      for (i = 0; i < N; i++) if (IL[i] == absmin) IL[i] = (int32_t)NDG_MAX; 
                                             else IL[i] = (int32_t)NDG_MIN;
    }
  }
  else
  {
    if (! llabelextrema(image, connex, LABMIN, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }

#ifdef VERBOSE
    printf("%s : NOMBRE DE MINIMA : %d\n", argv[0], nblabels-1);
#endif

    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      I = UCHARDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      IL = SLONGDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) IL[i] = (int32_t)NDG_MAX; else IL[i] = (int32_t)NDG_MIN;
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


