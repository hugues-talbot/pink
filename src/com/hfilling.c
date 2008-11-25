/* $Id: hfilling.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file hfilling.c

\brief homotopic filling (pseudo closing) by a ball

<B>Usage:</B> hfilling in.pgm r connex out.pgm

<B>Description:</B>
Homotopic filling (pseudo closing) by a ball of radius \b r.

Reference:<BR> 
[CB04] M. Couprie and G. Bertrand:
<A HREF="http://www.esiee.fr/~coupriem/Pdf/cb04.pdf">Topology preserving alternating sequential filter for smoothing 
2D and 3D objects</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho, topobin
\ingroup  morpho, topobin

\author Michel Couprie 2002

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lasft.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex, r;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm r connex out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  r = atoi(argv[2]);
  connex = atoi(argv[3]);

  if (depth(image) == 1)
  {
    if (! lhpclosingdisc(image, connex, r))
    {
      fprintf(stderr, "%s: lhpclosingdisc failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lhpclosing3dball(image, connex, r))
    {
      fprintf(stderr, "%s: lhpclosing3dball failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
