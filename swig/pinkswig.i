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
    if (depth($self)==1)
      sprintf(tmp,"xvimage(%d,%d) [type = %s]", rowsize($self),colsize($self), type);
    else 
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

  void save(char *name) {
    writeimage($self, name);
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
      return None
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

def __add__(self,other):
     return add(self, other)

def __sub__(self,other):
     return sub(self, other)

def __div__(self,other):
     return divide(self, other)

def __mul__(self,other):
     return mult(self, other)

}};

struct xvimage * readimage(
  const char *filename
);

void writeimage(
  struct xvimage * image,
  const char *filename
);


struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);
struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2);
struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2);
struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2);
struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2);

%feature("docstring",
	 "Regional minima \n"
	 "Description: \n"
	 "Selects the regional minima of a grayscale image with connexity connex.\n"
	 "connex should be 0[min. absolus], or 4, 8 (in 2D) or 6, 18, 26 (in 3D) \n"
	 "Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n");
struct xvimage* minima(struct xvimage *image, int connex);

%feature("docstring",
	 "Regional maxima \n"
	 "Description: \n"
	 "Selects the regional maxima of a grayscale image with connexity connex.\n"
	 "connex should be 0[min. absolus], or 4, 8 (in 2D) or 6, 18, 26 (in 3D) \n"
	 "Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n");
struct xvimage* maxima(struct xvimage *image, int connex);

%feature("docstring",
	 "Topological grayscale watershed.\n"
	 "Description:\n"
	 "Topological watershed as defined in [B05,CNB05], preserves the minima with connectivity connex.\n"
	 "If inverse!=0, then a watershed divide is computed\n"
	 "If inverse==0, the dual operator is applied, i.e., thalweg lines are computed.\n"
	 "[B05] G. Bertrand: ""On topological watersheds"",\n" 
	 "                   Journal of Mathematical Imaging and Vision,\n"
	 "                   Vol. 22, No. 2-3, pp. 217-230, 2005\n"
	 "[CNB05] M. Couprie and L. Najman and G. Bertrand: ""Quasi-linear algorithms for the topological watershed"",\n"
	 "                   Journal of Mathematical Imaging and Vision,\n"
	 "                   Vol. 22, No. 2-3, pp. 231-249, 2005.\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse=1);

%feature("docstring",
	 "Topological binary watershed.\n"
	 "Description:\n"
	 "Topological watershed as defined in [B05,CNB05] with connectivity connex.\n"
	 "A marker image is supplied in mark. The result is a binary image.\n"
	 "If inverse!=0, then a watershed divide is computed\n"
	 "If inverse==0, the dual operator is applied, i.e., thalweg lines are computed.\n"
	 "[B05] G. Bertrand: ""On topological watersheds"",\n" 
	 "                   Journal of Mathematical Imaging and Vision,\n"
	 "                   Vol. 22, No. 2-3, pp. 217-230, 2005\n"
	 "[CNB05] M. Couprie and L. Najman and G. Bertrand: ""Quasi-linear algorithms for the topological watershed"",\n"
	 "                   Journal of Mathematical Imaging and Vision,\n"
	 "                   Vol. 22, No. 2-3, pp. 231-249, 2005.\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse=1);


%feature("docstring",
	 "Description:\n"
	 "Morphological dilation by a plane structuring element.\n"
	 "The (plane) structuring element is given by the non-null values in the image elem, \n"
	 "its origin (wrt the point (0,0) of elem) \n"
	 "is given by the parameters x, y and z. \n"
	 "If x==-1 (resp. y==-1, z==-1), then the center is in the middle of elem,\n"
	 "i.e., x=rowsize(elem)/2 (resp. y=colsize(elem)/2, z=depth(elem).\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

%feature("docstring",
	 "Description:\n"
	 "Morphological erosion by a plane structuring element.\n"
	 "The (plane) structuring element is given by the non-null values in the image elem, \n"
	 "its origin (wrt the point (0,0) of elem) \n"
	 "is given by the parameters x, y and z. \n"
	 "If x==-1 (resp. y==-1, z==-1), then the center is in the middle of elem,\n"
	 "i.e., x=rowsize(elem)/2 (resp. y=colsize(elem)/2, z=depth(elem).\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

%feature("docstring",
	 "Description:\n"
	 "Morphological opening by a plane structuring element.\n"
	 "The (plane) structuring element is given by the non-null values in the image elem, \n"
	 "its origin (wrt the point (0,0) of elem) \n"
 "is given by the parameters x, y and z. \n"
	 "If x==-1 (resp. y==-1, z==-1), then the center is in the middle of elem,\n"
	 "i.e., x=rowsize(elem)/2 (resp. y=colsize(elem)/2, z=depth(elem).\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

%feature("docstring",
	 "Description:\n"
	 "Morphological closing by a plane structuring element.\n"
	 "The (plane) structuring element is given by the non-null values in the image elem, \n"
	 "its origin (wrt the point (0,0) of elem) \n"
	 "is given by the parameters x, y and z. \n"
	 "If x==-1 (resp. y==-1, z==-1), then the center is in the middle of elem,\n"
	 "i.e., x=rowsize(elem)/2 (resp. y=colsize(elem)/2, z=depth(elem).\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

%feature("docstring",
	 "Geodesic (grayscale or binary) dilation\n"
	 "Description:\n"
	 "Geodesic dilation of image1 under image2.\n"
	 "Let G and F be the two input images. If G is not over F, then \n"
	 "G is replaced initially by min(G,F).\n"
	 "The structuring element is specified by the value of the parameter connex,\n"
	 "which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"
	 "The parameter niter sets the number of iterations. If niter == -1,\n"
	 "then the iterations continue until stability.\n"
	 "Types supported: byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d.\n");
struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter=-1);

%feature("docstring",
	 "Geodesic (grayscale or binary) erosion\n"
	 "Description:\n"
	 "Geodesic erosion of image1 over image2.\n"
	 "Let G and F be the two input images. If G is not over F, then \n"
	 "G is replaced initially by max(G,F).\n"
	 "The structuring element is specified by the value of the parameter connex,\n"
	 "which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"
	 "The parameter niter sets the number of iterations. If niter == -1,\n"
	 "then the iterations continue until stability.\n"
	 "Types supported: byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d.\n");
struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter=-1);

%feature("docstring",
	 "Simple threshold\n"
	 "<B>Description:</B>\n"
	 "If th2<=0, for each pixel x, out[x] = if (in[x] < th1) then 0 else 255\n"
	 "If th2>0, for each pixel x, out[x] = if (th1 <= in[x] < th2) then 255 else 0\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* threshold(struct xvimage *imagein, double th1, double th2=0.);

%feature("docstring",
	 "Morphological binary erosion by a metric ball.\n"
	 "This erosion is the dual of the dilatball operator,\n"
	 "i.e. erosball(X) = inverse(dilatball(inverse(X))).\n"
	 "The structuring element for this erosion is a ball (or a disc) of radius r.\n"
	 "The erosion is computed by thresholding a distance map.\n"
	 "The distance used depends on the optional parameter dist (default is 0) :\n"
	 "-  0: rounded euclidean distance\n"
	 "-  2: chamfer distance\n"
	 "-  3: exact quadratic euclidean distance\n"
	 "-  4: 4-distance in 2d\n"
	 "-  8: 8-distance in 2d\n"
	 "-  6: 6-distance in 3d\n"
	 "- 18: 18-distance in 3d\n"
	 "- 26: 26-distance in 3d\n"
	 "\n"
	 "Warning: The input image imagein must be a binary image. No test is done.\n");
struct xvimage* erosball(struct xvimage *imagein, int r, int dist=0);

%feature("docstring",
	 "Morphological binary dilation by a metric ball.\n"
	 "The structuring element for this erosion is a ball (or a disc) of radius r.\n"
	 "In this case the result of the dilation is defined by Y = {x; d(x,X) <= r} where X\n"
	 "represents the original point set (non-null pixels in the original image).\n"
	 "The dilation is computed by thresholding a distance map.\n"
	 "The distance used depends on the optional parameter dist (default is 0) :\n"
	 "-  0: rounded euclidean distance\n"
	 "-  2: chamfer distance\n"
	 "-  3: exact quadratic euclidean distance\n"
	 "-  4: 4-distance in 2d\n"
	 "-  8: 8-distance in 2d\n"
	 "-  6: 6-distance in 3d\n"
	 "- 18: 18-distance in 3d\n"
	 "- 26: 26-distance in 3d\n"
	 "\n"
	 "Warning: The input image imagein must be a binary image. No test is done.\n");
struct xvimage* dilatball(struct xvimage *imagein, int r, int dist=0);

%feature("docstring",
	 "Generates an image with a white border and a black interior\n\n"
	 "Description:\n"
	 "The resulting image has the same size as the input image. Its border is set to 255,\n"
	 "all other pixels are set to \n"
	 "If the optional parameter width is given, then the border has thickness 'width'.\n"
         "Types supported: byte 2d, byte 3d\n");
struct xvimage* frame(struct xvimage *imagein, int width=1);

%feature("docstring",
	 "Computes the pixelwise inverse of an image\n\n"
	 "Description:\n"
	 "Byte images: for each pixel x, out[x] = 255 - in[x].<br>\n"
	 "Long or float images: for each pixel x, out[x] = VMAX - in[x], where VMAX = max{in[x]}.\n"
	 "Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n");
struct xvimage* inverse(struct xvimage *imagein);

%feature("docstring",
	 "Minimum of 2 images\n"
	 "Description:\n"
	 "For each pixel x, out[x] = min{in1[x], in2[x]}. \n"
	 "Images must be of the same type and same dimensions.\n"
	 "Otherwise, a conversion to the more general type is performed.\n"
	 "Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n");
struct xvimage* min(struct xvimage *imagein1, struct xvimage *imagein2);

%feature("docstring",
	 "Maximum of 2 images\n"
	 "Description:\n"
	 "For each pixel x, out[x] = max{in1[x], in2[x]}. \n"
	 "Images must be of the same type and same dimensions.\n"
	 "Otherwise, a conversion to the more general type is performed.\n"
	 "Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n");
struct xvimage* max(struct xvimage *imagein1, struct xvimage *imagein2);


%feature("docstring",
	 "border of a binary image\n"
	 "Description:\n"
	 "Let X be the subset of Zn that corresponds to the input image, and \n"
	 "let k be the value of the parameter connex (that is, 4, 8 (2d) or 6, 26 (3d)),\n"
	 "which is the connectivity chosen for X.\n"
	 "\n"
	 "The border B(X) of X is defined by:\n"
	 "B(X) = {x in X, (Gamma_nk(x) inter Xbar) not empty}\n"
	 "where nk is the complementary connectivity for k.\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* border(struct xvimage *imagein, int connex=4);

%feature("docstring",
	 "h-maxima operator\n"
	 "Description:\n"
	 "h-maxima with connexity connex and depth ""depth"".\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* heightmaxima(struct xvimage *imagein, int32_t depth, int32_t connex=4);

%feature("docstring",
	 "h-minima operator\n"
	 "Description:\n"
	 "h-minima with connexity connex and depth ""depth"".\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* heightminima(struct xvimage *imagein, int32_t depth, int32_t connex=4);

%feature("docstring",
	 "Area opening operator\n"
	 "Description:\n"
	 "Area opening with connexity connex and parameter area+1.\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* areaopening(struct xvimage *imagein, int32_t area, int32_t connex=4);

%feature("docstring",
	 "Area closing operator\n"
	 "Description:\n"
	 "Area closing with connexity connex and parameter area+1.\n"
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* areaclosing(struct xvimage *imagein, int32_t area, int32_t connex=4);

%feature("docstring",
	 "Maxima-Volume-based filtering\n"
	 "Description:\n"
	 "Maxima-Volume-based filtering with connexity connex and volume vol."
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* volmaxima(struct xvimage *imagein, int32_t vol, int32_t connex=4);

%feature("docstring",
	 "Minima-Volume-based filtering\n"
	 "Description:\n"
	 "Minima-Volume-based filtering with connexity connex and volume vol."
	 "Types supported: byte 2d, byte 3d\n");
struct xvimage* volminima(struct xvimage *imagein, int32_t vol, int32_t connex=4);
