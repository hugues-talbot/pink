/* $Id: lpemeyer3d.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file lpemeyer3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>lpemeyer3d</B> - watershed transformation

SYNOPSIS

<B>lpemeyer3d</B> in.pgm mark.pgm out.pgm

DESCRIPTION

Performs the watershed transformation on the image <B>in.pgm</B>, taking the
set of markers in <B>mark.pgm</B>. 

Types supported: byte 3D.

CLASS 

morph

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  int32_t trace;
  char * nameout;

  if ((argc < 4) || (argc > 5) || ((argc == 5) && (strcmp(argv[3], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm marqueurs.pgm [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    trace = 0;
    nameout = argv[3];
  }
  else
  {
    trace = 1;
    nameout = argv[4];
  }

  image = readimage(argv[1]);
  marqueurs = readimage(argv[2]);
  if ((image == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! llpemeyer3d(image, marqueurs, trace))
  {
    fprintf(stderr, "%s: llpemeyer3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, nameout);
  freeimage(image);
  freeimage(marqueurs);

  return 0;
} /* main */
