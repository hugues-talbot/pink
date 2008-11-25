/* $Id: drawcurve.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file drawcurve.c

\brief draw line segments which are specified by a text file

<B>Usage:</B> drawline in.pgm curve.txt out.pgm

<B>Description:</B>
The file \b curve.txt contains a list of points under the format:<br>
c nb_points<br>
x1 y1<br>
x2 y2<br>
x3 y3<br>
...<br>

<B>Types supported:</B> byte 2D

<B>Category:</B> draw
\ingroup draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i;
  double x1, y1, x2, y2;
  int32_t X1, Y1, X2, Y2;
  FILE *fd = NULL;
  int32_t npoints;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm curve.txt out.pgm \n", argv[0]);
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

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%c", &type);
  if ((type != 'c') && (type != 'b'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", type);
    exit(1);
  }
  fscanf(fd, "%d", &npoints);
  fscanf(fd, "%lf%lf", &x1, &y1);
  X1 = arrondi(x1);
  Y1 = arrondi(y1);

  for (i = 1; i < npoints; i++)
  {
    fscanf(fd, "%lf%lf", &x2, &y2);
    X2 = arrondi(x2);
    Y2 = arrondi(y2);
    ldrawline(image, X1, Y1, X2, Y2);
    X1 = X2; Y1 = Y2;
  }
  fclose(fd);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

