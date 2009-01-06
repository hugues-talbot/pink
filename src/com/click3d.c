/* $Id: click3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
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
