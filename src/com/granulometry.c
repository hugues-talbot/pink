/* $Id: granulometry.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file granulometry.c

\brief granulometry of a binary image

<B>Usage:</B> granulometry in.pgm [rmin] rmax out.list

<B>Description:</B>
Let X be the set of non-null points in <B>in.pgm</B>.
This operator computes the area (volume in 3d) of the opening of X by 
euclidean balls of increasing radius, ranging from 1 
(or <B>rmin</B> if this parameter is specified) to <B>rmax</B>. 
The result <B>out.list</B> contains a list of couples <B>r a</B> where 
<B>r</B> is a radius and <B>a</B> is the corresponding area (or volume).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * temp;
  FILE *fd = NULL;
  int32_t rayonmin, rayonmax, rayon;
  int32_t rs, cs, ds, N, i, S;
  uint8_t *T;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [rmin] rmax out.list \n", argv[0]);
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
  ds = depth(image);
  N = rs * cs * ds;

  if (argc == 4) 
  {
    rayonmin = 1;
    rayonmax = atoi(argv[2]);
  }
  else
  {
    rayonmin = atoi(argv[2]);
    rayonmax = atoi(argv[3]);
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  fprintf(fd, "s %d\n", rayonmax - rayonmin + 1);

  temp = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (!temp)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  T = UCHARDATA(temp);

  for (rayon = rayonmin; rayon <= rayonmax; rayon++)
  {
#ifdef VERBOSE
    fprintf(stderr, "%s: radius = %d\n", argv[0], rayon);
#endif

    copy2image(temp, image);

    // ouverture
    if (! lerosball(temp, rayon, 0))
    {
      fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
      exit(1);
    }
    if (! ldilatball(temp, rayon, 0))
    {
      fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
      exit(1);
    }

    S = 0;
    for (i = 0; i < N; i++) if (T[i]) S++;

    fprintf(fd, "%d %d\n", rayon, S);

  } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

  fclose(fd);
  freeimage(image);
  freeimage(temp);
  return 0;
} /* main */
