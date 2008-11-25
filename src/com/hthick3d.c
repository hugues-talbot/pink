/* $Id: hthick3d.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file hthick3d.c

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
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;
  int32_t connex;
  int32_t alpha = 0;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connex niter <alpha|delta> fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 26) && (connex != 6))
  {
    fprintf(stderr, "%s : connex = <6|26>\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[4]);

  if (strcmp(argv[5],"alpha") == 0) 
    alpha = 1;
  else
  if (strcmp(argv[5],"delta") != 0) 
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connex niter <alpha|delta> fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (alpha)
  {
    fprintf(stderr, "%s: alpha NYI \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lhthickdelta3d(image, imagecond, nitermax, connex))
  {
    fprintf(stderr, "%s: lhthickdelta3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[6]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
