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
#include <jccodimage.h>
#include <jcimage.h>

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

#define VFF_TYP_GABYTE          12      /* graphe d'arete code sur 1 octet */
#define VFF_TYP_GAFLOAT         13      /* graphe d'arete code en float */
#define VFF_TYP_GADOUBLE        14      /* graphe d'arete code en double */



/**
   \brief The image class for the C functions.

   This class holds the image data for the C functions of Pink.
*/ 
typedef struct xvimage {

%extend {
  int colsize() {
    return colsize($self);
  }
  int rowsize() {
    return rowsize($self);
  }
  int depth() {
    return depth($self);
  }
  int datatype() {
    return datatype($self);
  }
  void *__imagedata__() {
    return $self->image_data;
  }
  
  void name(const char*name)
  {
    if ($self->name != NULL)
      free($self->name);
    $self->name = (char *)calloc(1,strlen(name)+1);
    if ($self->name == NULL){
      fprintf(stderr,"%s: malloc failed for name\n", "xvimage");
      return;
    }
    sprintf((char *)($self->name), name);
  }
  const char* name()
  {
    if ($self->name == NULL)
      return "";
    else
      return $self->name;
  }
  
  char *__str__() {
    static char tmp[1024];
    static char type[100];
    switch (datatype($self)) {
    case VFF_TYP_1_BYTE: sprintf(type, "uint8_t"); break;
    case VFF_TYP_2_BYTE: sprintf(type, "int16_t"); break;
    case VFF_TYP_4_BYTE: sprintf(type, "int32_t"); break;
    case VFF_TYP_FLOAT: sprintf(type, "float"); break;
    case VFF_TYP_DOUBLE: sprintf(type, "double"); break;
    case VFF_TYP_GABYTE: sprintf(type, "uint8_t Edge Graph"); break;
    case VFF_TYP_GAFLOAT: sprintf(type, "float Edge Graph"); break;
    case VFF_TYP_GADOUBLE: sprintf(type, "double Edge Graph"); break;
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


  xvimage(index_t x, index_t y, index_t z=1, int32_t color=0, int32_t type=VFF_TYP_1_BYTE) {
    struct xvimage *image=allocimage(NULL, x,y , z, type);
    if (image == NULL) {
      fprintf(stderr, "%s: cannot allocate image\n", "xvimage");
      return NULL;
    }
    int32_t N = x * y * z;
    for (int i=0; i<N; i++)
      switch(datatype(image)) {
      case VFF_TYP_1_BYTE: UCHARDATA(image)[i] = (int8_t)color;
	break;
      case VFF_TYP_4_BYTE: ULONGDATA(image)[i] = (int32_t)color;
	break;
      case VFF_TYP_FLOAT: FLOATDATA(image)[i] = (float)color;
	break;
      case VFF_TYP_DOUBLE: DOUBLEDATA(image)[i] = (double)color;
	break;
      default:
	fprintf(stderr, "Setting color for this type not implemented.\n");
	fprintf(stderr, "It will be random colors.\n");
	return image;
      }
    return image;
  }
  
  xvimage(char *name) {
    xvimage *im;
    im = readimage(name);
    if (im == NULL) {
      im = readGAimage(name);
      if (im == NULL) {
	fprintf(stderr, "%s: readimage failed\n", "xvimage");
	return NULL;
      }
    }
    return im;  
  }

  ~xvimage() {
    freeimage($self);
  }

  int save(char *name) {
    int ret;
    if (datatype($self)>=VFF_TYP_GABYTE) {
      ret = writerawGAimage($self, name);
      if (ret == 0)
	fprintf(stderr, "%s: Error writing edge_weigthed image %s\n", "xvimage::save", name);
    } else {
      ret = writeimage($self, name);
      if (ret == 0)
	fprintf(stderr, "%s: Error writing image %s\n", "xvimage::save", name);
    }
    return ret;
  }

}} xvimage;

%pythoncode{
import ctypes

def fromnumpy(mat):
    if mat.flags['C_CONTIGUOUS'] == False:
      print "fromnumpy: Memory is not C contiguous."
      print "fromnumpy: Please use ""numpy.ascontiguousarray""."
      print "fromnumpy: and do not forget to keep a reference"
      print "fromnumpy: to the Numpy array thus created."
      raise NameError("Memory not C-contiguous")
    if len(mat.shape) == 2:
      row, col = mat.shape
      depth = 1
    else: 
      row, col, depth = mat.shape
    type = mat.dtype.name
    translate = {'uint8' : 1, 'uint16' : 2, 'uint32' : 4, 'float32' : 5, 'float64' : 6}
    if type in translate:
      im = __createimage__(row, col, depth, translate[type], mat.ctypes.data)
      return im
    else:
      print "Type:", type, "is not supported"
      raise NameError("Type not supported")


def surimp(imgrey, mask):
    """Combines a greyscale image with a binary mask.
       Returns a numpy color image, with the mask in red
       over the original greyscale image."""
    im = imgrey.tonumpy()
    w, h = im.shape
    ret = np.empty((w, h, 3), dtype=np.uint8)
    ret[:, :, 2] =  ret[:, :, 1] =  ret[:, :, 0] =  im
    red = mask.tonumpy()[:] > 0
    ret[red]= [255,0,0]
    return ret
}

%extend xvimage {
%pythoncode{

@property
def shape(self):
    if self.depth() == 1:
      return self.colsize(), self.rowsize()
    else:
      return self.colsize(), self.rowsize(), self.depth()

import ctypes
	      
def getdata(self):
    if self.depth() == 1: 
      data = getattr(self.ctypes,self.getctype()) *self.rowsize()*self.colsize()
    else:
      data = getattr(self.ctypes,self.getctype()) *self.rowsize()*self.colsize()*self.depth()
    data = data.from_address(int(self.__imagedata__()))
    return data

def tonumpy(self):
      import numpy as np
      return np.ctypeslib.as_array(self.getdata())

def getpixel(self, x, y, z=0):
    data = self.getdata()
    if self.depth() == 1:
      return data[x][y]
    else:
      return data[x][y][z]

def setpixel(self, value, x, y, z=0):
    data = self.getdata()
    if self.depth() == 1:
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

def __le__(self,other):
     return inf(self,other)
def __ge__(self,other):
     return sup(self,other)
	
def __add__(self,other):
     return add(self, other)

def __radd__(self,other):
     return add(self, other)

def __sub__(self,other):
     return sub(self, other)

def __div__(self,other):
     return divide(self, other)

def __mul__(self,other):
     return mult(self, other)

def __rmul__(self,other):
     return mult(self, other)

def __xor__(self,other):
     return xor(self, other)

def __or__(self,other):
     return max(self, other)

def __and__(self,other):
     return min(self, other)

def __invert__(self):
     return invert(self)

}};

%rename(__readimage__) readimage;
%rename(__writeimage__) writeimage;
%rename(__readGAimage__) readGAimage;
%rename(__writeGAimage__) writerawGAimage;
%newobject readimage;
struct xvimage * readimage(const char *filename);
int writeimage(struct xvimage * image,  const char *filename);
%newobject readGAimage;
struct xvimage * readGAimage(char *filename);
int writerawGAimage(struct xvimage * image,  char *filename);
%pythoncode{
  def readimage(name):
    ret = __readimage__(name)
    if ret == None:
      ret = __readGAimage__(name)
    if ret != None:
      from os.path import basename
      ret.name(str.split(basename(name),'.')[0])	
    return ret

  def writeimage(im, name):
    if im.datatype()<VFF_TYP_GABYTE:
      ret = __writeimage__(im,name)
    else:
      ret = __writeGAimage__(im,name)
    return ret	
}

%rename(__createimage__) createimage;
%newobject createimage;
struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);

%rename(copy) copyimage;
%newobject copyimage;
struct xvimage *copyimage(struct xvimage *f);

%rename(add) addconst;
%rename(sub) subconst;
%rename(mult) multconst;
%rename(divide) divideconst;

%feature("docstring",
	 "Pixelwise addition of two images.\n"
	 "Description:\n"
	 "For each pixel x, out[x] = in1[x] + in2[x].\n"
	 "If out[x] exceeds the maximum possible value for the pixel type,\n"
	 "then out[x] is to this maximum value.\n"
	 "Images must be of the same dimensions.\n"
	 "The second image can be a constant number.\n"
	 "In that case, it is as if adding image of constant value.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
%newobject add;
struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject addconst;
struct xvimage* addconst(struct xvimage *imagein1, double const);
%feature("docstring",
	 "Pixelwise substraction of two images\n"
	 "Description:\n"
	 "For each pixel x, out[x] = in1[x] - in2[x].\n"
	 "For byte and int32_t image types, if out[x] < 0, then out[x] is set to 0.\n"
	 "Images must be of the same dimensions.\n"
	 "The second image can be a constant number.\n"
	 "In that case, it is as if substracting an image of constant value.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
%newobject sub;
struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject subconst;
struct xvimage* subconst(struct xvimage *imagein1, double constante);
%feature("docstring",
	 "Pixelwise multiplication of two images\n"
	 "Description:\n"
	 "For each pixel x, out[x] = in1[x] * in2[x]. If both images are byte images,\n"
	 "and if out[x] exceeds 255, then out[x] is set to 255.\n"
	 "Images must be of the same dimensions.\n"
	 "The second image can be a constant number.\n"
	 "In that case, it is as if multiplying by an image of constant value.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
%newobject mult;
struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject multconst;
struct xvimage* multconst(struct xvimage *imagein1, double constante);
%feature("docstring",
	 "Pixelwise division of two images\n"
	 "Description:\n"
	 "For each pixel x such that in2[x] != 0, out[x] = in1[x] / in2[x]. \n"
	 "For each pixel x such that in2[x] = 0, out[x] = 0. \n"
	 "Images must be of the same type and same dimensions.\n"
	 "The second image can be a constant number.\n"
	 "In that case, it is as if dividing by an image of constant value.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d)\n");
%newobject divide;
struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject divideconst;
struct xvimage* divideconst(struct xvimage *imagein1, double constante);
%feature("docstring",
	 "pixelwise xor of two images\n"
	 "Description:\n"
	 "For each pixel x, if out[x] = in1[x] xor in2[x].\n"
	 "Images must be of the same type and same dimensions.\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject xor;
struct xvimage* xor(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject inf;
%feature("docstring",
	 "pixelwise inf predicate\n"
	 "Description:\n"
	 "For each pixel x, if in1[x] <= in2[x] then out[x] = 255 else out[x] = 0.\n"
	 "Images must be of the same type and same dimensions.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* inf(struct xvimage *imagein1, struct xvimage *imagein2);
%newobject sup;
%feature("docstring",
	 "pixelwise sup predicate\n"
	 "Description:\n"
	 "For each pixel x, if in1[x] >= in2[x] then out[x] = 255 else out[x] = 0.\n"
	 "Images must be of the same type and same dimensions.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
struct xvimage* sup(struct xvimage *imagein1, struct xvimage *imagein2);

%feature("docstring",
	 "absolute value of an image.\n"
	 "Description:\n"
	 "For each pixel x, out[x] = abs(in[x]).\n"
	 "Types supported: float 2d, float 3d\n");
%rename(abs) absimg;
%newobject absimg;
struct xvimage* absimg(struct xvimage *imagefloat);
		
%feature("docstring",
	 "Normalization of grayscale values.\n"
	 "Description: \n"
	 "Grayscale values of the input image are normalized to span the range of [nmin...nmax].\n"
	 "The parameters ""nmin"" and ""nmax"" are optional.\n"
	 "The default values are nmin == 0 and nmax == 255.\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
%newobject normalize;
struct xvimage* normalize(struct xvimage *imagein, double nmin=0, double nmax=255);

%feature("docstring",
	 "Regional minima \n"
	 "Description: \n"
	 "Selects the regional minima of a grayscale image with connexity connex.\n"
	 "connex should be 0[min. absolus], or 4, 8 (in 2D) or 6, 18, 26 (in 3D) \n"
	 "Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n");
%newobject minima;
struct xvimage* minima(struct xvimage *image, int connex);

%feature("docstring",
	 "Regional maxima \n"
	 "Description: \n"
	 "Selects the regional maxima of a grayscale image with connexity connex.\n"
	 "connex should be 0[min. absolus], or 4, 8 (in 2D) or 6, 18, 26 (in 3D) \n"
	 "Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n");
%newobject maxima;
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
%newobject wshedtopo;
struct xvimage* wshedtopo(struct xvimage *image, int connex=8, int inverse=1);

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
%newobject watershed;
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
%newobject dilation;
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
%newobject erosion;
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
%newobject opening;
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
%newobject closing;
struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

%feature("docstring",
	 "Rank filter\n"
	 "Description:\n"
	 "Let F be the input image, G be the output image, and E the structuring\n"
	 "element. Let n be the number of elements of E, and R be the product n.r,\n"
	 "then for each pixel p, G[p] is the Rth element of the sorted list (by \n"
	 "increasing order) of the pixel values in the set { F[q], q in E[p] }.\n"
	 "\n"
	 "Particular cases are the median filter (r = 0.5), the erosion (r = 0),\n"
	 "and the dilation (r = 1).\n"
	 "The origin of the structuring element is given by the parameters x, y and z. \n"
	 "If x==-1 (resp. y==-1, z==-1), then the center is in the middle of elem,\n"
	 "i.e., x=rowsize(elem)/2 (resp. y=colsize(elem)/2, z=depth(elem).\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject rankfilter;
struct xvimage* rankfilter(struct xvimage *image, float r, struct xvimage *elem, int32_t x=-1, int32_t y=-1, int32_t z=-1);

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
%newobject geodilat;
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
%newobject geoeros;
struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter=-1);

%feature("docstring",
	 "Simple threshold\n"
	 "Description:\n"
	 "If th2<=th1, for each pixel x, out[x] = if (in[x] < th1) then 0 else 255\n"
	 "If th2>th1, for each pixel x, out[x] = if (th1 <= in[x] < th2) then 255 else 0\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n");
%newobject threshold;
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
%newobject erosball;
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
%newobject dilatball;
struct xvimage* dilatball(struct xvimage *imagein, int r, int dist=0);

%feature("docstring",
         "Description:\n"
         "Alternate sequential filter for binary images. \n"
         "\n"
         "Disc-shaped structuring elements of increasing radius,\n"
         "ranging from 1 (or ""radiusmin"" if this parameter is specified) to ""radiusmax"",\n"
         "are generated. \n"
         "\n"
         "Let D_1,...,D_n be these structuring elements, sorted by increasing radius.\n"
         "Let F_0 = imagein, the ith intermediate result F_i is obtained by the closing of\n"
         "the opening of F_{i-1} by the structuring element D_i. \n"
         "The resulting image contains the final result F_n.\n"
         "Giving a value 0 for the optional parameter radiusmin has the effect of beginning \n"
         "by a closing instead of an opening.\n"
         "\n"
         "Types supported: byte 2d, byte 3d\n");
%newobject asfbin;
struct xvimage* asfbin(struct xvimage *imagein, int32_t radiusmax, int32_t radiusmin=1);

%feature("docstring",
	 "Alternate sequential filter\n"
	 "Description:\n"
	 "Alternate sequential filter for grayscale images (for binary images\n"
	 "use preferably asfbin).\n"
	 "Disc-shaped structuring elements of increasing radius, ranging from 1\n"
	 "(or ""radiusmin"" if this parameter is specified) to ""radiusmax"", are\n"
	 "generated.\n"
	 "\n"
	 "Let D1...Dn be these structuring elements, sorted by increasing radius.\n"
	 "\n"
	 "Let I0 be the input image, the ith intermediate result Ii is obtained by\n"
	 "the closing of the opening of Ii-1 by the structuring element Di.\n"
	 "\n"
	 "The function returns the final result In.\n"
	 "\n"
	 "Giving a value 0 for the optional parameter <B>rmin</B> has the effect\n"
	 "of beginning by a closing instead of an opening.\n"
	 "\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject asf;
struct xvimage* asf(struct xvimage *imagein, int32_t radiusmax, int32_t radiusmin=1);


%feature("docstring",
	 "Generates an image with a white border and a black interior\n\n"
	 "Description:\n"
	 "The resulting image has the same size as the input image. Its border is set to 255,\n"
	 "all other pixels are set to \n"
	 "If the optional parameter width is given, then the border has thickness 'width'.\n"
         "Types supported: byte 2d, byte 3d\n");
%newobject frame;
struct xvimage* frame(struct xvimage *imagein, int width=1);

%feature("docstring",
	 "Generates a binary euclidean ball\n"
	 "\n"
	 "Description:\n"
	 "Draws a binary euclidean ball in the resulting image.\n"
	 "The parameter radius specifies the radius of the ball. \n"
	 "The parameter ""dim"" specifies the dimension: 2 (default) or 3.\n"
	 "The center of the ball is designated as the origin (for use as structuring element).\n"
         "Types supported: byte 2d, byte 3d\n");
%newobject genball;
struct xvimage* genball(double radius, int32_t dim=2);

%feature("docstring",
	 "Computes the pixelwise inverse of an image\n\n"
	 "Description:\n"
	 "Byte images: for each pixel x, out[x] = 255 - in[x].<br>\n"
	 "Long or float images: for each pixel x, out[x] = VMAX - in[x], where VMAX = max{in[x]}.\n"
	 "Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n");
%newobject invert;
struct xvimage* invert(struct xvimage *imagein);

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
%newobject max;
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
%newobject border;
struct xvimage* border(struct xvimage *imagein, int connex=4);

%feature("docstring",
	 "h-maxima operator\n"
	 "Description:\n"
	 "h-maxima with connexity connex and depth ""depth"".\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject heightmaxima;
struct xvimage* heightmaxima(struct xvimage *imagein, int32_t depth, int32_t connex=4);

%feature("docstring",
	 "h-minima operator\n"
	 "Description:\n"
	 "h-minima with connexity connex and depth ""depth"".\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject heightminima;
struct xvimage* heightminima(struct xvimage *imagein, int32_t depth, int32_t connex=4);

%feature("docstring",
	 "Area opening operator\n"
	 "Description:\n"
	 "Area opening with connexity connex and parameter area+1.\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject areaopening;
struct xvimage* areaopening(struct xvimage *imagein, int32_t area, int32_t connex=4);

%feature("docstring",
	 "Area closing operator\n"
	 "Description:\n"
	 "Area closing with connexity connex and parameter area+1.\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject areaclosing;
struct xvimage* areaclosing(struct xvimage *imagein, int32_t area, int32_t connex=4);

%feature("docstring",
	 "Maxima-Volume-based filtering\n"
	 "Description:\n"
	 "Maxima-Volume-based filtering with connexity connex and volume vol."
	 "Types supported: byte 2d, byte 3d\n");
%newobject volmaxima;
struct xvimage* volmaxima(struct xvimage *imagein, int32_t vol, int32_t connex=4);


%feature("docstring",
	 "Minima-Volume-based filtering\n"
	 "Description:\n"
	 "Minima-Volume-based filtering with connexity connex and volume vol."
	 "Types supported: byte 2d, byte 3d\n");
%newobject volminima;
struct xvimage* volminima(struct xvimage *imagein, int32_t vol, int32_t connex=4);

%feature("docstring",
         "distance transform \n"
         "Description:\n"
         "Distance to the object X defined by the binary image ""image"".\n"
         "The result function DX(x) is defined by: DX(x) = min {d(x,y), y in X}.\n"
         "\n"
         "The distance d used depends on the parameter ""mode"":\n"
         "-   0: euclidean distance (rounded to the nearest int32)\n"
         "-   1: approximate quadratic euclidean distance (Danielsson)\n"
         "-   2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)\n"
         "-   3: exact quadratic euclidean distance (int32)\n"
         "-   4: 4-distance in 2d\n"
         "-   5: exact euclidean distance (float)\n"
         "-   8: 8-distance in 2d\n"
         "-   6: 6-distance in 3d\n"
         "-  18: 18-distance in 3d\n"
         "-  26: 26-distance in 3d\n"
         "-  40: 4-distance in 2d (byte coded ouput)\n"
         "-  80: 8-distance in 2d (byte coded ouput)\n"
         "-  60: 6-distance in 3d (byte coded ouput)\n"
         "- 180: 18-distance in 3d (byte coded ouput)\n"
         "- 260: 26-distance in 3d (byte coded ouput)\n"
         "\n"
         "The output is of type int32_t for modes < 40, of type float for mode==5,of type byte for other modes.\n"
         "\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject dist;
struct xvimage* dist(struct xvimage *image, int32_t mode);

%rename(skeleton) skeletonprio2;
%rename(skeleton) skeletondist2;
%rename(skeleton) skeletonprio1;
%rename(skeleton) skeletondist1;

%feature("docstring",
         "ultimate binary skeleton guided by a priority image (see [BC07])\n"
         "Description:\n"
         "Ultimate binary skeleton guided by a priority image ""prio"".\n"
         "If a number ""mode"" is provided instead of an image ""prio,""\n"
         "then the priority image is a distance map.\n"
         "\n"
         "The distance map computed depends on the parameter ""mode"":\n"
         "-   0: euclidean distance (rounded to the nearest int32)\n"
         "-   1: approximate quadratic euclidean distance (Danielsson)\n"
         "-   2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)\n"
         "-   3: exact quadratic euclidean distance (int32)\n"
         "-   4: 4-distance in 2d\n"
         "-   5: exact euclidean distance (float)\n"
         "-   8: 8-distance in 2d\n"
         "-   6: 6-distance in 3d\n"
         "-  18: 18-distance in 3d\n"
         "-  26: 26-distance in 3d\n"
         "-  40: 4-distance in 2d (byte coded ouput)\n"
         "-  80: 8-distance in 2d (byte coded ouput)\n"
         "-  60: 6-distance in 3d (byte coded ouput)\n"
         "- 180: 18-distance in 3d (byte coded ouput)\n"
         "- 260: 26-distance in 3d (byte coded ouput)\n"
         "\n"
         "The parameter connex indicates the connectivity of the binary object.\n"
         "Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"
         "If a binary image inhibitimage is provided and non null,\n"
         "then the points of this image will be left unchanged. \n"
         "If an integer parameter ""inhibit"" is given,\n"
         "then the points which correspond to this priority value will be left unchanged. \n"
         "\n"
         "Let X be the set corresponding to the input image imagein.\n"
         "Let P be the function corresponding to the priority image.\n"
         "Let I be the set corresponding to the inhibit image inhibimage, if given, or the empty\n"
         "set otherwise.\n"
         "The algorithm is the following:\n"
         "\n"
         "Repeat until stability\n"
         "  Select a point x in X \ I such that P[x] is minimal\n"
         "  If x is simple for X then\n"
         "    X = X \ {x}\n"
         "Result: X\n"
         "\n"
         "Reference: \n"
         "[BC07] G. Bertrand and M. Couprie: \n"
         "Transformations topologiques discretes.\n"
         "in Geometrie discrete et images numeriques\n"
         "D. Coeurjolly and A. Montanvert and J.M. Chassery, \n"
         "pp. 187-209, Hermes, 2007.\n"
         "http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf\n"
         "\n"
	 "Types supported: byte 2d, byte 3d\n");

%newobject skeletonprio2;
struct xvimage* skeletonprio2(struct xvimage *imagein, struct xvimage *prio, int32_t connex, struct xvimage *inhibimage=NULL);

%newobject skeletondist2;
struct xvimage* skeletondist2(struct xvimage *imagein, int32_t mode, int32_t connex, struct xvimage *inhibimage);

%newobject skeletonprio1;
struct xvimage* skeletonprio1(struct xvimage *imagein, struct xvimage *prio, int32_t connex, int32_t inhibit);

%newobject skeletondist1;
struct xvimage* skeletondist1(struct xvimage *imagein, int32_t mode, int32_t connex, int32_t inhibit);


%feature("docstring",
         "Detects isolated points in a binary image\n"
         "Description:\n"
         "An isolated point is a white point, all the n-neighbours of which are black\n"
         "(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter \b connex)\n"
         "Types supported: byte 2d, byte 3d\n");
%newobject ptisolated;
struct xvimage* ptisolated(struct xvimage *imagein, int32_t connex);

%feature("docstring",
         "Detects junction points in 2d or 3d binary images\n"
         "\n"
         "Description:\n"
         "Detects junction points in the 2d or 3d binary image imagein, which is supposed to contain a skeleton.\n"
         "A junction point is a white point x such that\n #(Nn[x] inter X) > 2,\n"
         "where Nn[x] stands for the n-neighborhood of x (excluding x),\n"
         "and n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter ""connex"".\n"
         "\n"
         "When the type of imagein is 4_BYTE, the image is treated as a label image,\n"
         "where each label is processed as a separate binary image\n"
         "(all other labels are considered as background).\n"
         "\n"
         "Types supported: byte 2D, byte 3D, long 3D\n");
%newobject ptjunction;
struct xvimage* ptjunction(struct xvimage *imagein, int32_t connex);

%feature("docstring",
         "Detects curve points in a binary image\n"
         "Description:\n"
         "An curve point is a white point, which has exactly 2 white n-neighbours \n"
         "that are not n-neighbours to each other. In other words, Tn = 2 and \n"
         "each neighboring n-connected component is made of exactly one point\n"
         "(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter ""connex"").\n"
         "\n"
         "Types supported: byte 2d, byte 3d\n");
%newobject ptcurve;
struct xvimage* ptcurve(struct xvimage *imagein, int32_t connex);

%feature("docstring",
         "Detects end points in a binary image\n"
         "Description:\n"
         "An end point is a white point, which has exactly 1 white n-neighbour\n"
         "(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter ""connex"")\n"
         "\n"
         "When the type of imagein is 4_BYTE, the image is treated as a label image,\n"
         "where each label is processed as a separate binary image\n"
         "(all other labels are considered as background).\n"
         "\n"
         "Types supported: byte 2D, byte 3D, long 3D\n");
//%newobject createimage;
%newobject ptend;
struct xvimage* ptend(struct xvimage *imagein, int32_t connex);


%rename(computeEdgeGraph) ComputeEdgeGraphGrey;
%rename(computeEdgeGraph) ComputeEdgeGraphColor;
		  
%feature("docstring",
	 "Computes a GA (an edge-weighted graph) from an image. The values of an\n"
	 "edge linking two pixels {x,y} is computed according to the parameter\n"
	 "\n"
	 "If the input image is greyscale, then\n"
	 "  If param == 0, the absolute difference of intensity between x and y is used.\n"
	 "  If param == 1, the maximum between the intensities of x and y is used.\n"
	 "  If param == 2, the minimum between the intensities of x and y is used.\n"
	 "  If param == 3, a Deriche-like gradient is used, the optional parameter\n"
	 "    alpha specifies the spatial extention of the filter (by default alpha\n"
	 "   is set to  1]),\n"
	 "If the input image is color (input images are the red, green and blue channels), then\n"
	 "  If param == 0, the absolute difference of intensity between x and y is used.\n"
	 "  If param == 1, the Euclidean distance between x and y is used.\n"
	 "  If param == 2, the Fuzzy Affinity between x and y is used.\n"
	 "\n"
	 "If the input image is a 2D (resp. 3D) image, then the output image is a 2D (resp\n"
	 "3D edge graph), that is a 2D 4-connected edge-weighted graph (resp. a 3D\n"
	 "6-connected edge-weighted graph).\n"
         "\n"
         "Types supported: byte 2D, float 2D, byte 3D\n");
%newobject ComputeEdgeGraphGrey;
struct xvimage* ComputeEdgeGraphGrey(struct xvimage* im, int32_t param=0, double alpha=1.);
%newobject ComputeEdgeGraphColor;
struct xvimage* ComputeEdgeGraphColor(struct xvimage* r, struct xvimage* g, struct xvimage* b, int32_t param=0);

			  
%include "typemaps.i"
//Thanks to: http://jim-jotting.blogspot.co.il/2014/08/swig-python-and-opaque-structs.html
%typemap(in, numinputs=0) xvimage ** (xvimage *temp) {
  $1 = &temp;
}
%typemap(argout) xvimage ** {
  if (*$1 != NULL) {
  if (!PyList_Check($result)) {
    PyObject* temp = $result;
    $result = PyList_New(1);
    PyList_SetItem($result, 0, temp);
  }
  PyObject* temp = SWIG_NewPointerObj(*$1, SWIGTYPE_p_xvimage, SWIG_POINTER_OWN |  0);
  PyList_Append($result, temp);
  Py_DECREF(temp);
}}

%feature("autodoc", "watershedcut(edge-weighted-image, labeled-markers) -> [wc, labels]") watershedcut;
%feature("docstring",
	 "Compute a watershed cut of an edge-weighted image from a set of labeled markers,\n"
	 "using a minimum spanning forest algorithm.\n"
	 "Description:\n"
	 "Compute a minimum spanning forest of the edge weighted input image\n"
	 "(an edge-weighted graph that is 4-connected in 2D or 6-connected 3D)\n"
	 "relative to the labeled image of markers.\n"
	 "Return a list of both an edge-weighted image whose non-zero edges form the induced watershed cut\n"
	 "and a labeled image that represents the vertex partition induced by the watershed cut.\n"
	 "\n"
	 "See [COUSTYetAl-PAMI2009] and [COUSTYetAl-PAMI2010] for more details.\n"
	 "Types supported: edge-weighted byte 2D, edge-weighted byte 3D\n");
void watershedcut(struct xvimage* image, struct xvimage* markers,  xvimage** wc, xvimage** labels);

			  
%feature("docstring",
	 "Convert an edge-weighted image into its\n"
	 "representation in the Khalimsky grid\n"
	 "Description:\n"
	 "Convert a 4-connected (6-connected, in 3D) edge-weighted graph\n"
	 "into its Khalimsky grid representation (depending on the parameter ""type"").\n"
	 "The vertices of the graph are associated to the\n"
	 "square of the Khalimsky grid, and the edges of the graphs are\n"
	 "associated to the line segments.\n"
	 "\n"
	 "If type == 0, then the closure of the set of weighted-edges is used\n"
	 "(usefull for representing contours).\n"
	 "\n"
	 "If type == 1, then the dual of the closure of the set of weighted edges\n"
	 "is used (usefull for representing regions).\n"
	 "Types supported: edge-weighted byte 2D, edge-weighted byte 3D\n");
%newobject EWG2Khalimsky;
struct xvimage* EWG2Khalimsky(struct xvimage *edgeWeightedImage, int32_t type=0);

%feature("docstring",
	 "\brief labeling of the foreground components of a binary image\n"
	 "Description:\n"
	 "Each connected component of the input image is labeled with a unique integer, starting from 1.\n"
	 "The background points are labeled by 0.\n"
	 "The argument ""connex"" selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"
	 "The resulting output image has the type ""int32_t"".\n"
	 "Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n");
%newobject labelfgd;
struct xvimage* labelfgd(struct xvimage *imagein, int32_t connex=4);

%feature("docstring",
	 "Description:\n"
	 "The connected component of the binary input image\n"
	 "(according to the connectivity ""connex"")\n"
	 "which contains the point (x,y,z)\n"
	 "is copied in the resulting output image. All other points are ignored.\n"
	 "Possible values for ""connex ""are 4, 8 (2D), 6, 18, 26, 60, 260 (3D).\n"
	 "Values 60 and 260 correspond to restrictions of 6 and 26 connectivities to the \n"
	 "current xy plane.\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject selectcomp;
struct xvimage* selectcomp(struct xvimage *imagein, int32_t x, int32_t y, int32_t z, int32_t connex=4);

%feature("docstring",
	 "\brief converts an ""int32_t"" image to a ""byte"" image\n"
	 "Description:\n"
	 "Depending on the value given for the (optional) parameter ""mode"":\n"
	 "-   mode == 0 (default) : for all x, out[x] = min(255, in[x]).\n"
	 "-   mode == 1 : for all x, out[x] = in[x] modulo 256.\n"
	 "-   mode == 2 : scales values in the range 0-255.\n"
	 "-   mode == 3 : sorts the values by decreasing number of occurence in the image.\n"
	 "                Replaces the values by their order.\n"
	 "                Only the ""nbfirst"" (default 255) first values are kept.\n"
	 "                Useful for label images.\n"
	 "-   mode == 4 : truncation of the square root in the range 0-255.\n"
	 "-   mode == 5 : truncation of the log in the range 0-255.\n"
	 "Types supported: int32_t 2d, int32_t 3d\n");
%newobject long2byte;
struct xvimage* long2byte(struct xvimage *imagelong, int32_t mode=0, int32_t nbfirst=255);

%feature("docstring",
	 "converts a ""long"" image to a ""float"" image.\n"
	 "Types supported: int32_t 2d, int32_t 3d\n");
%newobject long2float;
struct xvimage* long2float(struct xvimage *imagelong);

%feature("docstring",
	 "converts a ""byte"" image to a ""float"" image.\n"
	 "Types supported: uint8_t 2d, uint8_t 3d\n");
%newobject byte2float;
struct xvimage* byte2float(struct xvimage *imagebyte);

%feature("docstring",
	 "converts a ""byte"" image to a ""int32_t"" image.\n"
	 "Types supported: uint8_t 2d, uint8_t 3d\n");
%newobject byte2long;
struct xvimage* byte2long(struct xvimage *imagebyte);

%feature("docstring",
	 "converts a ""float"" image to a ""int32_t"" image.\n"
	 "Applies the following tranformation to each value x in the input image :\n"
	 "X = round(offset + factor * x)\n"
	 "Types supported: float 2d, float 3d\n");
%newobject float2long;
struct xvimage* float2long(struct xvimage*imagefloat, double offset=0., double factor=1.);

%feature("docstring",
	 "Converts a ""float"" image to a ""byte"" image\n"
	 "Description:\n"
	 "Depending on the value given for the (optional) parameter ""mode"":\n"
	 "-   mode == 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"
	 "-   mode == 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"
	 "-   mode == 2 : scales values in the range 0-255.\n"
	 "-   mode == 4 : truncation of the square root in the range 0-255.\n"
	 "-   mode == 5 : truncation of the log in the range 0-255.\n"
	 "Types supported: float 2d, float 3d\n");
%newobject float2byte;
struct xvimage* float2byte(struct xvimage *imagefloat, int32_t mode=0);

%feature("docstring",
	 "Converts a ""double"" image to a ""byte"" image\n"
	 "Description:\n"
	 "Depending on the value given for the (optional) parameter ""mode"":\n"
	 "-   mode == 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"
	 "-   mode == 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"
	 "-   mode == 2 : scales values in the range 0-255.\n"
	 "-   mode == 4 : truncation of the square root in the range 0-255.\n"
	 "-   mode == 5 : truncation of the log in the range 0-255.\n"
	 "Types supported: double 2d, double 3d\n");
%newobject double2byte;
struct xvimage* double2byte(struct xvimage *imagedouble, int32_t mode=0);

%feature("docstring",
	 "Grayscale ultimate homotopic thinning\n"
	 "Description:\n"
	 "Grayscale ultimate homotopic thinning (refs. [BEC97, CBB01]).\n"
	 "The parameter ""connex"" gives the connectivity used for the minima;\n"
	 "possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"
	 "Let F be the function corresponding to the input image .\n"
	 "Let G be the function corresponding to the optional condition image \n"
	 """imcond"", or the null function if imcond is not provided.\n"
	 "The algorithm is the following:\n"
	 "Repeat until stability:\n"
	 "  select p destructible for F such that F(p) is minimal\n"
	 "  F(p) = max{ delta-(p,F), G(p) }    \n"
	 "Result: F\n"
	 "References: \n"
	 "[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \n"
	 "Image segmentation through operators based upon topology\n"
	 "Journal of Electronic Imaging, Vol. 6, No. 4, pp. 395-405, 1997.\n"
	 "[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \n"
	 "Topological operators for grayscale image processing\n"
	 "Journal of Electronic Imaging, Vol. 10, No. 4, pp. 1003-1015, 2001.\n"
	 "\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject htkern;
struct xvimage* htkern(struct xvimage *imagebyte, int32_t connex=4, struct xvimage* imagecond=NULL);

%feature("docstring",
	 "Grayscale filtered topological skeleton\n"
	 "Description:\n"
	 "Filtered topological skeleton for 2D grayscale images. \n"
	 "The connectivity used for the minima is 4;\n"
	 "The parameter ""lambda"" is a contrast parameter (positive integer).\n"
	 "Let F be the function corresponding to the input image \b in.pgm.\n"
	 "The (optional) image ""imagecond"" is a constraint function G.\n"
	 "If G is not provided by the user, then it is the null function.\n"
	 "The algorithm is the following:\n"
	 "\n"
	 "Repeat until stability:\n"
	 "  Select a point p which is lambda-destructible for F or a peak\n"
	 "      such that F(p) > G(p) and such that F(p) is minimal\n"
	 "    F(p) = alpha-(p,F)\n"
	 "Result: F\n"
	 "\n"
	 "Reference: \n"
	 "[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \n"
	 "Topological operators for grayscale image processing\n"
	 "Journal of Electronic Imaging, Vol. 10, No. 4, pp. 1003-1015, 2001.\n"
	 "\n"
	 "Types supported: byte 2d, byte 3d\n");
%newobject lambdaskel;
struct xvimage* lambdaskel(struct xvimage *imagebyte, int32_t lambda, struct xvimage* imagecond=NULL);

%feature("docstring",
	 "topological lower filter\n"
	 "Description:\n"
	 "Topological upper filter. Performs the homotopic thinning controlled by\n"
	 "a radius ""radius"", followed by a peak deletion, and a homotopic reconstruction under\n"
	 "the original image.\n"
	 "Types supported: byte 2d\n");
%newobject tuf;
struct xvimage* tuf(struct xvimage *imagebyte, int32_t connexmin, int32_t radius);

%feature("docstring",
	 "topological lower filter\n"
	 "Description:\n"
	 "Topological lower filter. Performs the homotopic thickening controlled by\n"
	 "a radius ""radius"", followed by a well deletion, and a homotopic reconstruction over\n"
	 "the original image.\n"
	 "Types supported: byte 2d\n");
%newobject tlf;
struct xvimage* tlf(struct xvimage *imagebyte, int32_t connexmin, int32_t radius);

%feature("docstring",
	 "Crest restoration algorithm \n"
	 "Description:\n"
	 "Crest restoration algorithm, as described in ref. CBB01.\n"
	 "The input image must be a ""thin"" grayscale image, as\n"
	 "the output of the operator ""hthin"".\n"
	 "The parameter ""niter"" gives the number of iterations.\n"
	 "The parameter ""connex"" indicates the connectivity used for the minimal regions (4 or 8, default is 4).\n"
	 "The optional parameter ""imcond"" is a binary image (a set C) which indicates the points\n"
	 "in the neighborhood of which the extensible points will be searched.\n"
	 "The points which are modified by the algorithm will be dynamically added to C.\n"
	 "If imcond is provided, then an image containing the final state of the \n"
	 "set C is also returned.\n"
	 "\n"
	 "Reference: \n"
	 "[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand\n"
	 "Topological operators for grayscale image processing\n"
	 "Journal of Electronic Imaging, Vol. 10, No. 4, pp. 1003-1015, 2001.\n"
	 "\n"
	 "Types supported: byte 2D\n");
%newobject crestrestoration;
struct xvimage* crestrestoration(struct xvimage *imagebyte, int32_t niter, int32_t connex=4, struct xvimage* imcond=NULL, xvimage** condout=NULL);

%feature("docstring",
	 "TO DO - LNA Private\n"
	 "Types supported: float 2D\n");
%newobject zerocrossing;
struct xvimage* zerocrossing(struct xvimage *imagefloat, int32_t bar);

%feature("docstring",
	 "TO DO - LNA private\n"
	 "Types supported: float 2D\n");
%newobject interpolateX2;
struct xvimage* interpolateX2(struct xvimage *imagefloat);
