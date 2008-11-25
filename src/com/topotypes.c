/* $Id: topotypes.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file topotypes.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* calcul des types topologiques des points d' une image */
/* Michel Couprie - juillet 1996 */

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
  struct xvimage * in;
  struct xvimage * out;
  int32_t rs, cs, bin, connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm bin connex out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "topotypes: readimage failed\n");
    exit(1);
  }

  bin = atoi(argv[2]);
  connex = atoi(argv[3]);

  rs = in->row_size;
  cs = in->col_size;
  out = allocimage(NULL,rs, cs, 1, VFF_TYP_1_BYTE);
  if (out == NULL)
  {
    fprintf(stderr, "topotypes: allocimage failed\n");
    exit(1);
  }

  if (! ltopotypes(in, out, connex, bin))
  {
    fprintf(stderr, "topotypes: function ltopotypes failed\n");
    exit(1);
  }
  
  writeimage(out, argv[argc - 1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
