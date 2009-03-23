/* $Id: eden.c,v 1.2 2009-03-23 12:45:35 mcouprie Exp $ */
/*! \file eden.c

\brief growth and/or shrinking of an 2D binary image - Eden process

<B>Usage:</B> eden in.pgm niter grow shrink topo out.pgm

<B>Description:</B>

Algorithm is as follows.

For growth:
  \li start from known set
  \li take pixels from the border into a queue
  \li choose randomly one such pixels
  \li set it to 1
  \li update queues
  \li repeat

For shrinking:
  \li same thing but set to 0

For adding boundary noise
  \li alternate growth and shrinking

With topological constraints
  \li make sure the point to add or remove is simple

Parameters:
  \li niter (positive integer): number of iterations
  \li grow (1/0): perform growing or not
  \li shrink (1/0): perform shrinking or not
  \li topo (8/4/0): connectivity for the object - 0: no topological constraint

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup geo

\author Hugues Talbot
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <leden.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t niter, grow, shrink, topo, rs, cs;
  uint8_t *In;
  uint8_t *Out;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in1.pgm niter grow shrink topo out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  In = UCHARDATA(image1);
  niter = atoi(argv[2]);
  grow = atoi(argv[3]);
  shrink = atoi(argv[4]);
  topo = atoi(argv[5]);

  image2 = copyimage(image1);
  if (image2 == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }
  Out = UCHARDATA(image2);
  rs = rowsize(image1);
  cs = colsize(image1);

  if (! ledengrowth(In, Out, rs, cs, niter, grow, shrink, topo))
  {
    fprintf(stderr, "%s: function ledengrowth failed\n", argv[0]);
    exit(1);
  }

  writeimage(image2, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


