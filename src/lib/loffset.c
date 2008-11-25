/* $Id: loffset.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* 
   Decale l'image des offsets ox, oy, oz (peuvent etre < 0) - 
   mode 0: les points ejectes hors du cadre sont perdus (defaut)
   mode 1: il y a rotation torique pour les points ejectes hors du cadre 

   Michel Couprie - juillet 1996

   Modif janvier 2000 : mode 0
   Modif novembre 2002 : 3D
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t loffset(
  struct xvimage * image,
  int32_t ox,
  int32_t oy,
  int32_t oz,
  int32_t mode)
/* ==================================== */
{
  struct xvimage * temp;
  int32_t x, y, z, xx, yy, zz;
  uint8_t *pti;
  uint8_t *ptt;
  int32_t rs, cs, ds, ps, N;

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  N = ps * ds;

  temp = copyimage(image);
  if (temp == NULL)
  {
    fprintf(stderr, "loffset: copyimage failed\n");
    return 0;
  }

  pti = UCHARDATA(image);
  ptt = UCHARDATA(temp);

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  printf("%d %d %d %d %d %d %d\n", rs, cs, ds, ox, oy, oz, mode);

  switch (mode)
  {
    case 0:
      memset(pti, 0, N);
      for (zz = 0; zz < ds; zz++)
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
          {
            z = zz - oz;
            y = yy - oy;
            x = xx - ox;
            if ((z >= 0) && (y >= 0) && (x >= 0) && (z < ds) && (y < cs) && (x < rs))
              pti[zz*ps + yy*rs + xx] = ptt[z*ps + y*rs + x];
          }
      break;
    case 1:
      memset(pti, 0, N);
      for (zz = 0; zz < ds; zz++)
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
          {
            z = zz - oz;
            if (z < 0) z += ds; else if (z >= ds) z -= ds;
            y = yy - oy;
            if (y < 0) y += cs; else if (y >= cs) y -= cs;
            x = xx - ox;
            if (x < 0) x += rs; else if (x >= rs) x -= rs;
            pti[zz*ps + yy*rs + xx] = ptt[z*ps + y*rs + x];
          }
      break;
    default:
      fprintf(stderr, "loffset: bad mode %d\n", mode);
      return 0;
  } /* switch (mode) */
  freeimage(temp);
  return 1;
}
