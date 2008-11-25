/* $Id: shake.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file shake.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* ajoute un bruit centre uniforme a une image */
/* Michel Couprie - janvier 1997 */

#include <stdio.h>
#include <stdint.h>
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
  struct xvimage * image;
  int32_t i;
  uint8_t *Im;
  int32_t rs, cs, N, p, tmp;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm p_entier out.pgm \n", argv[0]);
    exit(1);
  }

  p = atoi(argv[2]);
  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "shake: readimage failed\n");
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  Im = UCHARDATA(image);
  
  for (i = 0; i < N; i++)
  {
    tmp = (int32_t)Im[i] + p - (rand() % ((2*p)+1));
    if (tmp < 0) tmp = 0;
    if (tmp > 255) tmp = 255;
    Im[i] = (uint8_t)tmp;
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
