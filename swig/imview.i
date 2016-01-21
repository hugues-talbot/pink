/* File pinkswig.i
 author: Laurent Najman
 date: 21/01/2016
*/

%module(docstring="Interface to the imview viewer for the Pink library") imview

%{
#define SWIG_FILE_WITH_INIT
#include <mccodimage.h>

#include <com/imview.h>
void LIAREnableDebug(void);
void LIARDisableDebug(void);
/*  
#include <liarp.h>
#include <liarwrap.h>
#include <imclient.h>
*/
%}

%feature("autodoc", "1");


%pythoncode{
  
def __namestr__(obj, namespace):
    return [name for name in namespace if namespace[name] is obj]
  
def imview(images,debug=False):
    """A function to display an image in Pink/Python. It works on
    images and lists of images:
    pink.imview(I)
    pink.imview([I,J])
    """
    if not isinstance(images, list):
	if (debug):
	    __LiarEnableDebug__()
	port = __init__()
        conn = __login__("", "", port)
        imagename = __namestr__(images,locals())[0]
        up = __putimage__(images,imagename,conn)
        return None
    else:
	num = 0
	viewer = Imview(images[0])
	for q in range(1, len(images)):
	    viewer.show(images[q], str(q))
        return viewer

class Imview:
    """A class to display images in Pink/Python"""
    def __init__(self, image):
	self.port = __init__()
        self.conn = __login__("","",self.port)
        self.imagename = __namestr__(image, locals())[0]
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
%rename(__LIAREnableDebug__) LIAREnableDebug;
%rename(__LIARDisableDebug__) LIARDisableDebug;

int init(void);
int login(char *user, char *hostname, int port);
int putimage(struct xvimage *realdata, const char *name, int conn_id);
int setdebug(int debug);
int sendcommand(char *command, int connid);
void LIAREnableDebug(void);
void LIARDisableDebug(void);
