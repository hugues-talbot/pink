/* $Id: t26pp.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file t26pp.c

\brief computes the connecivity number t26pp for each image voxel

<B>Usage:</B> t26pp in out

<B>Description:</B> 
Computes the connecivity number t26pp (as defined in ref. BEC97) for each image voxel.

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

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lt26pp(image))
  {
    fprintf(stderr, "%s: function lt26pp failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */
