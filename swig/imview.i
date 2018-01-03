/* File pinkswig.i
 author: Laurent Najman
 date: 21/01/2016
*/

%module(docstring="Interface to the imview viewer for the Pink library") imview

%{
#define SWIG_FILE_WITH_INIT
#include <mccodimage.h>

#include <com/imview.h>

%}

%feature("autodoc", "1");


/*
  HT 2018-01-03
  necessary for bringing the print function in python 2.x
 */
%pythonbegin %{
from __future__ import print_function
%}

%pythoncode{

import numpy as np
  
def imview(images,name="",debug=False):
    """A function to display an image in Pink/Python. It works on
    images and lists of images.  
    An image is either a pink/xvimage or a Numpy array
    An optional name can be provided for the window title.
    The function returns an Imview object.
    Examples:
    viewer = imview.imview(I)
    imview.imview([I,J])
    imview.imview(I, ""A beautiful image"")
    """
    if not isinstance(images, list):
        viewer = Imview(images, name)
        if (debug):
            viewer.debug(True)
        return viewer
    else:
        viewer = Imview(images[0], name+"-"+str(0))
        for q in range(1, len(images)):
            viewer.show(images[q], name+"-"+str(q))
        return viewer

class Imview:
    """A class to display images in Pink/Python.
    An image is either a pink/xvimage or a Numpy array"""
    def __init__(self, image, viewername=""):
        self.port = __init__()
        self.conn = __login__("","",self.port)
        self.viewername = viewername
        self.show(image,self.viewername+"-"+image.name(),self.conn)
    def debug(self,d=False):
        self.debugstatus = __setdebug__(d)
    def show(self,image, name="", color=False):
        """Display an xv/numpy image.
           Use both the (optional) name and te image name"""
        if hasattr(image, 'tonumpy'): 
            self.up = __putimage__(image,name+"-"+image.name(),self.conn)
        else:
            self.__showNumpy__(image, name, color)

    def __showRGBNumpy__(self, mat, name=""):
        R = np.ascontiguousarray(mat[...,0])
        G = np.ascontiguousarray(mat[...,1])
        B = np.ascontiguousarray(mat[...,2])
        if len(mat.shape) == 3:
            row, col, ignore = mat.shape
            depth = 1
        else: 
            row, col, depth, ignore = mat.shape
        type = mat.dtype.name
        translate = {'uint8' : 2, 'uint16' : 4, 'uint32' : 6, 'float32' : 9, 'float64' : 10}
        if type in translate:
            self.up = __putbufferRGB__(R.ctypes.data, G.ctypes.data, B.ctypes.data,
                                      row, col, depth, translate[type], name, self.conn)
            return self.up
        else:
            print("Type:", type, "is not supported")
            return 0

    def __showNumpy__(self, mat, name="", color=False):
        if color == True:
            return self.__showRGBNumpy__(mat, name)
        if len(mat.shape) == 2:
            row, col = mat.shape
            depth = 1
        else: 
            row, col, depth = mat.shape
        if depth == 3:
            return self.__showRGBNumpy__(mat, name)
        type = mat.dtype.name
        translate = {'uint8' : 2, 'uint16' : 4, 'uint32' : 6, 'float32' : 9, 'float64' : 10}
        if type in translate:
            self.up = __putbuffer__(mat.ctypes.data,
                            row, col, depth, translate[type], name, self.conn)
            return self.up
        else:
            print("Type:", type, "is not supported")
            return 0      
}

%rename(__init__) init;
%rename(__login__) login;
%rename(__putimage__) putimage;
%rename(__putbufferRGB__) putbufferRGB;
%rename(__putbuffer__) putbuffer;
%rename(__setdebug__) setdebug;
%rename(__sendcommand__) sendcommand;

int init(void);
int login(char *user, char *hostname, int port);
int putimage(struct xvimage *realdata, const char *name, int conn_id);
int putbufferRGB(long int dataR, long int dataG, long int dataB, int row, int col, int depth, int datatype, const char *name, int conn_id);
int putbuffer(long int data, int row, int col, int depth, int datatype, const char *name, int conn_id);
int setdebug(int debug);
int sendcommand(char *command, int connid);
