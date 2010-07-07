// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyzoom.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


// note this function is too rigid (you can't choose the zoom algorithm)
// and should definitely be replaced later

template <class object_type>
PTR<object_type> py_zoom( const object_type & image,
			  double fx, double fy, double fz // the zoom factors 
		
  ){

  PTR<object_type> copy( new object_type( image ) );
  
  xvimage * out;

  lzoom( &(* copy ), // '&(* copy )' == 'copy->operator&' and returns an 'xvimage*'
	 &out,
	 fx, fy, fz 
    );
  

  PTR<object_type> result( new object_type(*out) );

  freeimage(out);
 
  return result;
};


void py_zoom_export(){

  def("cpp_py_zoom", &py_zoom<char_image>,
      args("image", "zoom factor X", "zoom factor Y", "zoom factor Z" ),
      "Zoom (shrink or engarle) an image. "
      "The three arguments are the zoom factors. "
      "More advanced parametrization through the 'pink.zoom' function."
      "Note: this function is too rigid (you can't choose the zoom algorithm) "
      " and should definitely be replaced later"
    );

  def("cpp_py_zoom", &py_zoom<short_image>,
      args("image", "zoom factor X", "zoom factor Y", "zoom factor Z" ),
      "Zoom (shrink or engarle) an image. "
      "The three arguments are the zoom factors. "
      "More advanced parametrization through the 'pink.zoom' function."
      "Note: this function is too rigid (you can't choose the zoom algorithm) "
      " and should definitely be replaced later"
    );

  def("cpp_py_zoom", &py_zoom<int_image>,
      args("image", "zoom factor X", "zoom factor Y", "zoom factor Z" ),
      "Zoom (shrink or engarle) an image. "
      "The three arguments are the zoom factors. "
      "More advanced parametrization through the 'pink.zoom' function."
      "Note: this function is too rigid (you can't choose the zoom algorithm) "
      " and should definitely be replaced later"
    );

  def("cpp_py_zoom", &py_zoom<float_image>,
      args("image", "zoom factor X", "zoom factor Y", "zoom factor Z" ),
      "Zoom (shrink or engarle) an image. "
      "The three arguments are the zoom factors. "
      "More advanced parametrization through the 'pink.zoom' function."
      "Note: this function is too rigid (you can't choose the zoom algorithm) "
      " and should definitely be replaced later"
    );

  def("cpp_py_zoom", &py_zoom<double_image>,
      args("image", "zoom factor X", "zoom factor Y", "zoom factor Z" ),
      "Zoom (shrink or engarle) an image. "
      "The three arguments are the zoom factors. "
      "More advanced parametrization through the 'pink.zoom' function."
      "Note: this function is too rigid (you can't choose the zoom algorithm) "
      " and should definitely be replaced later"
    );


};


















































// LuM end of file
