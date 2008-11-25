/* $Id: segmentarea.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file segmentarea.c

\brief segmentation by filtering components on an area criterion

<B>Usage:</B> segmentarea in.pgm connex area out.pgm

<B>Description:</B>
Segmentation by selection of cross-section components, based on an area criterion. 
Connexity = <B>connex</B> ; area threshold = <B>area</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B>  connect
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
#include <lattribarea.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex area [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex area [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentarea: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentarea(image, connex, param, 0))
    {
      fprintf(stderr, "segmentarea: lsegmentarea failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lsegmentarea(image, connex, param, 1))
    {
      fprintf(stderr, "segmentarea: lsegmentarea failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



