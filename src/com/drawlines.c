/* $Id: drawlines.c,v 1.3 2008-12-11 13:46:16 mcouprie Exp $ */
/*! \file drawlines.c

\brief draw line segments which are specified by a text file

<B>Usage:</B> drawlines in.pgm vect.txt out.pgm

<B>Description:</B>
The file \b vect.txt contains a list of line segments under the format:<br>
l nb_segments<br>
x11 y11 x12 y12<br>
x21 y21 x22 y22<br>
x31 y31 x32 y32<br>
...<br>

<B>Types supported:</B> byte 2D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/
/* 
  Trace des vecteurs dont les specifications (pt debut, pt fin) 
  sont lues dans un fichier. En entete: le nombre de vecteurs.

  Utilise l'algorithme de Bresenham.

  Michel Couprie - juin 1998 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>

//#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i, j, X1, Y1, X2, Y2, X, Y;
  double x1, y1, x2, y2;
  FILE *fd = NULL;
  int32_t nvect, n;
  int32_t *lx, *ly;
  char tag;
  int32_t rs, cs;
  uint8_t *F;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm vect.txt out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  F = UCHARDATA(image);

  lx = (int32_t *)malloc((rs+cs) * sizeof(int32_t));
  ly = (int32_t *)malloc((rs+cs) * sizeof(int32_t));
  if (!lx || !ly)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%c", &tag);
  if (tag != 'l')
  {
    fprintf(stderr, "%s: bad tag: %s ('l' expected)\n", argv[0], tag);
    exit(1);
  }

  fscanf(fd, "%d", &nvect);

#ifdef VERBOSE
  printf("loading %d lines segments\n", nvect);
#endif

  for (i = 0; i < nvect; i++)
  {
    fscanf(fd, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
    X1 = arrondi(x1);
    Y1 = arrondi(y1);
    X2 = arrondi(x2);
    Y2 = arrondi(y2);
    n = rs + cs;
    lbresenlist(X1, Y1, X2, Y2, lx, ly, &n);
    for (j = 0; j < n; j++)
    {
      X = lx[j]; Y = ly[j];
      if ((X >= 0) && (X < rs) && (Y >= 0) && (Y < cs))
	F[Y*rs + X] = NDG_MAX;
    }
  }

  fclose(fd); 
  free(lx);
  free(ly);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

