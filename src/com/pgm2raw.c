/* $Id: pgm2raw.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file pgm2raw.c

\brief suppress the header from a pgm file

<B>Usage:</B> pgm2raw in.pgm out.raw

<B>Description:</B> suppress the header from a pgm file

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  FILE *fd = NULL;
  int32_t rs, cs, ds, N, ret;
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.raw \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: only byte images supported\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

#ifdef VERBOSE
  printf("rs = %d ; cs = %d ; ds = %d ; N = rs * cs * ds = %d\n", rs, cs, ds, N);
#endif  

#ifdef UNIXIO
  fd = fopen(argv[argc - 1],"w");
#endif
#ifdef DOSIO
  fd = fopen(argv[argc - 1],"wb");
#endif

  ret = fwrite(UCHARDATA(image), sizeof(char), N, fd);
  if (ret != N)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], ret);
    exit(1);
  }

  fclose(fd);

  return 0;
} /* main */


