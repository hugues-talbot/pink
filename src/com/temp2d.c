/* $Id: temp2d.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file temp2d.c

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
#include <mckhalimsky2d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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

  k = Khalimskize2d(o);
  if (k == NULL)
  {
    fprintf(stderr, "%s: Khalimskize2d failed\n", argv[0]);
    exit(1);
  }

  Connex8Obj2d(k);

  DrawPsKh2d(k, 20, 50, 10, 17, 24);
  
  InitPileGrilles2d();

#define SQUELETTE

#ifdef OLDDEF
  if (EnsembleSimple2d(k)) printf("ensemble simple\n"); 
  else printf("ensemble NON simple\n");
#endif

#ifdef NEWDEF
  if (Ensemble2Contractile2d(k)) printf("ensemble contractile\n"); 
  else printf("ensemble NON contractile\n");
#endif

#ifdef SQUELETTE
  Squelette2d(k);
#endif

  TerminePileGrilles2d();
  
  ColorieKh2d(k);

  writeimage(k, argv[2]);
  freeimage(o);
  freeimage(k);

  return 0;
} /* main */
