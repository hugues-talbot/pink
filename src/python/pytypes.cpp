// UjoImro, 2009
// This software is meant to be free

#include <pink_python.h>

using namespace boost::python;
using namespace pink;


typedef vector<int> vector_int;


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


Python class pink.xvimage / c++ xvimage


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

void xvimage_object_export(){
  class_< xvimage, PTR<xvimage> >(
    "xvimage"
    "pink's 'struct xvimage'. It's included because of the inheritance "
    "(shallow_xvimage and deep_xvimage). "
    );
};

/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


Python class pink.vector_int / c++ std::vector<int>


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

void vector_int_object_export(){
  class_< vector_int >("vector_int", "c++'s standard library integer vector")
    .def( vector_indexing_suite< vector<int> >())
    ;
};


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


Python class pink.vint / c++ pink::vint


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

void vint_object_export(){

  class_< vint, 
    PTR<vint>, 
    bases<vector<int> > >
  ( "vint", 
    "This class is an extended 'vector_int' "
    "which is used as the dimension of the images "
    "and also as the coordinates of the pixels.",
    init<>(args("self"), "Default constructor. Creates an empty vector"))
  
  
  
//   .def( 
//     init< vint >     
//     (
//       args("self","src") , 
//       "copy constructor" ) 
//     )
  
  
  
//   .def( 
//     init< int  > ( 
//       args("self", "length"), 
//       "constructs a 'vint' with a givel length") 
//     )
  
  
  
//   .def( 
//     init< int, int > ( 
//       args("self", "length", "default_value"),
//       "constructs a 'vint' with a given 'length' and 'default_value' "
//       ) 
//     )
  
  
  
  .def(
    init< boost::python::list >(
      args("self","src"),
      "creates a 'vint' object from the elements of the 'src' python list. "
      "All the elements of the python list must be convertible into c++ 'int'."
      )
    )
  
  
  
  .def( "reset"       , &vint::reset, 
	args("self"), 
	"sets the values of 'vint' zero"
    )
  
  
  
  .def( "prod"        , &vint::prod,
	args("self"),
	"calculates the product of all the values of the 'vint'"
    )
  
  
  
  .def( "prodExcept"  , &vint::prodExcept,
	args("self", "p"),
	"calculates the product of all the values, but the p-th"
    )
  
  
  
  .def( "fabs"         , &vint::fabs,
	args("self"),
	"calculates the absolute value of the vector"
    )
  
  
  
  .def( "nextStep"    , &vint::nextStep,
	args("self", "step", "result"),
	"Suppose, that the 'vint' represents the dimensions of the image. "
	"This image is stored in an array: pixels = new imtype[vint::prod()]. "
	"This method calculates the coordinate corresponding with the given position 'step'."
	"The inverse function of 'nextStep' is 'position'."
    )

  
  
  .def( "nextStep_it" , &vint::nextStep_it, 
	args("self", "result"),
	"Suppose, that the 'vint' represents the dimensions of the image. "
	"This image is stored in an array: pixels = new imtype[vint::prod()]. "
	"If the parameter result represents the coordinates of the 'pixels[pos]' "
	"then this method calculates the coordinates of 'pixels[pos+1]'"
    )
  
  

  .def( "__eq__", &vint::operator==,
	args("self", "other"),
	"compares two vectors coordinate by coordinate"	
    )
  
  
  
  .def( "__ne__", &vint::operator!=,
	args("self", "other"),
	"compares two vectors coordinate by coordinate"	
    )
  
  
  
  .def( "position", &vint::position, 
	args("self", "elem"),
	"Suppose, that the 'vint' represents the dimensions of the image. "
	"This image is stored in an array: pixels = new imtype[vint::prod()]. "
	"If 'elem' stores the coordinates of the pixel in space, this method "
	"calculates the 'pos' of the corresponding 'pixel[pos]'. "
	"The inverse function of 'position' is 'nextStep'."
    )
    
/*.def("addSet", &vint::addSet, "I have no idea what this function does. And why?")*/
    
    .def( "__repr__", &vint::repr, 
	  args("self"),
	  "returns a string with the values in the vector" 
      )
    
    ;
  
}; /* vint_object_export */



// replaced with compile time ujoi_object_export template function in pyujimage.hpp

// declare_new_image_wrapper(
//   char_image,
//   char,
//   "char_image",
//   "This is the image object for 1 byte 'char' images"
// );

// declare_new_image_wrapper(
//   short_image,
//   short int ,
//   "short_image",
//   "This is the image object for 2 byte 'short int' images"
// );

// declare_new_image_wrapper(
//   int_image,
//   long int,
//   "int_image",
//   "This is the image object for 4 byte 'long int' images. "
//   "Note that on 32bit systems it's equivalent with c++ 'int'. "
//   "However while 'int' can change on 64bit systems, 'long int' "
//   "should remain 32bit."
// );

// declare_new_image_wrapper(
//   float_image,
//   float,
//   "float_image",
//   "This is the image object for 4 byte 'float' images"
// );

// declare_new_image_wrapper(
//   double_image,
//   double,
//   "double_image",
//   "This is the image object for 8 byte 'double' images"
// );
























































































































// LuM end of file
