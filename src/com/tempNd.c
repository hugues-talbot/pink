/* $Id: tempNd.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file tempNd.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Michel Couprie - novembre 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimskyNd.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * o;
  struct xvimage * k;
  Khalimsky *K;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  o = readimage(argv[1]);  
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  K = KhalimskizeNd(o);
  if (K == NULL)
  {
    fprintf(stderr, "%s: KhalimskizeNd failed\n", argv[0]);
    exit(1);
  }

  Connex4ObjNd(K);

  if (EnsembleSimpleNd(K)) printf("ensemble simple\n"); else printf("ensemble NON simple\n");

  k = DeKhalimskizeNd(K);

  FreeKhalimskyNd(K);
  TermineNd();
  writeimage(k, argv[2]);
  freeimage(o);
  freeimage(k);

  return 0;
} /* main */
