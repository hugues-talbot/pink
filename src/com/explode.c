/* $Id: explode.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file explode.c

\brief converts single 3D pgm file into a series of 2D pgm files

<B>Usage:</B> explode in.pgm name_prefix

<B>Description:</B>
Generated file names are of the form: <B>name_prefix</B>nnnn.pgm, 
where nnnn is a four digit decimal integer. 

<B>Types supported:</B> byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - avril 2001

  ATTENTION: pas de nom image resultat
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t i, j, k, x;
  char bufname[1024];
  int32_t namelen;
  struct xvimage * image_in;
  struct xvimage * image_out;
  int32_t rs, cs, ds, ps, N;
  uint8_t *I;
  uint8_t *O;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm name_prefix\n", argv[0]);
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

  strcpy(bufname, argv[2]);
  namelen = strlen(argv[2]);
  
  image_out = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (image_out == NULL)
  {   
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }
  O = UCHARDATA(image_out);

  for (k = 0; k < ds; k++)
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

    for (i = 0; i < ps; i++) O[i] = I[k * ps + i];
    writeimage(image_out, bufname);

  } /* for k */

  freeimage(image_in);
  freeimage(image_out);
  return 0;
} /* main() */
