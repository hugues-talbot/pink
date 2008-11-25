/* $Id: deletewells.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file deletewells.c

\brief deletes wells in a grayscale image 

<B>Usage:</B> deletewells filein.pgm [mask] connexmin fileout.pgm

<B>Description:</B>
Deletes wells in a grayscale image. A point p is a well if T-(p) = 0
(see ref. [BEC97]).

References:<BR> 
[BEC97] G. Bertrand, J. C. Everat and M. Couprie: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag97.ps.gz">"Image segmentation through operators based upon topology"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.<BR>

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltrestopo.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * mask;
  int32_t connexmin;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask] connexmin fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n");
      exit(1);
    }
    connexmin = atoi(argv[3]);
  }
  else
  {
    mask = NULL;
    connexmin = atoi(argv[2]);
  }

  if ((connexmin == 4) || (connexmin == 8))
  {
    if (! ldespuits(image, mask, connexmin))
    {
      fprintf(stderr, "%s: function ldespuits failed\n");
      exit(1);
    }
  }
  else if ((connexmin == 6) || (connexmin == 26))
  {
    if (! ldespuits3d(image, mask, connexmin))
    {
      fprintf(stderr, "%s: function ldespuits failed\n");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connexmin (must be 4, 8, 6 or 26)\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
