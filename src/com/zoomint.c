/* $Id: zoomint.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file zoomint.c

\brief zoom by an integer factor

<B>Usage:</B> zoomint in.pgm  <f | fx fy fz> [nofill] out.pgm

<B>Description:</B> 
The zoom factor(s) can be unique (\b f ) of there can be one zoom factor
per image dimension (\b fx, \b fy, \b fz ). In the case of a 2d image, set \b fz
to 1.
The zoom parameter may be a positive or negative integer.
If it is positive, the image is expanded in the corresponding 
direction. The pixels are replicated in order to fill all the space,
except if the parameter <B>nofill</B> is used.
If it is negative, the image is shrinked the corresponding 
directions, by undersampling.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie, Christophe Doublier
*/

/* Michel Couprie - septembre 1996, Christophe Doublier - mai 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lzoomint.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * in;
  struct xvimage * out;
  int32_t zx,zy,zz;
  int32_t fill;

  if ((argc != 4) && (argc != 5) && (argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm <f | fx fy fz> [nofill] out.pgm \n", argv[0]);
    exit(1);
  }

  fill = 1;
  if (argc == 5)
  {
    if (strcmp(argv[3], "nofill") == 0) fill = 0; else 
    {
      fprintf(stderr, "usage: %s in.pgm <f | fx fy fz> [nofill] out.pgm \n", argv[0]);
      exit(1);
    }
  }
  if (argc == 7)
  {
    if (strcmp(argv[5], "nofill") == 0) fill = 0; else 
    {
      fprintf(stderr, "usage: %s in.pgm <f | fx fy fz> [nofill] out.pgm \n", argv[0]);
      exit(1);
    }
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((argc == 7) || (argc == 6))
  {
    zx = atoi(argv[2]);
    zy = atoi(argv[3]);
    zz = atoi(argv[4]);
  }
  else
    zx = zy = zz = atoi(argv[2]);
  
  out = lzoomintxyz(in,zx,zy,zz,fill);

  if (out==NULL)
  {
    fprintf(stderr, "%s: function lzoomintxyz failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
