/* 
  lcrop
  lcrop3d
  lencadre
  lenframe
  linsert
  lexpandframe
*/

/* 
  Michel Couprie - aout 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
struct xvimage * lcrop(struct xvimage *in, int32_t x, int32_t y, int32_t w, int32_t h) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lcrop"
{
  int32_t i, j, i0, j0, i1, j1, x0, y0, x1, y1, xx, yy;
  int32_t rs, cs;
  struct xvimage * temp1;

  rs = in->row_size;
  cs = in->col_size;
  i0 = 0; i1 = w;
  j0 = 0; j1 = h;
  x0 = x; y0 = y;
  x1 = x+w; y1 = y+h;

  if ((x0 >= rs) || (y0 >= cs) || (x1 < 0) || (y1 < 0))
  {
    fprintf(stderr, "%s : out of bounds\n", F_NAME);
    return NULL;
  }

  if (x0 < 0) { i1 += x0; x0 = 0; }
  if (y0 < 0) { j1 += y0; y0 = 0; }
  if (x1 > rs) { i1 -= (x1-rs); x1 = rs; }
  if (y1 > cs) { j1 -= (y1-cs); y1 = cs; }

  temp1 = allocimage(NULL, i1, j1, 1, datatype(in));
  if (temp1 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *T1 = UCHARDATA(temp1);
    uint8_t *I = UCHARDATA(in);
    for (j = j0, yy = y0; j < j1; j++, yy++)
      for (i = i0, xx = x0; i < i1; i++, xx++)
      {
        T1[(j * i1) + i] = I[(yy * rs) + xx];
      }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    uint32_t *T1L = ULONGDATA(temp1);
    uint32_t *IL = ULONGDATA(in);
    for (j = j0, yy = y0; j < j1; j++, yy++)
      for (i = i0, xx = x0; i < i1; i++, xx++)
      {
        T1L[(j * i1) + i] = IL[(yy * rs) + xx];
      }
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *T1F = FLOATDATA(temp1);
    float *IF = FLOATDATA(in);
    for (j = j0, yy = y0; j < j1; j++, yy++)
      for (i = i0, xx = x0; i < i1; i++, xx++)
      {
        T1F[(j * i1) + i] = IF[(yy * rs) + xx];
      }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }
  return temp1;
} // lcrop()

/* =============================================================== */
struct xvimage * lcrop3d(struct xvimage *in, int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lcrop3d"
{
  int32_t i, j, k, i0, j0, k0, i1, j1, k1, x0, y0, z0, x1, y1, z1, xx, yy, zz;
  int32_t rs, cs, ds, ps, p1;
  struct xvimage *temp1;

  rs = rowsize(in);     /* taille rangee image originale */
  cs = colsize(in);     /* taille colonne image originale */
  ds = depth(in);       /* nb. plans image originale */
  ps = rs * cs;         /* taille plan image originale */

  i0 = 0; i1 = w;
  j0 = 0; j1 = h;
  k0 = 0; k1 = d;
  x0 = x; y0 = y; z0 = z;
  x1 = x+w; y1 = y+h; z1 = z+d;

  if ((x0 >= rs) || (y0 >= cs) || (z0 >= ds) || (x1 < 0) || (y1 < 0) || (z1 < 0))
  {
    fprintf(stderr, "%s : out of bounds\n", F_NAME);
    return NULL;
  }

  if (x0 < 0) { i1 += x0; x0 = 0; }
  if (y0 < 0) { j1 += y0; y0 = 0; }
  if (z0 < 0) { k1 += z0; z0 = 0; }
  if (x1 > rs) { i1 -= (x1-rs); x1 = rs; }
  if (y1 > cs) { j1 -= (y1-cs); y1 = cs; }
  if (z1 > ds) { k1 -= (z1-ds); z1 = ds; }

  p1 = i1 * j1;          /* taille plan image finale */

  temp1 = allocimage(NULL, i1, j1, k1, datatype(in));
  if (temp1 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *T1 = UCHARDATA(temp1);
    uint8_t *I = UCHARDATA(in);
    for (k = k0, zz = z0; k < k1; k++, zz++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
        for (i = i0, xx = x0; i < i1; i++, xx++)
        {
          T1[(k * p1) + (j * i1) + i] = I[(zz * ps) + (yy * rs) + xx];
        }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    uint32_t *T1L = ULONGDATA(temp1);
    uint32_t *IL = ULONGDATA(in);
    for (k = k0, zz = z0; k < k1; k++, zz++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
        for (i = i0, xx = x0; i < i1; i++, xx++)
        {
          T1L[(k * p1) + (j * i1) + i] = IL[(zz * ps) + (yy * rs) + xx];
        }
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *T1F = FLOATDATA(temp1);
    float *IF = FLOATDATA(in);
    for (k = k0, zz = z0; k < k1; k++, zz++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
        for (i = i0, xx = x0; i < i1; i++, xx++)
        {
          T1F[(k * p1) + (j * i1) + i] = IF[(zz * ps) + (yy * rs) + xx];
        }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }
  return temp1;
} // lcrop3d()

/* =============================================================== */
struct xvimage * lencadre(struct xvimage *image, int32_t grayval) 
/* =============================================================== */
// adds a border with a given gray value to an image
// obsolete - utiliser lenframe()
#undef F_NAME
#define F_NAME "lencadre"
{
  int32_t rs, cs, ds, ps, x, y, z;
  int32_t rs2, cs2, ds2, ps2;
  struct xvimage * imageout;

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  rs2 = rs + 2;
  cs2 = cs + 2;
  ds2 = ds + 2;
  ps2 = rs2 * cs2;

  if (ds > 1)
  {
    imageout = allocimage(NULL, rs+2, cs+2, ds+2, datatype(image));
    if (imageout == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", F_NAME);
      return NULL;
    }
  }
  else
  {
    imageout = allocimage(NULL, rs+2, cs+2, 1, datatype(image));
    if (imageout == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", F_NAME);
      return NULL;
    }
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *Imout = UCHARDATA(imageout);
    uint8_t *Im = UCHARDATA(image);
    uint8_t grayvalue = (uint8_t)grayval;

    if (ds > 1)
    {
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[0 * ps2 + y * rs2 + x] = grayvalue;          /* plan z = 0 */
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1) * ps2 + y * rs2 + x] = grayvalue;    /* plan z = ds2-1 */
  
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + 0 * rs2 + x] = grayvalue;          /* plan y = 0 */
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1) * rs2 + x] = grayvalue;    /* plan y = cs2-1 */
  
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + 0] = grayvalue;          /* plan x = 0 */
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1)] = grayvalue;    /* plan x = rs2-1 */
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
      for (z = 1; z < ds2-1; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-1) * ps + (y-1) * rs + (x-1)];
    }
    else
    {
      for (x = 0; x < rs2; x++) Imout[x] = grayvalue;
      for (x = 0; x < rs2; x++) Imout[(cs2 - 1) * rs2 + x] = grayvalue;
  
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2] = grayvalue;
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2 + rs2 - 1] = grayvalue;
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
        Imout[y * rs2 + x] = Im[(y-1) * rs + (x-1)];
    }
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    uint32_t *Imout = ULONGDATA(imageout);
    uint32_t *Im = ULONGDATA(image);
    uint32_t grayvalue = (uint32_t)grayval;

    if (ds > 1)
    {
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[0 * ps2 + y * rs2 + x] = grayvalue;          /* plan z = 0 */
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1) * ps2 + y * rs2 + x] = grayvalue;    /* plan z = ds2-1 */
  
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + 0 * rs2 + x] = grayvalue;          /* plan y = 0 */
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1) * rs2 + x] = grayvalue;    /* plan y = cs2-1 */
  
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + 0] = grayvalue;          /* plan x = 0 */
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1)] = grayvalue;    /* plan x = rs2-1 */
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
      for (z = 1; z < ds2-1; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-1) * ps + (y-1) * rs + (x-1)];
    }
    else
    {
      for (x = 0; x < rs2; x++) Imout[x] = grayvalue;
      for (x = 0; x < rs2; x++) Imout[(cs2 - 1) * rs2 + x] = grayvalue;
  
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2] = grayvalue;
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2 + rs2 - 1] = grayvalue;
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
        Imout[y * rs2 + x] = Im[(y-1) * rs + (x-1)];
    }
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *Imout = FLOATDATA(imageout);
    float *Im = FLOATDATA(image);
    float grayvalue = (float)grayval;

    if (ds > 1)
    {
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[0 * ps2 + y * rs2 + x] = grayvalue;          /* plan z = 0 */
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1) * ps2 + y * rs2 + x] = grayvalue;    /* plan z = ds2-1 */
  
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + 0 * rs2 + x] = grayvalue;          /* plan y = 0 */
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1) * rs2 + x] = grayvalue;    /* plan y = cs2-1 */
  
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + 0] = grayvalue;          /* plan x = 0 */
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1)] = grayvalue;    /* plan x = rs2-1 */
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
      for (z = 1; z < ds2-1; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-1) * ps + (y-1) * rs + (x-1)];
    }
    else
    {
      for (x = 0; x < rs2; x++) Imout[x] = grayvalue;
      for (x = 0; x < rs2; x++) Imout[(cs2 - 1) * rs2 + x] = grayvalue;
  
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2] = grayvalue;
      for (y = 1; y < cs2 - 1; y++) Imout[y * rs2 + rs2 - 1] = grayvalue;
  
      for (x = 1; x < rs2-1; x++)
      for (y = 1; y < cs2-1; y++) 
        Imout[y * rs2 + x] = Im[(y-1) * rs + (x-1)];
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }

  return imageout;
} /* lencadre() */

/* =============================================================== */
struct xvimage * lenframe(struct xvimage *image, int32_t grayval, int32_t width) 
/* =============================================================== */
// adds a border with a given gray value to an image
#undef F_NAME
#define F_NAME "lenframe"
{
  int32_t rs, cs, ds, ps, x, y, z;
  int32_t rs2, cs2, ds2, ps2;
  struct xvimage * imageout;

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  rs2 = rs + 2*width;
  cs2 = cs + 2*width;
  ds2 = ds + 2*width;
  ps2 = rs2 * cs2;

  if (ds > 1)
  {
    imageout = allocimage(NULL, rs+2*width, cs+2*width, ds+2*width, datatype(image));
    if (imageout == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", F_NAME);
      return NULL;
    }
  }
  else
  {
    imageout = allocimage(NULL, rs+2*width, cs+2*width, 1, datatype(image));
    if (imageout == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", F_NAME);
      return NULL;
    }
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *Imout = UCHARDATA(imageout);
    uint8_t *Im = UCHARDATA(image);
    uint8_t grayvalue = (uint8_t)grayval;

    if (ds > 1)
    {
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans z = 0.. */
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1-z) * ps2 + y * rs2 + x] = grayvalue;    /* plans z = ..ds2-1 */
  
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans y = 0.. */
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue;    /* plans y = ..cs2-1 */
  
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans x = 0.. */
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue;    /* plans x = ..rs2-1 */
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
      for (z = width; z < ds2-width; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-width) * ps + (y-width) * rs + (x-width)];
    }
    else
    {
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[y*rs2 + x] = grayvalue;

      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[(cs2-1-y)*rs2 + x] = grayvalue;
  
      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + x] = grayvalue;

      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + rs2-1-x] = grayvalue;
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
        Imout[y * rs2 + x] = Im[(y-width) * rs + (x-width)];
    }
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    uint32_t *Imout = ULONGDATA(imageout);
    uint32_t *Im = ULONGDATA(image);
    uint32_t grayvalue = (uint32_t)grayval;

    if (ds > 1)
    {
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans z = 0.. */
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1-z) * ps2 + y * rs2 + x] = grayvalue;    /* plans z = ..ds2-1 */
  
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans y = 0.. */
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue;    /* plans y = ..cs2-1 */
  
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans x = 0.. */
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue;    /* plans x = ..rs2-1 */
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
      for (z = width; z < ds2-width; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-width) * ps + (y-width) * rs + (x-width)];
    }
    else
    {
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[y*rs2 + x] = grayvalue;

      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[(cs2-1-y)*rs2 + x] = grayvalue;
  
      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + x] = grayvalue;

      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + rs2-1-x] = grayvalue;
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
        Imout[y * rs2 + x] = Im[(y-width) * rs + (x-width)];
    }
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *Imout = FLOATDATA(imageout);
    float *Im = FLOATDATA(image);
    float grayvalue = (float)grayval;

    if (ds > 1)
    {
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans z = 0.. */
      for (z = 0; z < width; z++)
      for (x = 0; x < rs2; x++)
      for (y = 0; y < cs2; y++) 
        Imout[(ds2-1-z) * ps2 + y * rs2 + x] = grayvalue;    /* plans z = ..ds2-1 */
  
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans y = 0.. */
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue;    /* plans y = ..cs2-1 */
  
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + x] = grayvalue;          /* plans x = 0.. */
      for (x = 0; x < width; x++)
      for (y = 0; y < cs2; y++)
      for (z = 0; z < ds2; z++) 
        Imout[z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue;    /* plans x = ..rs2-1 */
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
      for (z = width; z < ds2-width; z++) 
        Imout[z * ps2 + y * rs2 + x] = Im[(z-width) * ps + (y-width) * rs + (x-width)];
    }
    else
    {
      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[y*rs2 + x] = grayvalue;

      for (y = 0; y < width; y++)
      for (x = 0; x < rs2; x++) Imout[(cs2-1-y)*rs2 + x] = grayvalue;
  
      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + x] = grayvalue;

      for (x = 0; x < width; x++)
      for (y = 1; y < cs2 - 1; y++) Imout[y*rs2 + rs2-1-x] = grayvalue;
  
      for (x = width; x < rs2-width; x++)
      for (y = width; y < cs2-width; y++) 
        Imout[y * rs2 + x] = Im[(y-width) * rs + (x-width)];
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }

  return imageout;
} /* lencadre() */

/* =============================================================== */
int32_t linsert(struct xvimage *a, struct xvimage *b, int32_t x, int32_t y, int32_t z) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "linsert"
{
  int32_t i, j, k;
  int32_t rsa, csa, dsa, psa, rsb, csb, dsb, psb;

  rsa = rowsize(a);
  rsb = rowsize(b);
  csa = colsize(a);
  csb = colsize(b);
  dsa = depth(a);
  dsb = depth(b);
  psa = rsa * csa;
  psb = rsb * csb;

  if (datatype(a) != datatype(b))
  {
    fprintf(stderr, "%s : incompatible data types\n", F_NAME);
    return 0;
  }

  if (datatype(a) == VFF_TYP_1_BYTE)
  {
    uint8_t *A = UCHARDATA(a);
    uint8_t *B = UCHARDATA(b);

    for (k = 0; k < dsa; k++)
      for (j = 0; j < csa; j++)
        for (i = 0; i < rsa; i++)
          if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
              (z + k >= 0) && (y + j >= 0) && (x + i >= 0))
            B[((z + k) * psb) + ((y + j) * rsb) + x + i] = 
              A[(k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_4_BYTE)
  {
    uint32_t *A = ULONGDATA(a);
    uint32_t *B = ULONGDATA(b);

    for (k = 0; k < dsa; k++)
      for (j = 0; j < csa; j++)
        for (i = 0; i < rsa; i++)
          if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
              (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
            B[((z + k) * psb) + ((y + j) * rsb) + x + i] = 
              A[(k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_FLOAT)
  {
    float *A = FLOATDATA(a);
    float *B = FLOATDATA(b);

    for (k = 0; k < dsa; k++)
      for (j = 0; j < csa; j++)
        for (i = 0; i < rsa; i++)
          if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
              (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
            B[((z + k) * psb) + ((y + j) * rsb) + x + i] = 
              A[(k * psa) + (j * rsa) + i]; 
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return 0;
  }

  return 1;
} // linsert()

/* =============================================================== */
struct xvimage * lexpandframe(struct xvimage *in, int32_t n) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lexpandframe"
{
  int32_t ds1 = depth(in);
  int32_t rs1 = rowsize(in);
  int32_t cs1 = colsize(in);
  int32_t rs2, cs2, x, y;
  struct xvimage *temp1;

  if (ds1 > 1)
  {
    fprintf(stderr, "%s: 3D Not Yet Implemented\n", F_NAME);
    return NULL;
  }
  else
  { 
    if (datatype(in) == VFF_TYP_1_BYTE)
    {
      uint8_t *T1, *I = UCHARDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_1_BYTE);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = UCHARDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else if (datatype(in) == VFF_TYP_4_BYTE)
    {
      uint32_t *T1, *I = ULONGDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_4_BYTE);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = ULONGDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else if (datatype(in) == VFF_TYP_FLOAT)
    {
      float *T1, *I = FLOATDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = FLOATDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else
    {
      fprintf(stderr, "%s: bad datatype\n", F_NAME);
      return NULL;
    }

  }

  return temp1;
} // lexpandframe()
