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


%pythoncode{
  
def imview(images,imagename="image",debug=False):
    """A function to display an image in Pink/Python. It works on
    images and lists of images.  
    An optional name can be provided for the window title.
    The function returns an Imview object.
    Examples:
    viewer = imview.imview(I)
    imview.imview([I,J])
    imview.imview(I, ""A beautiful image"")
    """
    if not isinstance(images, list):
        viewer = Imview(images, imagename)
	if (debug):
	  viewer.debug(True)
        return viewer
    else:
	num = 0
        viewer = Imview(images[0],imagename+"-"+str(0))
	for q in range(1, len(images)):
	    viewer.show(images[q], imagename+"-"+str(q))
        return viewer

class Imview:
    """A class to display images in Pink/Python"""
    def __init__(self, image, imagename="image"):
	self.port = __init__()
        self.conn = __login__("","",self.port)
        self.imagename = imagename
        self.up = __putimage__(image,self.imagename,self.conn)
    def debug(self,d=False):
        self.debugstatus = __setdebug__(d)
    def show(self,image, name="image"):
	self.up = __putimage__(image,name,self.conn)
	if (self.up > 0):
	    self.__init__(image)

	      
}

%rename(__init__) init;
%rename(__login__) login;
%rename(__putimage__) putimage;
%rename(__setdebut__) setdebug;
%rename(__sendcommand__) sendcommand;

int init(void);
int login(char *user, char *hostname, int port);
int putimage(struct xvimage *realdata, const char *name, int conn_id);
int setdebug(int debug);
int sendcommand(char *command, int connid);