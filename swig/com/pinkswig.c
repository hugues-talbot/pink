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
#include <lseuil.h>
#include <ldist.h>
#include <lcrop.h>
#include <larith.h>
#include <lborder.h>

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

struct xvimage* checkAllocCopy(struct xvimage* imagein, char *name)
{
  if (imagein == NULL)
    {
      fprintf(stderr, "%s:  NULL image - can not process\n", name);
      return NULL;
    }
  
  struct xvimage * image=allocimage(NULL, rowsize(imagein), colsize(imagein), depth(imagein), datatype(imagein));
  if (image == NULL)
    {
      fprintf(stderr, "%s:  malloc failed\n", name);
      return NULL;
    }
  copy(image,imagein);
  return image;
}

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

// threshold(image, seuil1, seuil2)
// if seuil2<=0, then only the first seuil is used
// Always return a single-byte image
struct xvimage* threshold(struct xvimage *imagein, double seuil, double seuil2)
{

  if (imagein == NULL)
  {
    fprintf(stderr, "%s:  NULL image - can not process\n", "threshold");
    return NULL;
  }

  struct xvimage * image=allocimage(NULL, rowsize(imagein), colsize(imagein), depth(imagein), datatype(imagein));
  if (image == NULL)
  {
    fprintf(stderr, "%s:  malloc failed\n", "threshold");
    return NULL;
  }
  copy(image,imagein);


  if (seuil2 <= 0.)
  {
    if (! lseuil(image, seuil))
      {
	fprintf(stderr, "%s: function lseuil failed\n", "threshold");
	freeimage(image);
	return NULL;
      }
  }
  else
  {
    if (! lseuil3(image, seuil, seuil2))
      {
	fprintf(stderr, "%s: function lseuil3 failed\n", "threshold");
	freeimage(image);
	return NULL;
      }
  }

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    index_t rs, cs, ds, N, x;
    uint8_t *F;
    rs = rowsize(image); cs = colsize(image); ds = depth(image); N = rs * cs * ds; 
    struct xvimage *imagebin = allocimage(image->name, rs, cs, ds, VFF_TYP_1_BYTE);
    if (imagebin == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", "threshold");
      freeimage(image);
      return NULL;
    }
    F = UCHARDATA(imagebin); 

    if (datatype(image) == VFF_TYP_4_BYTE)
    {
      int32_t *FL = SLONGDATA(image);
      for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    }
    else if (datatype(image) == VFF_TYP_2_BYTE)
    {
      int16_t *FL = SSHORTDATA(image);
      for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    }
    else if (datatype(image) == VFF_TYP_FLOAT)
    {
      float *FL = FLOATDATA(image);
      for (x = 0; x < N; x++) if (FL[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
    }
    else if (datatype(image) == VFF_TYP_DOUBLE)
    {
      double *FL = DOUBLEDATA(image);
      for (x = 0; x < N; x++) if (FL[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
    }
    else
    {
      fprintf(stderr, "%s: bad data type %d\n", "threshold", datatype(image));
      freeimage(image);
      freeimage(imagebin);
      return NULL;
    }
    return imagebin;
    freeimage(image);
  }
  return image;
}

struct xvimage* erosball(struct xvimage *imagein, int r, int mode)
{
  static char* name="erosball";
  
  if ((mode != 0) && (mode != 2) && (mode != 3) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
    {
      fprintf(stderr, "%s: dist = [0|2|3|4|8|6|18|26] \n", name);
      return NULL;
    }
    
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  
  if (! lerosball(image, r, mode))
  {
    fprintf(stderr, "%s: function lerosball failed\n", "erosball");
    freeimage(image);
    return NULL;
  }

  return image;
}

struct xvimage* dilatball(struct xvimage *imagein, int r, int mode)
{
  static char* name="dilatball";

  if ((mode != 0) && (mode != 2) && (mode != 3) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
    {
      fprintf(stderr, "%s: dist = [0|2|3|4|8|6|18|26] \n", name);
      return NULL;
    }
    
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (! ldilatball(image, r, mode))
  {
    fprintf(stderr, "%s: function ldilatball failed\n", name);
    freeimage(image);
    return NULL;
  }

  return image;
}


struct xvimage* frame(struct xvimage *imagein, int width)
{
  static char* name="frame";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  razimage(image);
  lsetthickframe(image, width, NDG_MAX);
  return image;
}

struct xvimage* inverse(struct xvimage *imagein)
{
  static char* name="inverse";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (! linverse(image))
  {
    fprintf(stderr, "%s: function linverse failed\n", name);
    freeimage(image);
    return NULL;
  }
  
  return image;
}


struct xvimage* min(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="min";
  
  struct xvimage *image1 = checkAllocCopy(imagein1, name);
  if (image1 == NULL)
    return NULL;
  struct xvimage *image2 = checkAllocCopy(imagein2, name);
  if (image2 == NULL) {
    freeimage(image1);
    return NULL;
  }
  
  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  if (! lmin(image1, image2))
  {
    fprintf(stderr, "%s: function lmin failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  freeimage(image2);
  return image1;
}

struct xvimage* max(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="max";
  
  struct xvimage *image1 = checkAllocCopy(imagein1, name);
  if (image1 == NULL)
    return NULL;
  struct xvimage *image2 = checkAllocCopy(imagein2, name);
  if (image2 == NULL) {
    freeimage(image1);
    return NULL;
  }
  
  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  if (! lmax(image1, image2))
  {
    fprintf(stderr, "%s: function lmin failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  freeimage(image2);
  return image1;
}

struct xvimage* border(struct xvimage *imagein, int connex)
{
  static char *name="border";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (! mctopo3d_lborder(image, connex))
  {
    fprintf(stderr, "%s: lborder failed\n", name);
    freeimage(image);
    return NULL;
  }

  return image;
}


struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="sub";
  struct xvimage *image1 = checkAllocCopy(imagein1, name);
  if (image1 == NULL)
    return NULL;
  struct xvimage *image2 = checkAllocCopy(imagein2, name);
  if (image2 == NULL) {
    freeimage(image1);
    return NULL;
  }

  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  if (! lsub(image1, image2))
  {
    fprintf(stderr, "%s: function lsub failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }
  
  freeimage(image2);
  return image1;
}

struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="add";
  struct xvimage *image1 = checkAllocCopy(imagein1, name);
  if (image1 == NULL)
    return NULL;
  struct xvimage *image2 = checkAllocCopy(imagein2, name);
  if (image2 == NULL) {
    freeimage(image1);
    return NULL;
  }

  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }

  if (! ladd(image1, image2))
  {
    fprintf(stderr, "%s: function ladd failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }
  
  freeimage(image2);
  return image1;
}
