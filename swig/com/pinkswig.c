#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#include "pinkswig.h"

#include <mccodimage.h>
#include <mcimage.h>

#include <lwshedtopo.h>
#include <lminima.h>
#include <ldilateros.h>
#include <ldilateros3d.h>
#include <lsym.h>
#include <lgeodesic.h>


/* ==================================== */
// Helper function to use with numpy (dataInt should be uintptr_t)
xvimage *  createimage(index_t x, index_t y, index_t z, int32_t type, long int dataInt) {
/* ==================================== */
  xvimage *im;
  void *data = (void*) dataInt;
  if (data == NULL) 
    im = allocimage(NULL, x, y, z, type);
  else {
    // data is a pointer to an already allocated zone of memory
    im = allocheader(NULL, x, y, z, type);
    im->image_data = (void *)data;
  }
  return im;
}

/* ==================================== */
void invert(struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt;
  for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    *pt = NDG_MAX - *pt;
} // inverse

/* ==================================== */
void copy(struct xvimage*result, struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt, *r;
  for (pt = UCHARDATA(image), r=UCHARDATA(result), i = 0; i < N; i++, pt++, r++)
    *r = *pt;
} // copy

void copyinvert(struct xvimage*result, struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt, *r;
  for (pt = UCHARDATA(image), r=UCHARDATA(result), i = 0; i < N; i++, pt++, r++)
    *r = NDG_MAX - *pt;
} // copyinverse


struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse)
{

  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));

  if (inverse)
    copyinvert(result,image);
  else
    copy(result,image);
  
  if (! lwshedtopo_lwshedtopo(result, connex))
  {
    fprintf(stderr, "%s: lwshedtopo_lwshedtopo failed\n", "wshedtopo");
    freeimage(result);
    return NULL;
  }
  
  if (inverse)
    invert(result);
  
  return result;
}

struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse)
{

  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));

  if (inverse)
    copyinvert(result,image);
  else
    copy(result,image);
  
  if (! lwshedtopobin(result, mark, connex))
  {
    fprintf(stderr, "%s: lwshedtopo_lwshedtopo failed\n", "wshedtopo");
    freeimage(result);
    return NULL;
  }
  
  if (inverse)
    invert(result);
  
  return result;
}

struct xvimage* minima(struct xvimage *image, int connex)
{
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  copy(result,image);
  char c[20];
  sprintf(c, "%d", connex);
  if (!lminima(result, c))
    {
      fprintf(stderr, "%s: lminima failed\n", "minima");
      return(NULL);
    } /* if ! lminima*/
  return result;
}

struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z)
{
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  copy(result,image);

  if (x==-1)
    x = rowsize(elem)/2;
  
  if (y==-1)
    y = colsize(elem)/2;
  
  if (z==-1)
    z = depth(elem)/2;

  if (depth(image) == 1) {
    if (! ldilateros_ldilat(result, elem, x, y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "dilation");
	freeimage(result);
	return(NULL);
      }
  } else {
    if (! ldilat3d(result, elem, x, y, z))
      {
	fprintf(stderr, "%s: function ldilat3d failed\n", "dilation");
	freeimage(result);
	return(NULL);
      }
   }
  return result;
}


struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z)
{
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  copy(result,image);

  if (x==-1)
    x = rowsize(elem)/2;
  
  if (y==-1)
    y = colsize(elem)/2;
  
  if (z==-1)
    z = depth(elem)/2;
  
  if (depth(image) == 1) {
    if (! ldilateros_leros(result, elem, x, y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "erosion");
	freeimage(result);
	return(NULL);
      }
  } else {
    if (! leros3d(result, elem, x, y, z))
      {
	fprintf(stderr, "%s: function ldilat3d failed\n", "erosion");
	freeimage(result);
	return(NULL);
      }
   }
  return result;
}

struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z)
{
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  copy(result,image);

  if (x==-1)
    x = rowsize(elem)/2;
  
  if (y==-1)
    y = colsize(elem)/2;
  
  if (z==-1)
    z = depth(elem)/2;

  
  index_t rs, cs, ds;
  rs = rowsize(elem);
  cs = colsize(elem);
  ds = depth(elem);
  
  if (depth(image) == 1) {
    if (! ldilateros_leros(result, elem, x, y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
    if (! lsym(elem, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
    if (! ldilateros_ldilat(result, elem, rs - 1 - x, cs - 1 - y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
  } else {
    if (! leros3d(result, elem, x, y, z))
      {
	fprintf(stderr, "%s: function leros3d failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
    if (! lsym(elem, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
    if (! ldilat3d(image, elem, rs - 1 - x, cs - 1 - y, ds - 1 - z))
      {
	fprintf(stderr, "%s: function ldilat3d failed\n", "opening");
	freeimage(result);
	return(NULL);
      }
   }
  return result;
}

struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z)
{
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  copy(result,image);

  if (x==-1)
    x = rowsize(elem)/2;
  
  if (y==-1)
    y = colsize(elem)/2;
  
  if (z==-1)
    z = depth(elem)/2;

  
  index_t rs, cs, ds;
  rs = rowsize(elem);
  cs = colsize(elem);
  ds = depth(elem);
  
  if (depth(image) == 1) {
    if (! ldilateros_ldilat(result, elem, rs - 1 - x, cs - 1 - y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
    if (! lsym(elem, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
    if (! ldilateros_leros(result, elem, x, y))
      {
	fprintf(stderr, "%s: function ldilat failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
  } else {
    if (! ldilat3d(image, elem, rs - 1 - x, cs - 1 - y, ds - 1 - z))
      {
	fprintf(stderr, "%s: function ldilat3d failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
    if (! lsym(elem, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
    if (! leros3d(result, elem, x, y, z))
      {
	fprintf(stderr, "%s: function leros3d failed\n", "closing");
	freeimage(result);
	return(NULL);
      }
   }
  return result;
}


struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter)
{
  struct xvimage * result=allocimage(NULL, rowsize(image1), colsize(image1), depth(image1), datatype(image1));
  copy(result,image1);

  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    fprintf(stderr, "%s: bad connexity - use one of the following:\n", "geodilat");
    fprintf(stderr, "4, 8 (in 2d), 6, 18, 26 (in 3d)\n");
    freeimage(result);
    return(NULL);
  }

  if (! lgeodilat(result, image2, connex, niter))
  {
    fprintf(stderr, "%s: function lgeodilat failed\n", "geodilat");
    freeimage(result);
    return(NULL);
  }

  return result;
}

struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter)
{
  struct xvimage * result=allocimage(NULL, rowsize(image1), colsize(image1), depth(image1), datatype(image1));
  copy(result,image1);

  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    fprintf(stderr, "%s: bad connexity - use one of the following:\n", "geoeros");
    fprintf(stderr, "4, 8 (in 2d), 6, 18, 26 (in 3d)\n");
    freeimage(result);
    return(NULL);
  }
  
  if (depth(image1) == 1) {
    if (! lgeoeros(result, image2, connex, niter))
      {
	fprintf(stderr, "%s: function lgeodilat failed\n", "geoeros");
	freeimage(result);
	return(NULL);
      }
  } else  {
    if (! lgeoeros3d(result, image2, connex, niter))
      {
	fprintf(stderr, "%s: function lgeoeros3d failed\n", "geoeros");
	freeimage(result);
	return(NULL);
      }
  }
  
  return result;
}
