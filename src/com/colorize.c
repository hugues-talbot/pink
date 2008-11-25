/* $Id: colorize.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file colorize.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* genere une image en couleurs a partir d'une image en gris et d'une LookUp Table */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * in;
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;
  uint8_t r[256]; /* la LookUp Table */
  uint8_t g[256];
  uint8_t b[256];
  int32_t rs, cs, N;
  int32_t i;
  uint8_t v;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);

  if (in == NULL)
  {
    fprintf(stderr, "randrgb: readimage failed\n");
    exit(1);
  }

  rs = in->row_size;
  cs = in->col_size;
  N = rs * cs;

  g1 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  g2 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  g3 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if ((g1 == NULL) || (g2 == NULL) || (g3 == NULL))
  {
    fprintf(stderr, "randrgb: allocimage failed\n");
    exit(1);
  }

  /* init la LUT */
  r[0] = g[0] = b[0] = 0;
  r[1] = 255; g[1] =   0; b[1] =   0; /* rouge */
  r[2] =   0; g[2] = 255; b[2] =   0; /* vert */
  r[3] =   0; g[3] =   0; b[3] = 255; /* bleu */
  r[4] = 255; g[4] = 255; b[4] =   0; /* jaune */

  for (i = 0; i < N; i++)
  {
    v = (UCHARDATA(in))[i];
    (UCHARDATA(g1))[i] = r[v];
    (UCHARDATA(g2))[i] = g[v];
    (UCHARDATA(g3))[i] = b[v];
  }
  
  writergbimage(g1, g2, g3, argv[2]);
  freeimage(in);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
