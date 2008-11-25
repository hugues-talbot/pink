/* $Id: htkern3dbin.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file htkern3dbin.c

\brief homotopic thinning or kernel of a 3D binary image

<B>Usage:</B> htkern3dbin im.pgm connex nimax out.pgm

<B>Description:</B>
Homotopic thinning or kernel of a 3D binary image. 
Uses a breadth-first strategy. 
The connectivity of the object is given by <B>connex</B>.
The maximum number of iterations is given by <B>nimax</B>,
if equal to -1 stability is reached.

<B>Types supported:</B> byte 3d

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
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
  int32_t connex;
  int32_t nimax;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm connex nimax fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connex = <6|26>\n", argv[0]);
    exit(1);
  }

  nimax = atoi(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lhtkern3dbin(image, connex, nimax))
  {
    fprintf(stderr, "%s: lhtkern3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
