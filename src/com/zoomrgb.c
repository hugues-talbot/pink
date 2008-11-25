/* $Id: zoomrgb.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file zoomrgb.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* zoom une image en couleurs */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lzoom.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;
  struct xvimage * go1;
  struct xvimage * go2;
  struct xvimage * go3;
  int32_t rs;
  double z;

  if ((argc < 3) || (argc > 4))
  {
    fprintf(stderr, "usage: %s in.ppm [z] out.ppm\n", argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &g1, &g2, &g3) == 0)
  {
    fprintf(stderr, "rgb2bw: readrgbimage failed\n");
    exit(1);
  }

  rs = g1->row_size;

  if (argc == 4)
    z = atof(argv[2]);
  else
  {
    /* la valeur par defaut est choisie pour donner rs(out) = 50 */
    z = 50.0 / rs;
  }

  if ((! lzoom(g1, &go1, z, z, z)) || (! lzoom(g2, &go2, z, z, z)) || (! lzoom(g3, &go3, z, z, z)))
  {
    fprintf(stderr, "zoom: function lzoom failed\n");
    exit(1);
  }

  if (argc == 4)
    writergbimage(go1, go2, go3, argv[3]);
  else
    writergbimage(go1, go2, go3, argv[2]);

  freeimage(g1);
  freeimage(g2);
  freeimage(g3);
  freeimage(go1);
  freeimage(go2);
  freeimage(go3);

  return 0;
} /* main */
