/* $Id: skel2pgm.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file skel2pgm.c

\brief generation of a binary image from a curvilinear skeleton

<B>Usage:</B> skel2pgm in.skel out.pgm

<B>Description:</B>
Generation of a binary image from a curvilinear skeleton.

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2004
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  skel * S;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.skel fileout.pgm\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }

  //  printskel(S);

  if (! (image = lskel2image(S)))
  {
    fprintf(stderr, "%s: function lskel2image failed\n", argv[0]);
    exit(1);
  }


  writeimage(image, argv[argc-1]);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
