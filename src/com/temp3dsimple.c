/* $Id: temp3dsimple.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file temp3dsimple.c

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
#include <mckhalimsky3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * o;
  struct xvimage * k;

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

  k = Khalimskize3d(o);
  if (k == NULL)
  {
    fprintf(stderr, "%s: Khalimskize3d failed\n", argv[0]);
    exit(1);
  }

  Connex26Obj3d(k);
  
  InitPileGrilles3d();

  if (EnsembleSimple3d(k)) printf("ensemble simple\n"); else printf("ensemble NON simple\n");

  TerminePileGrilles3d();
  
  ColorieKh3d(k);

  writeimage(k, argv[2]);
  freeimage(o);
  freeimage(k);

  return 0;
} /* main */
