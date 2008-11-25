/* $Id: alpha.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file alpha.c

\brief computes the connecivity number alpha for each image voxel

<B>Usage:</B> alpha in out

<B>Description:</B> 
Computes the connecivity number alpha (as defined in ref. BEC97) for each image voxel.

<B>Types supported:</B> byte 3D

<B>Category:</B> topogray
\ingroup  topogray

References:

[BEC97] G. Bertrand, J. C. Everat and M. Couprie: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag97.ps.gz">"Image segmentation through operators based upon topology"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.

[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

\author Michel Couprie
*/
/* Michel Couprie - mai 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ltopotypes.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex;
  char sign;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm connex <m|p> out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  sign = argv[3][0];

  if (! lalpha(image, connex, sign))
  {
    fprintf(stderr, "%s: function lalpha failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */
