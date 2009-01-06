/* $Id: setorigin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file setorigin.c

\brief adds origin to file header

<B>Usage:</B> setorigin in.pgm [source_se | xorig yorig zorig] 

<B>Description:</B> Adds origin (the one of \b se or \b xorig \b yorig \b zorig ) 
to file header of \b in.pgm.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t x, y, z;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s image [source_se | xorig yorig zorig] \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    struct xvimage * se;
    se = readse(argv[2], &x, &y, &z);  
    freeimage(se);
  }
  else
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    z = atoi(argv[4]);
  }

  writese(image, argv[1], x, y, z);
  freeimage(image);

  return 0;
} /* main */


