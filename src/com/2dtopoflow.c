/* $Id: 2dtopoflow.c,v 1.1 2009-07-15 05:31:01 mcouprie Exp $ */
/*! \file 2dtopoflow.c

\brief ultimate constrained collapse guided by a priority image

<B>Usage:</B> 2dtopoflow in.pgm prio func [inhibit] out.pgm

<B>Description:</B>
Ultimate constrained collapse guided by a priority image.
The lowest values of the priority image correspond to the highest priority.

The parameter \b prio is either an image (byte or int32_t), or a numerical code
indicating that a distance map will be used as a priority image; 
the possible choices are:
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 
If the parameter \b inhibit is given and is a number I,
then the points with priority greater than or equal to I will be left unchanged. 

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky2d.h>
#include <mcgeo.h>
#include <mcgraphe.h>
#include <ldist.h>
#include <l2dcollapse.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * prio;
  struct xvimage * prio2;
  struct xvimage * func;
  struct xvimage * inhibimage = NULL;
  int32_t ret, priocode;
  int32_t rs, cs, N;
  float priomax_f;
  uint32_t priomax_l;
  graphe * flow;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm prio func [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  N = rs*cs;
  func = readimage(argv[3]);  
  if (func == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  ret = sscanf(argv[2], "%d", &priocode);
  if (ret == 0) // priorité : image 
  {
    prio2 = readimage(argv[2]);
    if (prio2 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if ((rowsize(prio2) != rs) || (colsize(prio2) != cs) || (depth(prio2) != 1))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", argv[0]);
      exit(1);
    }
    if (datatype(prio2) == VFF_TYP_1_BYTE)
    {
      uint8_t *B = UCHARDATA(prio2);
      uint32_t *L;
      int32_t x;
      prio = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
      if (prio == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      L = ULONGDATA(prio);
      for (x = 0; x < N; x++) L[x] = (uint32_t)B[x];
      freeimage(prio2);
    }
    else if (datatype(prio2) == VFF_TYP_4_BYTE)
    {
      prio = prio2;
    }
    else if (datatype(prio2) == VFF_TYP_FLOAT)
    {
      prio = prio2;
    }
    else
    {
      fprintf(stderr, "%s: bad datatype for prio\n", argv[0]);
      exit(1);
    }
  }
  else  // priorité : carte de distance (à calculer)
  {
    int32_t i;
    uint8_t *K;
    prio = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
    if (prio == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    K = UCHARDATA(k);;
    for (i = 0; i < N; i++) // inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  
    if (priocode == 0)
    {
      if (! ldisteuc(k, prio))
      {
        fprintf(stderr, "%s: ldisteuc failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 1)
    {
      if (! ldistquad(k, prio))
      {
        fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 2)
    {
      if (! lchamfrein(k, prio))
      {
        fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 3)
    {
      if (! lsedt_meijster(k, prio))
      {
	fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldist(k, priocode, prio))
      {
        fprintf(stderr, "%s: ldist failed\n", argv[0]);
        exit(1);
      }
    }
    for (i = 0; i < N; i++) // re-inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  }

  if (argc == 6) 
  {
    ret = sscanf(argv[4], "%f", &priomax_f);
    if (ret == 0) // inhibit : image
    {
      inhibimage = readimage(argv[3]);
      if (inhibimage == NULL)
      {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
      }
    }
  }
  else 
  {
    priomax_l = UINT32_MAX; 
    priomax_f = FLOAT_MAX; 
  }

  if (datatype(prio) == VFF_TYP_4_BYTE)
  {
    if ((argc == 6) && (inhibimage == NULL)) priomax_l = (uint32_t)floorf(priomax_f);
    if (! (flow = l2dtopoflow_l(k, prio, func, inhibimage, priomax_l)))
    {
      fprintf(stderr, "%s: function l2dtopoflow_l failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (datatype(prio) == VFF_TYP_FLOAT)
  {
    double maxf = MINDOUBLE;
    if (! (flow = l2dtopoflow_f(k, prio, func, inhibimage, priomax_f)))
    {
      fprintf(stderr, "%s: function l2dtopoflow_f failed\n", argv[0]);
      exit(1);
    }
    BellmanSCmax(flow);
  }
  
  SaveGraphe(flow, argv[argc-1]);

  //  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
