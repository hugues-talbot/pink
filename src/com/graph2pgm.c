/* $Id: graph2pgm.c,v 1.1 2009-07-15 05:31:01 mcouprie Exp $ */
/*! \file graph2pgm.c

\brief converts from graph representation to pgm

<B>Usage:</B> graph2pgm in.graph <in.pgm|rs cs ds> out.pgm

<B>Description:</B>

Reads the file <B>in.graph</B>. 
If a file name <B>in.pgm</B> is given, then the points read in <B>in.graph</B> are 
inserted (if possible) into the image read in <B>in.pgm</B>. Else, they are inserted in
a new image, the dimensions of which are <B>rs</B>, <B>cs</B>, and <B>d</B>.

<B>Types supported:</B> graph

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - juillet 2009
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgraphe.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rs, cs, ds, ps, N, i;
  char type;
  graphe *g;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.graph <in.pgm|rs cs ds> out.pgm\n", argv[0]);
    exit(1);
  }

  g = ReadGraphe(argv[1]);
  if (g == NULL)
  {
    fprintf(stderr, "%s: ReadGraphe failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    image = readimage(argv[2]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = rowsize(image);
    cs = colsize(image);
    ds = depth(image);
    ps = rs * cs;
    N = ps * ds;
    type = datatype(image);
  }
  else
  {
    rs = atoi(argv[2]);
    cs = atoi(argv[3]);
    ds = atoi(argv[4]);
    ps = rs * cs;
    N = ps * ds;
    type = VFF_TYP_1_BYTE;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
  }

  switch(type)
  {
    case VFF_TYP_1_BYTE:
    {
      uint8_t *F = UCHARDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (uint8_t)(g->v_sommets[i]); 
      break;
    }
    case VFF_TYP_4_BYTE:
    {
      uint32_t *F = ULONGDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (uint8_t)(g->v_sommets[i]); 
      break;
    }
    case VFF_TYP_FLOAT:
    {
      float *F = FLOATDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (float)(g->v_sommets[i]); 
      break;
    }
    default:
    {
      fprintf(stderr, "%s: bad data type %d\n", argv[0], type);
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

