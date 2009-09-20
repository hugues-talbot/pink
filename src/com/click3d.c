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
/* \file click3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lclick.h>

#define NBMAXCLICKS 10000 

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  uint32_t *LAB;
  int32_t *VLAB;
  int32_t zoom;
  int32_t i, rs, cs, ds, ps, N, nlabs;
  FILE *fd;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm [labs.pgm] zoom fileout\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
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

  if (argc == 4) /* pas d'image de labels : sortie dans une liste de points */
  {
    LAB = (uint32_t *)calloc(1,NBMAXCLICKS * sizeof(int32_t));
    VLAB = (int32_t *)calloc(1,NBMAXCLICKS * sizeof(int32_t));
    if ((LAB == NULL) || (VLAB == NULL))
    {   
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }
    zoom = atoi(argv[2]);

    if (! lclick3d(image, LAB, VLAB, &nlabs, zoom))
    {
      fprintf(stderr, "%s: lclick3d failed\n", argv[0]);
      exit(1);
    }

    fd = fopen(argv[argc - 1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
      exit(1);
    }

    if (ds == 1) 
    {
      fprintf(fd, "n %d\n", nlabs);
      for (i = 0; i < nlabs; i++)
        fprintf(fd, "%d %d %d\n", LAB[i] % rs, LAB[i] / rs, VLAB[i]); 
    }
    else
    {
      fprintf(fd, "N %d\n", nlabs);
      for (i = 0; i < nlabs; i++)
        fprintf(fd, "%d %d %d %d\n", LAB[i] % rs, (LAB[i] % ps) / rs, LAB[i] / ps, VLAB[i]); 
    }
    fclose(fd);
    free(LAB);
    freeimage(image);
  }
  else /* image de labels disponible */
  {
    result = readimage(argv[2]);
    if (result == NULL)
    {   
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if ((rowsize(result) != rs) || (colsize(result) != cs) || (depth(result) != ds))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", argv[0]);
      exit(1);
    }
    if (datatype(result) != VFF_TYP_4_BYTE)
    {   
      fprintf(stderr, "%s: type of label image MUST be int32_t\n", argv[0]);
      exit(1);
    }
    zoom = atoi(argv[3]);

    if (! lclick2d(image, result, zoom))
    {
      fprintf(stderr, "%s: lclick2d failed\n", argv[0]);
      exit(1);
    }

    writelongimage(result, argv[4]);
    freeimage(result);
    freeimage(image);
  }

  return 0;
} /* main */
