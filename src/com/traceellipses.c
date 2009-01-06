/* $Id: traceellipses.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file traceellipses.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Trace des ellipses dont les specifications 
  (coord centre, extremite grand axe (coord. relatives au centre), extremite petit axe (idem)) 
  sont lues dans un fichier.
  Format du fichier: 
  <nb ellipse>
  <ell1>
  .
  .
  .


  Utilise l'algorithme de Bresenham generalise (voir lbresen)

  Michel Couprie - nov. 1998
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lbresen.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i, x1, y1, x2, y2, xc, yc, x, y;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t nellipse;
  ellipse *ell;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm ellipses.txt [fill] out.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 5) && strcmp(argv[3], "fill"))
  {
    fprintf(stderr, "usage: %s in.pgm ellipses.txt [fill] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  if (d > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }
  N = rs * cs;
  Im = UCHARDATA(image);

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  ell = AllocEllipseList(rs, cs);
  
  fscanf(fd, "%d", &nellipse);

  for (i = 0; i < nellipse; i++)
  {
    fscanf(fd, "%d%d%d%d%d%d", &xc, &yc, &x1, &y1, &x2, &y2);
    if (argc == 4)
      lellipse(Im, rs, cs, x1, y1, x2, y2, xc, yc);
    else
    {
      lellipselist(ell, rs, cs, x1, y1, x2, y2, xc, yc);
      for (y = 0; y < cs; y++) 
      {
        for (x = ell->xmin[y]; x <= ell->xmax[y]; x++)
          Im[y*rs + x] = NDG_MAX;
      }
    }
  }
  fclose(fd);
  FreeEllipseList(ell);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

