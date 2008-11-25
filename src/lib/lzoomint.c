/* $Id: lzoomint.c,v 1.1.1.1 2008-11-25 08:01:44 mcouprie Exp $ */
/* zoom par sous- ou sur-echantillonage */
/* Michel Couprie - septembre 1996 Christophe Doublier - mai 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lzoomint.h>

/* ==================================== */
int32_t lzoomint(
  struct xvimage * in,
  struct xvimage ** out,
  int32_t z,
  int32_t fill) /* booleen : remplit l'espace inter-pixels */
/* ==================================== */
{
  int32_t xx, yy, zz;
  int32_t i, j, k;
  uint8_t *ptin;
  uint8_t *ptout;
  int32_t rs, cs, ds, ps, N, rsz, csz, psz;

  rs = in->row_size;
  cs = in->col_size;
  ds = depth(in);
  ps = rs * cs;
  N = ps * ds;
  ptin = UCHARDATA(in);

  if (z == 0)
  {   fprintf(stderr,"lzoomint() : bad zoom factor : 0\n");
      return 0;
  }

  if (ds == 1)
  {
    if (z > 0)
    {
      *out = allocimage(in->name, rs * z, cs * z, ds, VFF_TYP_1_BYTE);
    }
    else
    {
      *out = allocimage(in->name, rs / (-z), cs / (-z), ds, VFF_TYP_1_BYTE);
    }

    if (*out == NULL)
    {   fprintf(stderr,"lzoomint() : allocimage failed\n");
        return 0;
    }
    ptout = (UCHARDATA(*out));

    if (z > 0)
    {
      if (fill)  /* pas efficace - a ameliorer */
        for (yy = 0; yy < cs; yy++)
          for (j = 0; j < z; j++)
            for (xx = 0; xx < rs; xx++)
              for (i = 0; i < z; i++)
              {
                ptout[(yy*z+j)*rs*z + xx*z+i] = ptin[zz*ps + yy*rs + xx];
 	      }
      else
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
            ptout[(yy*z)*rs*z + xx*z] = ptin[zz*ps + yy*rs + xx];
    }
    else
      for (yy = 0; yy < cs; yy++)
        for (xx = 0; xx < rs; xx++)
          if (((zz % (-z)) == 0) && ((yy % (-z)) == 0) && ((xx % (-z)) == 0)) 
            ptout[(yy/(-z))*(rs/(-z)) + (xx/(-z))] = ptin[zz*ps + yy*rs + xx];

  }
  else /* ds != 1 */
  {
    if (z > 0)
    {
      *out = allocimage(in->name, rs * z, cs * z, ds * z, VFF_TYP_1_BYTE);
      rsz = rs * z;
      csz = cs * z;
      psz = rsz * csz;
    }
    else
    {
      *out = allocimage(in->name, rs / (-z), cs / (-z), ds / (-z), VFF_TYP_1_BYTE);
      rsz = rs / (-z);
      csz = cs / (-z);
      psz = rsz * csz;
    }

    if (*out == NULL)
    {   fprintf(stderr,"lzoomint() : allocimage failed\n");
        return 0;
    }
    ptout = (UCHARDATA(*out));

    if (z > 0)
    {
      if (fill)  /* pas efficace - a ameliorer */
        for (zz = 0; zz < ds; zz++)
          for (k = 0; k < z; k++)
            for (yy = 0; yy < cs; yy++)
              for (j = 0; j < z; j++)
                for (xx = 0; xx < rs; xx++)
                  for (i = 0; i < z; i++)
	          {
                    ptout[(zz*z+k)*psz + (yy*z+j)*rsz + xx*z+i] = ptin[zz*ps + yy*rs + xx];
	          }
      else
        for (zz = 0; zz < ds; zz++)
          for (yy = 0; yy < cs; yy++)
            for (xx = 0; xx < rs; xx++)
              ptout[(zz*z)*psz + (yy*z)*rsz + xx*z] = ptin[zz*ps + yy*rs + xx];
    }
    else
      for (zz = 0; zz < ds; zz++)
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
            if (((zz % (-z)) == 0) && ((yy % (-z)) == 0) && ((xx % (-z)) == 0)) 
              ptout[(zz/(-z))*(psz) + (yy/(-z))*(rsz) + (xx/(-z))] = ptin[zz*ps + yy*rs + xx];
  }
  return 1;
}

/* ==================================== */
struct xvimage* lzoomintxyz(
  struct xvimage * in,
  int32_t zoomx,
  int32_t zoomy,
  int32_t zoomz,
  int32_t fill) /* booleen : remplit l'espace inter-pixels */
/* ==================================== */
{
  int32_t xx, yy, zz;
  int32_t i, j, k;
  uint8_t *ptin;
  uint8_t *ptout;
  int32_t rs, cs, ds, ps, N, rsz, csz, dsz, psz;
  struct xvimage * out;

  rs = in->row_size;
  cs = in->col_size;
  ds = depth(in);
  ps = rs * cs;
  N = ps * ds;
  ptin = UCHARDATA(in);

  if (zoomx == 0||zoomy ==0||zoomz==0)
  {   fprintf(stderr,"lzoomintxyz() : bad zoom factor : 0\n");
      return 0;
  }

  if (ds == 1)
  {
    if (zoomx > 0 && zoomy>0 && zoomz>0)
    {
       out = allocimage(in->name, rs * zoomx, cs * zoomy, ds , VFF_TYP_1_BYTE);
    }
    else if (zoomx < 0 && zoomy<0 && zoomz<0)
    {
       out = allocimage(in->name, rs / (-zoomx), cs / (-zoomy), ds , VFF_TYP_1_BYTE);
    }
    else 
      {
	fprintf (stderr,"erreur facteur de zoom non tous positif ou non tous négatif");
	return 0;
      }

    if (out == NULL)
    {   fprintf(stderr,"lzoomintxyz() : allocimage failed\n");
        return 0;
    }
    ptout = UCHARDATA(out);

    if (zoomx>0 && zoomy>0 && zoomz>0)
    {
      if (fill)  /* pas efficace - a ameliorer */
        for (yy = 0; yy < cs; yy++)
          for (j = 0; j < zoomy; j++)
            for (xx = 0; xx < rs; xx++)
              for (i = 0; i < zoomx; i++)
              {
                ptout[(yy*zoomy+j)*rs*zoomx + xx*zoomx+i] = ptin[yy*rs + xx];
 	      }
      else
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
            ptout[(yy*zoomy)*rs*zoomx + xx*zoomx] = ptin[yy*rs + xx];
    }
    else if (zoomx<0 && zoomy<0 && zoomz<0)
    {
      for (yy = 0; yy < (cs); yy++)
        for (xx = 0; xx < (rs); xx++)
	  if ( yy%zoomy==0 && xx%zoomx==0)
            ptout[(yy/(-zoomy))*(rs/(-zoomx)) + (xx/(-zoomx))] = ptin[ yy*rs + xx];
    }
    else 
    {
      fprintf(stderr,"lzoomintxyz() : facteur zoom non tous positifs ou non tous negatifs\n");
      return 0;
    }

  }
  else /* ds != 1 */
  {
    if (zoomx>0 && zoomy>0 && zoomz>0)
    {
      rsz = rs * zoomx;
      csz = cs * zoomy;
      dsz = ds * zoomz;
      psz = rsz * csz;
      out = allocimage(in->name, rsz, csz, dsz, VFF_TYP_1_BYTE);
    }
    else if (zoomx<0 && zoomy<0 && zoomz<0)
    {
      rsz = rs / (-zoomx);
      csz = cs / (-zoomy);
      dsz = ds / (-zoomz);
      psz = rsz * csz;
      out = allocimage(in->name, rsz, csz, dsz, VFF_TYP_1_BYTE);
    }
    else 
    {
      fprintf(stderr,"lzoomintxyz() : facteur zoom non tous positifs ou non tous negatifs\n");
      return 0;
    }

    if (out == NULL)
    {   fprintf(stderr,"lzoomintxyz() : allocimage failed\n");
        return 0;
    }
    ptout = UCHARDATA(out);

    if (zoomx > 0) //zoomy>0 et zoomz>0 aussi
    {
      if (fill)  /* pas efficace - a ameliorer */
        for (zz = 0; zz < ds; zz++)
          for (k = 0; k < zoomz; k++)
            for (yy = 0; yy < cs; yy++)
              for (j = 0; j < zoomy; j++)
                for (xx = 0; xx < rs; xx++)
                  for (i = 0; i < zoomx; i++)
	          {
                    ptout[(zz*zoomz+k)*psz + (yy*zoomy+j)*rsz + xx*zoomx+i] = ptin[zz*ps + yy*rs + xx];
	          }
      else
        for (zz = 0; zz < ds; zz++)
          for (yy = 0; yy < cs; yy++)
            for (xx = 0; xx < rs; xx++)
              ptout[(zz*zoomz)*psz + yy*rsz + xx] = ptin[zz*zoomz*ps + yy*zoomy*rs + xx*zoomx];
    } 
    else // zoomx<0 et zoomy<0 et zoomz<0 
      for (zz = 0; zz < ds; zz++)
        for (yy = 0; yy < cs; yy++)
          for (xx = 0; xx < rs; xx++)
           if ( xx%zoomx==0 && yy%zoomy==0 && zz%zoomz==0 &&
		xx/(-zoomx) < rsz && yy/(-zoomy) < csz && zz/(-zoomz) < dsz
	      )
	   {
              ptout[(zz/(-zoomz))*psz + ((yy/(-zoomy)))*rsz + (xx/(-zoomx))] = ptin[zz*ps + yy*rs + xx];
	   }
  }
  return out;
}
