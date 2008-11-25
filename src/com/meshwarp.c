/* $Id: meshwarp.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file meshwarp.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* generation interactice de grille pour mesh warper */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <lmeshwarp.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t mrs, rs, cs;
  double s;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm nbcol out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "meshwarp: readimage failed\n");
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;

  mrs = atoi(argv[2]);
  if ((mrs < 0) || (mrs >= rs/2))
  {
    fprintf(stderr, "meshwarp: bad number of columns : %d\n", mrs);
    exit(1);
  }

  if (! lmeshwarp(image, mrs))
  {
    fprintf(stderr, "meshwarp: function lmeshwarp failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
