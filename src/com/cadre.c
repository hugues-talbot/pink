/* $Id: cadre.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file cadre.c

OBSOLETE - voir frame.c

\brief generates an image with a white border and a black interior

<B>Usage:</B> cadre in.pgm out.pgm

<B>Description:</B>
The result out.pgm has the same size as in.pgm. Its border is set to 255, 
all other pixels are set to 0.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N, x, y, z;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm fileout.pgm\n", argv[0]);
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
  Im = UCHARDATA(image);
  memset(Im, 0, N);

  if (ds > 1)
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[0 * ps + y * rs + x] = NDG_MAX;          /* plan z = 0 */
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[(ds-1) * ps + y * rs + x] = NDG_MAX;     /* plan z = ds-1 */

    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + 0 * rs + x] = NDG_MAX;          /* plan y = 0 */
    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + (cs-1) * rs + x] = NDG_MAX;     /* plan y = cs-1 */

    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + 0] = NDG_MAX;          /* plan x = 0 */
    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + (rs-1)] = NDG_MAX;     /* plan x = rs-1 */
  }
  else
  {
    for (x = 0; x < rs; x++) Im[x] = NDG_MAX;
    for (x = 0; x < rs; x++) Im[(cs - 1) * rs + x] = NDG_MAX;

    for (y = 1; y < cs - 1; y++) Im[y * rs] = NDG_MAX;
    for (y = 1; y < cs - 1; y++) Im[y * rs + rs - 1] = NDG_MAX;
  }

  /* ---------------------------------------------------------- */
  /* fabrique le nom de l'image resultat */
  /* ---------------------------------------------------------- */

  image->name = NULL;
  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */



