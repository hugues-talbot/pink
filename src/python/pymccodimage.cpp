// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pymccodimage.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

typedef vector<int> vector_int;

// void xvimage_object_export(){
//   class_< xvimage, PTR<xvimage> >(
//     "xvimage",
//     "The C image struct of pink. It shouldn't be accessed directly from python. "
//     "Use one of the 'ujoi' objects instead."
//     );  
// };


void shallow_xvimage_object_export() {



  class_< shallow_xvimage, PTR<shallow_xvimage>, bases< xvimage > >( 
    "shallow_xvimage",     
    "The C image struct of pink, armed with some methods, like it can tell it's type. "
    "Be carefull, it does not contain or allocate any data! It's mainly used in the ujoi objects. "
    "It shouldn't be accessed directly from python. "
    "Use one of the 'ujoi' objects instead. "
    )



    .def( "imtype", &shallow_xvimage::imtype,
	  "returns the image type of the object. Current types are: "
	  "uint8_t, int16_t, int32_t, float, double "
      )
    ;



};



void deep_xvimage_object_export(){



  class_< deep_xvimage, PTR<deep_xvimage>, bases<xvimage> >( 
    "deep_xvimage",     
    "The C image struct of pink, armed with some methods, like it can tell it's type."
    "This child allocates the data! It's used mainly in readimage. "
    "It shouldn't be accessed directly from python. "
    "Use one of the 'ujoi' objects instead."
    )



    .def( "imtype", &deep_xvimage::imtype,
	  "returns the image type of the object. Current types are: "
	  " uint8_t, int16_t, int32_t, float, double"
      )

    
    ;

};






































// LuM end of file