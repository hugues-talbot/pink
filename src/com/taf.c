/* $Id: taf.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file taf.c

\brief topological alternating filter

<B>Usage:</B> taf in.pgm connexmin r [lambdap [lambdaw]] out.pgm

<B>Description:</B>
Topological alternating filter. Performs alternatively topological upper filter (tuf)
and topological lower filter (tlf) with increasing radius until radius <B>r</B>.

If given, parameter <B>lambdap</B> (resp. <B>lambdaw</B>) is an integer which 
indicates that peaks (resp. wells) of height greater than this value must be 
preserved. 

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
/* 
   Filtre topologique alterne 

   Michel Couprie - janvier 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltrestopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayon;
  int32_t connexmin;
  int32_t lambdapics = 255;
  int32_t lambdapuits = 255;

  if ((argc != 5) && (argc != 6)  && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm connexmin rayon [lambdapics [lambdapuits]] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connexmin = atoi(argv[2]);
  rayon = atoi(argv[3]);

  if (argc >= 6) lambdapics = lambdapuits = atoi(argv[4]);
  if (argc == 7) lambdapuits = atoi(argv[5]);

  if (! ltaflambda(image, connexmin, rayon, lambdapics, lambdapuits))
  {
    fprintf(stderr, "%s: function ltaflambda failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
