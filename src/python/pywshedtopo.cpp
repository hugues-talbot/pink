// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pywshedtopo.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;




// int32_t lwshedtopo_lwshedtopo( struct xvimage *image, int32_t connex );


PTR<char_image> py_wshedtopo( char_image & orig, int connex ) {

  PTR<char_image> result( new char_image(orig) ); // copy constructor
  if (1 != lwshedtopo_lwshedtopo(  &( *result ), connex ) ) // '&(*result)' == 'result->operator&' and returns an 'xvimage*'
    error("lwshedtopo_lwshedtopo failed");
  return result;


};




void py_wshedtopo_export(){
  def("wshedtopo", &py_wshedtopo, 
      args("image", "connex"),
      "topological grayscale watershed (Couprie-Bertrand algorithm)\n"
      "Usage: wshedtopo in.pgm connex [i] out.pgm\n"
      "Description: Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity connex If the parameter i is given, then the dual operator is applied.\n"
      "    [CB97] M. Couprie and G. Bertrand: ``Topological Grayscale Watershed Transformation'', SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"
      "Types supported: byte 2d, byte 3d\n"
      "Category: connect\n"
      "Author:\n"
      "    Michel Couprie\n"
      "Example:\n"
      "wshedtopo ur1 4 ur1_wshedtopo"
    );
};

































// LuM end of file
