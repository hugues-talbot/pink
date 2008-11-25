/* $Id: skelend.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file skelend.c

\brief homotopic skeleton of a 2d or 3d binary image with dynamic detection of end points

<B>Usage:</B> skelend in.pgm connex [n] out.pgm

<B>Description:</B>
Homotopic skeletonization by iterative removal of simple,
non-end points. Breadth-first strategy.
During the first \b n iterations (default 0), the end points
are removed as well.
If \b n = -1, the end points are always removed.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\warning This operator is very sensitive to small contour irregularities. 
See skelcurv for a more robust operator.

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
#include <lsquelbin.h>
#include <lskeletons.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex;
  int32_t niseuil;
  FILE *fd;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [n] fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if (argc == 4)
    niseuil = 0;
  else
    niseuil = atoi(argv[3]);

  if (depth(image) == 1)
  {
    if (! lsquelbin(image, connex, niseuil))
    {
      fprintf(stderr, "%s: lsquelbin failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    uint8_t *endpoint;
    struct xvimage * prio;
    char tablefilename[128];
    int32_t tablesize, ret;

    if (niseuil != 0)
    {
      fprintf(stderr, "%s: param. niseuil not implemented for 3d\n", argv[0]);
      exit(1);
    }
#ifdef PRIO
    prio = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
    if (prio == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }    
    if (! lsedt_meijster(image, prio))
    {
      fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
      exit(1);
    }
#ifdef DIST6
    if (! ldist(image, 6, prio))
    {
      fprintf(stderr, "%s: ldist failed\n", argv[0]);
      exit(1);
    }
#endif
#endif
    tablesize = 1<<24;
    endpoint = malloc(tablesize);
    if (! endpoint)
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }
    
    sprintf(tablefilename, "%s/src/tables/TabEndPoints.txt", getenv("PINK"));
    fd = fopen (tablefilename, "r");
    if (fd == NULL) 
    {   
      fprintf(stderr, "%s: error while opening table\n", argv[0]);
      exit(1);
    }
    ret = fread(endpoint, sizeof(char), tablesize, fd);
    if (ret != tablesize)
    {
      fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", argv[0], tablesize, ret);
      exit(1);
    }
    fclose(fd);

    //    if (! lskelend3d(image, prio, connex, endpoint))
    if (! lskelend3d(image, connex, endpoint))
    {
      fprintf(stderr, "%s: lskelend3d failed\n", argv[0]);
      exit(1);
    }
    //    freeimage(prio);
    free(endpoint);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */
