/* $Id: dynamique.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file dynamique.c

\brief dynamics of the maxima

<B>Usage:</B> dynamique in.pgm connex [order] out.pgm

<B>Description:</B>
Computes the ordered dynamics of the maxima, with connectivity <B>connex</B>.
The definition of the ordered dynamics is the one given in [Ber05]. 
The optional argument <B>order</B> is one of the following:
\li 0: altitude [default] 
\li 1: area
\li 2: volume

References:<BR> 
[Ber05] G. Bertrand: "A new definition of the dynamics", <I>Procs. ISMM05</I>, Springer, series Computational Imaging and Vision, Vol.&nbsp;30, pp.&nbsp;197-206, 2005.<BR>

<B>Types supported:</B> byte 2D, byte 3D.

<B>Category:</B> 
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldynamique.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * order;
  int32_t mode;
  int32_t connex;
  int32_t nblabels;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [order] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (argc == 5)
    mode = atoi(argv[3]);
  else
    mode = 0;

  order = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (order == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! llabelextrema(image, connex, LABMAX, order, &nblabels))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
    exit(1);
  }

  switch (mode)
  {
  case 0:
    if (! lordermaxima(image, order, connex, nblabels))
    {
      fprintf(stderr, "%s: lordermaxima failed\n", argv[0]);
      exit(1);
    }
    break;
  case 1:
    if (! lordermaximasurf(image, order, connex, nblabels))
    {
      fprintf(stderr, "%s: lordermaximasurf failed\n", argv[0]);
      exit(1);
    }
    break;
  case 2:
    if (! lordermaximavol(image, order, connex, nblabels))
    {
      fprintf(stderr, "%s: lordermaximavol failed\n", argv[0]);
      exit(1);
    }
    break;
  default:
    fprintf(stderr, "%s: bad mode: %d\n", argv[0], mode);
    exit(1);    
  } // switch (mode)

  if (! ldynamique(image, order, connex))
  {
    fprintf(stderr, "%s: ldynamique failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



