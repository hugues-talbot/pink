/* $Id: watershedMeyer_time.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file watershedMeyer.c

\brief watershed transformation (Meyer's algorithm)

<B>Usage:</B> watershedMeyer in mark <bgmark|null> <roi|null> connex out

<B>Description:</B>
Performs the watershed transformation on the image <B>in</B>, taking the
set of markers in <B>mark</B>. 
If this parameter is present, <B>bgmark</B>
is used as a set of markers for the background.
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>
#include <time.h>

/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * image, *result;
  int32_t connex;
  clock_t t1,t2;

  printf("Watershed Meyer");

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in connex out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  t1 = clock();
  if ((connex == 4) || (connex == 8))
  {
    if (! llpemeyersansligne(image, connex, result))
    {
      fprintf(stderr, "%s: llpemeyer failed\n", argv[0]);
      exit(1);
    }
  } else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }

  t2 = clock(); 
  printf("temps �coule %d ms\n", 
	 (t2 - t1)/ (CLOCKS_PER_SEC / 1000));
  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result); 
  return 0;
} /* main */
