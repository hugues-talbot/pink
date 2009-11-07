/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
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
int main(int argc, char **argv)
/* =============================================================== */
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

  if (depth(image) == 1) // 2D
  {
    if (! lsquelbin(image, connex, niseuil))
    {
      fprintf(stderr, "%s: lsquelbin failed\n", argv[0]);
      exit(1);
    }
  }
  else // 3D
  {
    uint8_t *endpoint;
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
#ifdef DIST6
    if (! ldist(image, 6, prio))
    {
      fprintf(stderr, "%s: ldist failed\n", argv[0]);
      exit(1);
    }
#else
    if (! lsedt_meijster(image, prio))
    {
      fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
      exit(1);
    }
#endif
#endif
    tablesize = 1<<24;
    endpoint = (uint8_t *)malloc(tablesize);
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
