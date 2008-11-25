/* $Id: lpemeyer.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file lpemeyer.c

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

<B>lpemeyer</B> - watershed transformation

SYNOPSIS

<B>lpemeyer</B> in.pgm connex mark.pgm [bgmark.pgm] out.pgm

DESCRIPTION

Performs the watershed transformation on the image <B>in.pgm</B>, taking the
set of markers in <B>mark.pgm</B>. If this parameter is present, <B>bgmark.pgm</B>
is used as a set of markers for the background.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

Types supported: byte 2D, 3D.

CLASS 

morph

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * marqueursfond;
  int32_t connex;

  if ((argc < 5) || (argc > 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex marqueurs.pgm [marqueursfond.pgm] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  connex = atoi(argv[2]);
  marqueurs = readimage(argv[3]);
  if ((image == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    marqueursfond = NULL;
  }
  else
  {
    marqueursfond = readimage(argv[4]);
    if (marqueursfond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if ((connex == 4) || (connex == 8))
  {
    if (! llpemeyer(image, marqueurs, marqueursfond, connex))
    {
      fprintf(stderr, "%s: llpemeyer failed\n", argv[0]);
      exit(1);
    }
  }
  else if ((connex == 6) || (connex == 18) || (connex == 26))
  {
    if (! llpemeyer3d(image, marqueurs, marqueursfond, connex))
    {
      fprintf(stderr, "%s: llpemeyer3d failed\n", argv[0]);
      exit(1);
    }
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }


  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (marqueursfond) freeimage(marqueursfond);

  return 0;
} /* main */
