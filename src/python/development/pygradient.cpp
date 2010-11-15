// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pygradient.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



void py_gradient_export(){

  def( "cpp_spline_absolut_gradient_float", &uiGradientAbs,
       args("source image"),
       "returns the image's spline approximated gradient's absolute value\n"
       " \n"
// end of the documenation
    );

  

};
