/* $Id: extrude.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file extrude.c

\brief catenates a sequence of 2d (resp. 1d) images, made of the repetition of the same image, into a 3d (resp. 2d) image

<B>Usage:</B> extrude in.pgm nbrep [xdim ydim zdim] out.pgm

<B>Description:</B>
Catenates a sequence of 2d (resp. 1d) images, made of the repetition of the same image, into a 3d (resp. 2d) image.

<B>Types supported:</B> byte 1d, 2d

<B>Category:</B>  convert
\ingroup   convert

\author Michel Couprie
*/

/* 
  Michel Couprie - mars 2008
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t i, j, x, seqsize;
  struct xvimage * image_in;
  struct xvimage * image_out;
  int32_t rs, cs, N;
  uint8_t *I;
  uint8_t *O;

  if ((argc != 4) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm nbrep [xdim ydim zdim] out.pgm\n", argv[0]);
    exit(1);
  }

  image_in = readimage(argv[1]);
  if (image_in == NULL)
  {
    fprintf(stderr, "%s: readimage failed: %s\n", argv[0], argv[1]);
    exit(1);
  }
  rs = rowsize(image_in);   /* taille ligne */
  cs = colsize(image_in);   /* taille colonne */
  N = rs * cs;              /* taille image */
  I = UCHARDATA(image_in);
  seqsize = atoi(argv[2]);

  if (cs > 1)
    image_out = allocimage(NULL, rs, cs, seqsize, VFF_TYP_1_BYTE);
  else
    image_out = allocimage(NULL, rs, seqsize, 1, VFF_TYP_1_BYTE);

  if (image_out == NULL)
  {   
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }
  O = UCHARDATA(image_out);

  for (j = 1; j < seqsize; j++)
    for (i = 0; i < N; i++) O[j * N + i] = I[i];

  if (argc == 7)
  {
    image_out->xdim = atof(argv[4]);
    image_out->ydim = atof(argv[5]);
    image_out->zdim = atof(argv[6]);
  }
  else
    image_out->xdim = image_out->ydim = image_out->zdim = 0.0;

  writeimage(image_out, argv[argc-1]);
  freeimage(image_out);
  freeimage(image_in);
  return 0;
} /* main() */
