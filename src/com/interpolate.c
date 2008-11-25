/* $Id: interpolate.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* interpolation entre plans d'une image 3d */
/* Michel Couprie - mai 2001 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>

/* ==================================== */
int32_t linterpolate(struct xvimage * image1, int32_t factor, struct xvimage ** res)
/* ==================================== */
{
  int32_t x, y, z, i;
  uint8_t *p;
  uint8_t *r;
  int32_t rs, cs, ps, ds, N;
  uint8_t t1, t2;

  rs = rowsize(image1);
  cs = colsize(image1);
  ps = rs * cs;
  ds = depth(image1);
  N = ps * ds;
  
  *res = allocimage(NULL, rs, cs, factor*(ds-1) + 1, VFF_TYP_1_BYTE);
  if (*res == NULL)
  {   fprintf(stderr,"linterpolate() : allocimage failed\n");
      return 0;
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  p = UCHARDATA(image1);
  r = UCHARDATA((*res));
  for (z = 0; z < ds-1; z++)
  {
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
    {
      t1 = p[z*ps + y*rs + x];
      t2 = p[(z+1)*ps + y*rs + x];
      for (i = 0; i < factor; i++)
        r[(z*factor + i) * ps + y * rs + x] = t1 + ((t2-t1)*i)/factor;
    }
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      r[(ds-1)*factor * ps + y * rs + x] = p[(ds-1)*ps + y*rs + x];
  }

  /* ---------------------------------------------------------- */
  /* fabrique le nom de l'image resultat */
  /* ---------------------------------------------------------- */

  return 1;
}

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * f;
  struct xvimage * r;
  int32_t factor;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm factor out.pgm \n", argv[0]);
    exit(0);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(0);
  }

  factor = atoi(argv[2]);

  if (! linterpolate(f, factor, &r))
  {
    fprintf(stderr, "%s: function linterpolate failed\n", argv[0]);
    exit(0);
  } 

  writeimage(r, argv[argc-1]);
  freeimage(r);
  freeimage(f);

} /* main */





