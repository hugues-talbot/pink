/* $Id: explodecomp.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file explodecomp.c

\brief converts single 2D pgm file into a series of 2D pgm files,
  where each file of the series contains one component of the original image

<B>Usage:</B> explodecomp in.pgm connex <min|max|pla> name_prefix

<B>Description:</B>
Generated file names are of the form: <B>name_prefix</B>nnnn.pgm, 
where nnnn is a four digit decimal integer. 
The argument \b connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).
The argument <B><min|max|pla></B> selects the kind of flat zone
to be labeled:
\li \b min selects regional minima (or background components for a binary image)
\li \b max selects regional maxima (or foreground components for a binary image)
\li \b pla selects all flat zones (plateaux).

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - avril 2001
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <llabelextrema.h>
#include <llabelplateaux.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i, k, connex, nblabels;
  char bufname[1024];
  int32_t namelen;
  struct xvimage * image_in;
  struct xvimage * image_out;
  struct xvimage * labels;
  int32_t rs, cs, ds, ps, N;
  uint8_t *I;
  uint8_t *O;
  int32_t *L;
  int32_t function;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm connex <min|max|pla> name_prefix\n", argv[0]);
    exit(1);
  }

  image_in = readimage(argv[1]);
  if (image_in == NULL)
  {
    fprintf(stderr, "%s: readimage failed: %s\n", argv[0], bufname);
    exit(1);
  }
  rs = rowsize(image_in);   /* taille ligne */
  cs = colsize(image_in);   /* taille colonne */
  ds = depth(image_in);     /* nb plans */
  ps = rs * cs;             /* taille plan */
  N = ps * ds;              /* taille image */
  I = UCHARDATA(image_in);

  connex = atoi(argv[2]);

  if (strcmp(argv[3], "min") == 0) function = LABMIN; else
  if (strcmp(argv[3], "max") == 0) function = LABMAX; else
  if (strcmp(argv[3], "pla") == 0) function = LABPLA; else
  {
    fprintf(stderr, "usage: %s in.pgm connex <min|max|pla> name_prefix\n", argv[0]);
    exit(1);
  }

  strcpy(bufname, argv[argc-1]);
  namelen = strlen(argv[argc-1]);
  
  image_out = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (image_out == NULL)
  {   
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }
  O = UCHARDATA(image_out);

  labels = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  L = SLONGDATA(labels);
  
  if (function != LABPLA)
  {
    if (! llabelextrema(image_in, connex, function, labels, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! llabelplateaux(image_in, connex, labels, &nblabels))
    {
      fprintf(stderr, "%s: llabelplateaux failed\n", argv[0]);
      exit(1);
    }
  }

  for (k = 1; k < nblabels; k++)
  {  
    bufname[namelen] =   '0' + (k / 1000) % 10;
    bufname[namelen+1] = '0' + (k / 100) % 10;
    bufname[namelen+2] = '0' + (k / 10) % 10;
    bufname[namelen+3] = '0' + (k / 1) % 10;
    bufname[namelen+4] = '.';
    bufname[namelen+5] = 'p';
    bufname[namelen+6] = 'g';
    bufname[namelen+7] = 'm';
    bufname[namelen+8] = '\0';
    memset(O, 0, N);
    for (i = 0; i < ps; i++) 
      if (L[i] == k) 
        O[i] = NDG_MAX;
    writeimage(image_out, bufname);
  } /* for k */

  freeimage(image_in);
  freeimage(image_out);
  freeimage(labels);
  return 0;
} /* main() */
