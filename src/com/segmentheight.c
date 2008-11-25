/* $Id: segmentheight.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file segmentheight.c

\brief segmentation based on a height criterion

<B>Usage:</B> segmentheight in.pgm connex height [r] out.pgm

<B>Description:</B>
Height segmentation with connexity <B>connex</B> and depth <B>height</B>, 
followed by a reconstruction (2D only) if option <B>r</B> is set (see reference: to appear)
or a maximization if option <B>m</B> is set.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribheight.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex height [r|m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "r") != 0) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex height [r|m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentheight(image, connex, param, 0))
    {
      fprintf(stderr, "%s: lsegmentheight failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (strcmp(argv[4], "m") == 0)
  {
    if (! lsegmentheight(image, connex, param, 1))
    {
      fprintf(stderr, "%s: lsegmentheight failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lsegreconsheight(image, connex, param))
    {
      fprintf(stderr, "%s: lsegreconsheight failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



