/* $Id: segmentvol.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file segmentvol.c

\brief segmentation by filtering components on a volume criterion

<B>Usage:</B> segmentvol in.pgm connex vol out.pgm

<B>Description:</B>
Segmentation by selection of cross-section components, based on a 
volume criterion. 
Connexity = <B>connex</B> ; volume threshold = <B>vol</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribvol.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex vol [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex vol [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentvol: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentvol(image, connex, param, 0))
    {
      fprintf(stderr, "segmentvol: lsegmentvol failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lsegmentvol(image, connex, param, 1))
    {
      fprintf(stderr, "segmentvol: lsegmentvol failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



