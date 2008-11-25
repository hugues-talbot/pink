/* $Id: border.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file border.c

\brief border of a binary image

<B>Usage:</B> border in connex out

<B>Description:</B>
Let X be the subset of Zn which corresponds to the input image \b in, and 
let k be the value of the parameter \b connex (that is, 4, 8 or 6(3d)).
The border B(X) of X is defined by:
B(X) = {x in X, (Gamma_k(x) inter Xbar) not empty}

<B>Types supported:</B> byte 2d, byte 3d

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
#include <lborder.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "border: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lborder(image, connex))
  {
    fprintf(stderr, "border: lborder failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
