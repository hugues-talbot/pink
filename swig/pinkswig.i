/* File pinkswig.i
 author: Laurent Najman
 date: 12/01/2016
*/

%module(docstring="Interface to python of the Pink library") pinkswig

%{
#define SWIG_FILE_WITH_INIT
#include "com/pinkswig.h"
#include <mccodimage.h>
#include <mcimage.h>

%}

%feature("autodoc", "1");

  typedef unsigned char u_int8_t;
  typedef unsigned int  u_int32_t;
  typedef unsigned char uint8_t;
  typedef int           int32_t;
  typedef unsigned int  uint32_t;
  typedef long long     int64_t;
  typedef unsigned long long uint64_t;
  typedef unsigned long long u_int64_t;

typedef int32_t index_t;

#define	VFF_TYP_BIT		0	/* pixels are on or off (binary image)*/
                                        /* Note: This is an X11 XBitmap 
					   with bits packed into a byte and
					   padded to a byte */
#define	VFF_TYP_1_BYTE		1	/* pixels are byte (uint8_t) */
#define	VFF_TYP_2_BYTE		2	/* pixels are two byte (int16_t) */
#define	VFF_TYP_4_BYTE		4	/* pixels are four byte (int32_t) */
#define	VFF_TYP_FLOAT		5	/* pixels are float (single precision)*/
#define VFF_TYP_DOUBLE		6	/* pixels are float (double precision)*/
#define VFF_TYP_COMPLEX		7	/* pixels are complex (single precision)*/
#define VFF_TYP_DCOMPLEX	8	/* pixels are complex (double precision)*/

/**
   \brief The image class for the C functions.

   This class holds the image data for the C functions of Pink.
*/ 
typedef struct xvimage {
    /** \brief Size of a row (number of columns) */
    index_t row_size;                    

    /** \brief Size of a column (number of rows) */
    index_t col_size;

    /** \brief Number of planes (for 3d images) */
    index_t depth_size;                  

    /** \brief Number of (2d or 3d) images */
    index_t time_size;

    void * image_data;

%extend {
  char *__str__() {
    static char tmp[1024];
    static char type[100];
    switch (datatype($self)) {
    case VFF_TYP_1_BYTE: sprintf(type, "uint8_t"); break;
    case VFF_TYP_2_BYTE: sprintf(type, "int16_t"); break;
    case VFF_TYP_4_BYTE: sprintf(type, "int32_t"); break;
    case VFF_TYP_FLOAT: sprintf(type, "float"); break;
    case VFF_TYP_DOUBLE: sprintf(type, "double"); break;
    default: sprintf(type,"other");
    }
    sprintf(tmp,"xvimage(%d,%d,%d) [type = %s]", rowsize($self),colsize($self),depth($self), type);
    return tmp;
  }

  char *getctype() {
    static char type[100];
    switch(datatype($self)) {
    case VFF_TYP_1_BYTE: sprintf(type, "c_uint8"); break;
    case VFF_TYP_2_BYTE: sprintf(type, "c_uint16"); break;
    case VFF_TYP_4_BYTE: sprintf(type, "c_uint32"); break;
    case VFF_TYP_FLOAT: sprintf(type, "c_float"); break;
    case VFF_TYP_DOUBLE: sprintf(type, "c_double"); break;
    default: sprintf(type, "not implemented");
    }
    return type;
  }


  xvimage(index_t x, index_t y, index_t z=1, int32_t type=VFF_TYP_1_BYTE) {
    return createimage(x,y,z,type, 0);
  }
  
  xvimage(char *name) {
    xvimage *im;
    im = readimage(name);
    if (im == NULL) {
      fprintf(stderr, "%s: readimage failed\n", "xvimage");
      return NULL;
    }
    return im;  
  }

  ~xvimage() {
    freeimage($self);
  }

/*
  uint8_t getpixel(index_t x, index_t y, index_t z=0) {
    return voxel($self,x,y,z);
  }
  void setpixel(int8_t value, index_t x, index_t y, index_t z=0) {
    ((uint8_t*)(($self)->image_data))[((z)*colsize($self)+(y))*rowsize($self)+(x)] = value;
  }
*/
}} xvimage;

%pythoncode{
import ctypes
def fromnumpy(mat):
    if len(mat.shape) == 2:
      row, col = mat.shape
      depth = 1
    else: 
      row, col, depth = mat.shape
    type = mat.dtype.name
    translate = {'uint8' : 1, 'uint16' : 2, 'uint32' : 4, 'float32' : 5, 'float64' : 6}
    if type in translate:
      return createimage(row, col, depth, translate[type], mat.ctypes.data)
    else:
      print "Type:", type, "is not supported"
      return
}

%extend xvimage {
%pythoncode{

import ctypes
	      

    
def getdata(self):
    if self.depth_size == 1: 
      data = getattr(self.ctypes,self.getctype()) *self.row_size*self.col_size
    else:
      data = getattr(self.ctypes,self.getctype()) *self.row_size*self.col_size*self.depth_size
    data = data.from_address(int(self.image_data))
    return data

def getpixel(self, x, y, z=0):
    data = self.getdata()
    if self.depth_size == 1:
      return data[x][y]
    else:
      return data[x][y][z]

def setpixel(self, value, x, y, z=0):
    data = self.getdata()
    if self.depth_size == 1:
      data[x][y] = value
    else:
      data[x][y][z] = value
	
def __getitem__(self, tup):
    if len(tup) == 2:
      x, y = tup
      return self.getpixel(x,y)
    else:
      x, y, z = tup
    return self.getpixel(x,y,z)
	
def __setitem__(self, tup, value):
    if len(tup)==2:
      x, y = tup
      self.setpixel(value,x,y)
    else:
      x, y, z = tup
      self.setpixel(value,x,y,z)

}};

struct xvimage * readimage(
  const char *filename
);

void writeimage(
  struct xvimage * image,
  const char *filename
);


struct xvimage* minima(struct xvimage *image, int connex);
struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse=1);
struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse=1);
struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);
struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);
struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);
struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);
struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter=-1);
struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter=-1);
struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);
struct xvimage* threshold(struct xvimage *imagein, double seuil, double seuil2=0.);
struct xvimage* erosball(struct xvimage *imagein, int r, int mode=0);
struct xvimage* dilatball(struct xvimage *imagein, int r, int mode=0);
struct xvimage* frame(struct xvimage *imagein, int width=1);
struct xvimage* inverse(struct xvimage *imagein);
struct xvimage* min(struct xvimage *imagein1, struct xvimage *imagein2);
struct xvimage* max(struct xvimage *imagein1, struct xvimage *imagein2);
struct xvimage* border(struct xvimage *imagein, int connex=4);
