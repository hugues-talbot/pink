/* $Id: segbi.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file segbi.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsegbi.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  double cmin = 1.0;
  double cmax = 1.0;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [cmin cmax] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segbi: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (argc == 6)
  {
    cmin = atof(argv[3]);
    cmax = atof(argv[4]);
  }

  if (! lsegbihomotopique(image, connex, cmin, cmax))
  {
    fprintf(stderr, "segbi: lsegbi failed\n");
    exit(1);
  }

  if (argc == 6) writeimage(image, argv[5]); else writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
