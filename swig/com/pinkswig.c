#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "pinkswig.h"

#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <mcutil.h>

#include <lconvert.h>

#include <lseuil.h>
#include <lcrop.h>
#include <larith.h>
#include <lborder.h>
#include <ldraw.h>

#include <ldist.h>

#include <lskeletons.h>
#include <lseltopo.h>

#include <ldilateros.h>
#include <ldilateros3d.h>
#include <lsym.h>
#include <lgeodesic.h>

#include <lminima.h>
#include <lmaxima.h>

#include <lattribheight.h>
#include <lattribarea.h>
#include <lattribvol.h>

#include <lwshedtopo.h>

#include <jccodimage.h>
#include <jcimage.h>
#include <lppm2GA.h>
#include <lMSF.h>

#include <lga2khalimsky.h>
#include <llabelextrema.h>

/* ==================================== */
// Helper function to use with numpy (dataInt should be uintptr_t)
xvimage *  createimage(index_t x, index_t y, index_t z, int32_t type, long int dataInt) {
/* ==================================== */
  xvimage *im=NULL;
  void *data = (void*) dataInt;
  if (data == NULL) {
    //im = allocimage(NULL, x, y, z, type);
    fprintf(stderr, "createimage: cannot create an image from NULL data\n");
  } else {
    // data is a pointer to an already allocated zone of memory
    im = allocheader(NULL, x, y, z, type);
    im->image_data = (void *)data;
  }
  return im;
}

/* ==================================== */
int EqualSize(struct xvimage * image1, struct xvimage *image2)
/* ==================================== */
{
  if (   (depth(image1) == depth(image2))
      && (rowsize(image1) == rowsize(image2))
      && (colsize(image1) == colsize(image2))
      )
    return 1;
  else
    return 0;
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
  static char *name="wshedtopo";
  if (image == NULL)
    {
      fprintf(stderr, "%s:  NULL image - can not process\n", name);
      return NULL;
    }
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  if (result == NULL)
    {
      fprintf(stderr, "%s:  malloc failed\n", name);
      return NULL;
    }
  if (inverse)
    copyinvert(result,image);
  else
    copy(result,image);
  
  if (! lwshedtopo_lwshedtopo(result, connex))
  {
    fprintf(stderr, "%s: lwshedtopo_lwshedtopo failed\n", name);
    freeimage(result);
    return NULL;
  }
  
  if (inverse)
    invert(result);
  
  return result;
}

struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse)
{
  static char *name="watershed";
  if ((image == NULL) || (mark == NULL))
    {
      fprintf(stderr, "%s:  NULL image - can not process\n", name);
      return NULL;
    }
  if (!EqualSize(image, mark) ) {
      fprintf(stderr, "%s:  Images not of the same size - can not process\n", name);
      return NULL;
  }    
  struct xvimage * result=allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  if (result == NULL)
    {
      fprintf(stderr, "%s:  malloc failed\n", name);
      return NULL;
    }
  if (inverse)
    copyinvert(result,image);
  else
    copy(result,image);
  
  if (! lwshedtopobin(result, mark, connex))
  {
    fprintf(stderr, "%s: lwshedtopo_lwshedtopo failed\n", name);
    freeimage(result);
    return NULL;
  }
  
  if (inverse)
    invert(result);
  
  return result;
}

struct xvimage* minima(struct xvimage *image, int connex)
{
  static char *name="minima";
  struct xvimage * result=checkAllocCopy(image, name);
  if (result == NULL)
    return NULL;
  
  char c[20];
  sprintf(c, "%d", connex);
  if (!lminima(result, c))
    {
      fprintf(stderr, "%s: lminima failed\n", name);
      return(NULL);
    } /* if ! lminima*/
  return result;
}

struct xvimage* maxima(struct xvimage *image, int connex)
{
  static char *name="maxima";
  struct xvimage * result=checkAllocCopy(image, name);
  if (result == NULL)
    return NULL;
  
  if (!lmaxima(result, connex))
    {
      fprintf(stderr, "%s: lminima failed\n", "maxima");
      return(NULL);
    } /* if ! lmaxima*/
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

  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
  
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
  
  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }

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
  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
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
  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
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

struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="mult";
  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
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

  if (! lmult(image1, image2))
  {
    fprintf(stderr, "%s: function lmult failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }
  
  freeimage(image2);
  return image1;
}

struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2)
{
  static char *name="div";
  if (!EqualSize(imagein1, imagein2)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
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

  if (! ldivide(image1, image2))
  {
    fprintf(stderr, "%s: function ldivide failed\n", name);
    freeimage(image1);
    freeimage(image2);
    return NULL;
  }
  
  freeimage(image2);
  return image1;
}

struct xvimage* heightmaxima(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="heightmaxima";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  if (! lheightmaxima(image, connex, param))
  {
    fprintf(stderr, "%s: lheightmaxima failed\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

struct xvimage* heightminima(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="heightminima";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  if (! lheightminima(image, connex, param))
  {
    fprintf(stderr, "%s: lheightminima failed\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

// param == param+1
struct xvimage* areaopening(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="areaopening";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  if (! lareaopening(image, connex, param+1))
  {
    fprintf(stderr, "%s: lareaopening failed\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

// param == param+1
struct xvimage* areaclosing(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="areaclosing";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  if (! lareaclosing(image, connex, param+1))
  {
    fprintf(stderr, "%s: lareaclosing failed\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

struct xvimage* volmaxima(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="volmaxima";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  if (! lvolmaxima(image, connex, param))
  {
    fprintf(stderr, "%s: lvolmaxima failed\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

struct xvimage* volminima(struct xvimage *imagein, int32_t param, int32_t connex)
{
  static char *name="volminima";
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
  invert(image);
  if (! lvolmaxima(image, connex, param))
  {
    fprintf(stderr, "%s: lvolmaxima failed\n", name);
    freeimage(image);
    return NULL;
  }
  invert(image);
  return image;
}

struct xvimage* dist(struct xvimage *image, int32_t mode)
{
  index_t N, i;
  uint8_t *F;
  struct xvimage *result = NULL;
    
  static char *name="dist";
  if (image == NULL)
  {
    fprintf(stderr, "%s: NULL image - this is a problem.\n", name);
    return NULL;

  }
  
  if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
      (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
      (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
    {
      fprintf(stderr, "usage: %s mode is not correct.\n", name);
      fprintf(stderr, "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n");
      fprintf(stderr, "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n");
      fprintf(stderr, "                40, 80 (2D), 60, 180, 260 (3D)\n");
      return NULL;
    }
  
  if (mode < 40)
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  else
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  if (result == NULL) {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  N = rowsize(image) * colsize(image) * depth(image);
  F = UCHARDATA(image);;
  if (mode == 1)
  {
    if (depth(image) == 1)
    {
      if (! ldistquad(image, result))
      {
        fprintf(stderr, "%s: ldistquad failed\n", name);
        return NULL;
      }
    }
    else
    {
      if (! ldistquad3d(image, result))
      {
        fprintf(stderr, "%s: ldistquad3d failed\n", name);
        return NULL;
      }
    }
  }
  else if (mode == 2)
  {
    if (! lchamfrein(image, result))
    {
      fprintf(stderr, "%s: lchamfrein failed\n", name);
      return NULL;
    }
  }
  else if ((mode == 0) || (mode == 3) || (mode == 5))
  {
    for (i = 0; i < N; i++) // inverse l'image
      if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
    if (! lsedt_meijster(image, result))
    {
      fprintf(stderr, "%s: lsedt_meijster failed\n", name);
      return NULL;
    }
    if (mode == 0)
    {
      double d;
      uint32_t *R = ULONGDATA(result);
      for (i = 0; i < N; i++) 
      {
	
	d = sqrt((double)(R[i]));
	R[i] = (uint32_t)arrondi(d);
      }
    }
    else if (mode == 5)
    {
      float *D;
      convertfloat(&result);
      D = FLOATDATA(result);
      for (i = 0; i < N; i++) D[i] = (float)sqrt(D[i]);
    }
  }
  else if (mode < 40)
  {
    if (! ldist(image, mode, result))
    {
      fprintf(stderr, "%s: ldist failed\n", name);
      return NULL;
    }
  }
  else
  {
    if (! ldistbyte(image, mode, result))
    {
      fprintf(stderr, "%s: ldist failed\n", name);
      return NULL;
    }
  }

  return result;
}
  
struct xvimage* skeletonprio2(struct xvimage *imagein, struct xvimage *prio, int32_t connex, struct xvimage *inhibimage)
{
  static char *name="skeletonprio2";
  if (prio == NULL) {
    fprintf(stderr, "%s: Priority image is NULL - This is a problem.", name);
    return NULL;
  }
  if (!EqualSize(imagein, prio)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }
  if (inhibimage)
    if (!EqualSize(imagein, inhibimage)) {
      fprintf(stderr, "%s: image not of the same size\n", name);
      return NULL;
    }
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (depth(image) == 1)
  {
    if (! lskelubp2(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp2 failed\n", name);
	freeimage(image);
        return NULL;
      }
  }
  else
  {
    if (! lskelubp3d2(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp3d2 failed\n", name);
	freeimage(image);
        return NULL;
      }
  }

  return image;
}

struct xvimage* skeletondist2(struct xvimage *imagein, int32_t mode, int32_t connex, struct xvimage *inhibimage)
{
  static char *name="skeletondist2";

  if (inhibimage)
    if (!EqualSize(imagein, inhibimage)) {
      fprintf(stderr, "%s: image not of the same size\n", name);
      return NULL;
    }
  struct xvimage* prio = dist(imagein, mode);
  if (prio == NULL) {
    fprintf(stderr, "%s: ditance failed - Priority image is NULL - This is a problem.", name);
    return NULL;
  }
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (depth(image) == 1)
  {
    if (! lskelubp2(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp2 failed\n", name);
	freeimage(image);
        return NULL;
      }
  }
  else
  {
    if (! lskelubp3d2(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp3d2 failed\n", name);
	freeimage(image);
        return NULL;
      }
  }

  return image;
}

struct xvimage* skeletonprio1(struct xvimage *imagein, struct xvimage *prio, int32_t connex, int32_t inhibvalue)
{
  static char *name="skeletonprio1";
  if (prio == NULL) {
    fprintf(stderr, "%s: Priority image is NULL - This is a problem.", name);
    return NULL;
  }
  if (!EqualSize(imagein, prio)) {
    fprintf(stderr, "%s: image not of the same size\n", name);
    return NULL;
  }

  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (depth(image) == 1)
  {
    if (! lskelubp(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp failed\n", name);
	freeimage(image);
        return NULL;
      }
  }
  else
  {
    if (! lskelubp3d(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp3d failed\n", name);
	freeimage(image);
        return NULL;
      }
  }

  return image;
}

struct xvimage* skeletondist1(struct xvimage *imagein, int32_t mode, int32_t connex, int32_t inhibvalue)
{
  static char *name="skeletondist1";

  struct xvimage* prio = dist(imagein, mode);
  if (prio == NULL) {
    fprintf(stderr, "%s: ditance failed - Priority image is NULL - This is a problem.", name);
    return NULL;
  }
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (depth(image) == 1)
  {
    if (! lskelubp(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp failed\n", name);
	freeimage(image);
        return NULL;
      }
  }
  else
  {
    if (! lskelubp3d(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp3d failed\n", name);
	freeimage(image);
        return NULL;
      }
  }

  return image;
}

struct xvimage* ptisolated(struct xvimage *imagein, int32_t connex)
{
  static char *name="ptisolated";
  
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (! lptisolated(image, connex))
  {
    fprintf(stderr, "%s: function lptisolated failed\n", name);
    return NULL;
  }
  
  return image;
}

struct xvimage* ptjunction(struct xvimage *imagein, int32_t connex)
{
  static char *name="ptjunction";
  
  if (datatype(imagein) == VFF_TYP_4_BYTE)
  {
    struct xvimage * res = allocimage(NULL, rowsize(imagein), colsize(imagein), depth(imagein), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", name);
      return NULL;
    }
    if (! lptjunctionlab(imagein, connex, res))
    {
      fprintf(stderr, "%s: function lptjunctionlab failed\n", name);
      freeimage(res);
      return NULL;
    }
    
    return res;
  } else {
    struct xvimage *image = checkAllocCopy(imagein, name);
    if (image == NULL)
      return NULL;
    
    if (! lptjunction(image, connex))
      {
	fprintf(stderr, "%s: function lptjunction failed\n", name);
	return NULL;
      }
    
    return image;
  }
}

struct xvimage* ptcurve(struct xvimage *imagein, int32_t connex)
{
  static char *name="ptcurve";
  
  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (! lptcurve(image, connex))
  {
    fprintf(stderr, "%s: function lptcurve failed\n", name);
    return NULL;
  }
  
  return image;
}

struct xvimage* ptend(struct xvimage *imagein, int32_t connex)
{
  static char *name="ptend";
  
  if (datatype(imagein) == VFF_TYP_4_BYTE)
  {
    struct xvimage * res = allocimage(NULL, rowsize(imagein), colsize(imagein), depth(imagein), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", name);
      return NULL;
    }
    if (! lptendlab(imagein, connex, res))
    {
      fprintf(stderr, "%s: function lptendlab failed\n", name);
      freeimage(res);
      return NULL;
    }
    
    return res;
  } else {
    struct xvimage *image = checkAllocCopy(imagein, name);
    if (image == NULL)
      return NULL;
    
    if (! lptend(image, connex))
      {
	fprintf(stderr, "%s: function lptend failed\n", name);
	return NULL;
      }
    
    return image;
  }
}

struct xvimage* asfbin(struct xvimage *imagein, int32_t radiusmax, int32_t radiusmin)
{
  static char *name="asfbin";
  int32_t radius;
  int32_t FERMETURE_EN_1 = 0;

  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;
 
  if (radiusmin == 0) 
    {
      radiusmin = 1;
      FERMETURE_EN_1 = 1;
    }

  for (radius = radiusmin; radius <= radiusmax; radius++)
    {
#ifdef VERBOSE
      fprintf(stderr, "%s: radius = %d\n", name, radius);
#endif
      
      if (FERMETURE_EN_1)
	{
	  /* fermeture par l'element structurant */
	  if (! ldilatball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function ldilatball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  if (! lerosball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function lerosball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  /* ouverture par l'element structurant */
	  if (! lerosball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function lerosball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  if (! ldilatball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function ldilatball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	}
      else
	{
	  /* ouverture par l'element structurant */
	  if (! lerosball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function lerosball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  if (! ldilatball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function ldilatball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  /* fermeture par l'element structurant */
	  if (! ldilatball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function ldilatball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	  if (! lerosball(image, radius, 0))
	    {
	      fprintf(stderr, "%s: function lerosball failed\n", name);
	      freeimage(image);
	      return NULL;
	    }
	}
      
    } /* for (radius = 1; radius <= radiusmax; radius++) */

  return image;
}

struct xvimage* ComputeEdgeGraphGrey(struct xvimage* im, int32_t param, double alpha)
{
  static char *name="pgm2GA";
  struct xvimage *ga;
  
  if (im == NULL) {
    fprintf(stderr, "%s: input image is NULL\n", name);
    return NULL;
  }
  switch(datatype(im)){
  case VFF_TYP_1_BYTE:
    if( (ga = allocGAimage(NULL, im->row_size , im->col_size, depth(im), VFF_TYP_GABYTE )) == NULL )
      {
	fprintf(stderr, "%s: edge graph not allocated\n", name);
	return NULL;
      }
    break;
  case VFF_TYP_FLOAT:
    if( (ga = allocGAimage(NULL, im->row_size , im->col_size, depth(im), VFF_TYP_GAFLOAT )) == NULL )
      {
	fprintf(stderr, "%s: edge graph not allocated\n", name);
	return NULL;
      }
    break;
  default:
    {
      fprintf(stderr,"%s: image type not valid (not yet implemented)\n", name);
      return NULL;
    }
  }

  if(depth(im) == 1)
    {
      switch(datatype(im)){
      case VFF_TYP_1_BYTE:
	//fprintf(stderr,"Type de l'image vaut 1\n");
	if( lpgm2ga(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: lppm2ga failed\n", name);
	  freeimage(ga);
	  return NULL;
	}
	break;
      case VFF_TYP_FLOAT:
	if( lpgm2gafloat(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: lppm2gafloat failed\n", name);
	  freeimage(ga);
	  return NULL;
	}
	break;
      }
    }
  else
    if( lpgm2ga3d(im,ga,param) != 1 )
    {
      fprintf(stderr, "%s: lppm2ga failed\n", name);
      freeimage(ga);
      return NULL;
    }
  
  return ga;
}

struct xvimage* ComputeEdgeGraphColor(struct xvimage* r, struct xvimage* v, struct xvimage* b, int32_t param)
{
  static char *name="pgm2GA";
  struct xvimage *ga;
  if ((r == NULL) || (v == NULL) || (b == NULL)) {
    fprintf(stderr, "%s: input image is NULL\n", name);
    return NULL;
  }
  if( (ga = allocGAimage(NULL, r->row_size , r->col_size, depth(r), VFF_TYP_GABYTE )) == NULL )
    {
      fprintf(stderr, "%s: edge graph not allocated\n", name);
      return NULL;
    }

  if(depth(r) == 1)
    {
      if( lppm2ga(r,v,b,ga,param) != 1 )
      {
	fprintf(stderr, "%s: lppm2ga failed\n", name);
	freeimage(ga);
	return NULL;
      }
    }
  else
    {
	fprintf(stderr, "%s: 3D color images not yet implemented\n", name);
	freeimage(ga);
	return NULL;
    }
  return ga;
}

// markers should be labeled
// wc and labels are the returned images - they are allocated here
void watershedcut(struct xvimage* image, struct xvimage* markers,  struct xvimage** wc, struct xvimage** labels)
{
  static char *name="watershedcut";

  *wc = *labels = NULL;
  
  struct xvimage *ga = checkAllocCopy(image, name);
  if (ga == NULL)
    return;
  struct xvimage *labeling = checkAllocCopy(markers, name);
  if (labeling == NULL)
    return;

  int msf = MSF(ga,labeling);
  if (! msf){
    fprintf(stderr, "%s: MSF failed\n", name);
    freeimage(ga);
    freeimage(labeling);
    ga = labeling = NULL;
    return;
  }

  *wc = ga;
  *labels = labeling;
  return;
}

struct xvimage* EWG2Khalimsky(struct xvimage *ga, int32_t bar)
{
  static char *name="EWG2Khalimsky";
  struct xvimage * out = NULL;
  
  if(datatype(ga) != VFF_TYP_GABYTE)
  {
    fprintf(stderr, "%s : input image of a bad format\n", name); 
    return NULL;
  }
  if(depth(ga) == 1){
    out = allocimage("", rowsize(ga)*2, colsize(ga)*2, 1, VFF_TYP_1_BYTE);
    if (out == NULL) {
      fprintf(stderr, "%s : malloc failed.\n", name); 
      return NULL;
    }
    if(!lga2khalimsky(ga, out, bar))
    {
      fprintf(stderr, "%s : lga2khalimsky failed.\n", name); 
      freeimage(out);
      return NULL;
    }
  }
  else
  {
    out = allocimage("", rowsize(ga)*2, colsize(ga)*2, depth(ga)*2, VFF_TYP_1_BYTE);
    if (out == NULL) {
      fprintf(stderr, "%s : malloc failed.\n", name); 
      return NULL;
    }
    if(!lga2khalimsky3d(ga, out, bar))
    {
      fprintf(stderr, "%s : lga2khalimsky3d failed.\n", name); 
      freeimage(out);
      return NULL;
    }
  }
  return out;
}

struct xvimage* labelfgd(struct xvimage *image, int32_t connex)
{
  static char *name="labelfgd";

  if (datatype(image) != VFF_TYP_1_BYTE) {
    fprintf(stderr, "%s: image should be of type VFF_TYP_1_BYTE.\n", name);
    return NULL;
  }
  
  struct xvimage *result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  if (! llabelfgd(image, connex, result))
  {
    fprintf(stderr, "%s: llabelfgd failed\n", name);
    freeimage(result);
    return NULL;
  }
  return result;
}

struct xvimage* genball(double radius, int32_t dim)
{
  static char *name="genball";

  struct xvimage *image = NULL;
  int32_t cen = (int32_t)ceil(radius);
  int32_t size = 2* cen + 1;

  if (dim == 3)
  {
    image = allocimage(NULL, size, size, size, VFF_TYP_1_BYTE);
    if (image == NULL) {
      fprintf(stderr, "%s: memory can not be allocated", name);
      return NULL;
    }
    ldrawball(image, radius, cen, cen, cen);
  }
  else if (dim == 2)
  {
    image = allocimage(NULL, size, size, 1, VFF_TYP_1_BYTE);
    if (image == NULL) {
      fprintf(stderr, "%s: memory can not be allocated", name);
      return NULL;
    }
    ldrawdisc(image, radius, cen, cen);
  }
  else
  {
    fprintf(stderr, "%s: dimension should be 2 or 3\n", name);
    return NULL;
  }
  return image;
}

struct xvimage* selectcomp(struct xvimage *imagein, int32_t x, int32_t y, int32_t z, int32_t connex)
{
  static char *name="selectcomp";

  struct xvimage *image = checkAllocCopy(imagein, name);
  if (image == NULL)
    return NULL;

  if (datatype(image) == VFF_TYP_1_BYTE) {
    if (! lselectcomp(image, connex, x, y, z))  {
      fprintf(stderr, "%s: function lselectcomp failed\n", name);
      freeimage(image);
      return NULL;
    }
  } else {
    fprintf(stderr, "%s: bad data type\n", name);
    freeimage(image);
    return NULL;
  }
  return image;
}

struct xvimage* long2byte(struct xvimage *imagelong, int32_t mode, int32_t nbnewval)
{
  static char *name="long2byte";
  
  struct xvimage*imagebyte = llong2byte(imagelong, mode, nbnewval);

  if (imagebyte == NULL)
    fprintf(stderr, "%s: llong2byte failed\n", name);

  return imagebyte;
}

struct xvimage* float2byte(struct xvimage *imagefloat, int32_t mode)
{
  static char *name="float2byte";
  
  struct xvimage*imagebyte = lfloat2byte(imagefloat, mode);

  if (imagebyte == NULL)
    fprintf(stderr, "%s: lfloat2byte failed\n", name);

  return imagebyte;
}
