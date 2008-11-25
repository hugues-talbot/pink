/* $Id: sym.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file sym.c

\brief symmetry (vertical, horizontal, central in 2D; x, y, z in 3D) 

<B>Usage:</B> sym in.pgm type out.pgm

<B>Description:</B>
Symmetry (vertical, horizontal, central in 2D; x, y, z in 3D).
The parameter <B>type</B> is set to <B>v</B> or <B>y</B> (vertical), 
<B>h</B> or <B>x</B> (horizontal), or <B>c</B> (central) for 2D images, 
or to <B>x</B>, <B>y</B> or <B>z</B> for 3D images.

<B>Types supported:</B> byte 2d, byte 3d.

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
   Michel Couprie  -  Mai 1998 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsym.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  char mode;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm [v|h|c|x|y|z] out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "sym: readimage failed\n");
    exit(1);
  }

  mode = argv[2][0];
  if ((mode != 'v')&&(mode != 'h')&&(mode != 'c')&&(mode != 'x')&&(mode != 'y')&&(mode != 'z'))
  {
    fprintf(stderr, "usage: %s in1.pgm [v|h|c|x|y|z] out.pgm \n", argv[0]);
    exit(1);
  }

  if (! lsym(image1, mode))
  {
    fprintf(stderr, "sym: function lsym failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


