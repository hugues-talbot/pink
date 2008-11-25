/* $Id: pgm2list1d.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file pgm2list1d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Traduit un fichier image 1D (pgm) en fichier contenant le signal 1D correspondant,
  sous forme d'une liste de couples  x f(x)

  Michel Couprie - mars 2001
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, x, y;
  uint8_t *F;
  uint32_t *G;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s image.pgm out.list \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((datatype(image) != VFF_TYP_1_BYTE) && (datatype(image) != VFF_TYP_4_BYTE))
  {
    fprintf(stderr, "%s: bad image type(s)\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  
  if ((ds != 1) || (cs != 1))
  {
    fprintf(stderr, "%s: cette version ne traite que les signaux 1D\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    F = UCHARDATA(image);
    for (x = 0; x < rs; x++)
      fprintf(fd, "%d %d\n", x, F[x]); 
  }
  else
  {
    G = ULONGDATA(image);
    for (x = 0; x < rs; x++)
      fprintf(fd, "%d %d\n", x, G[x]); 
  }

  fclose(fd);
  freeimage(image);
  return 0;
}
