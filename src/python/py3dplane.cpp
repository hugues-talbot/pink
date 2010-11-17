/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in py3dplane.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
//using namespace pink;

namespace pink {
  namespace python {


    char_image plane3d(
      const char_image & image, 
      double a,
      double b,
      double c,
      double d
      )
    {
      
      char_image result;
      result = image;
      
      if ( !l3dplane( result.get_output(), a, b, c, d ) )
      {
        error("function l3dplane failed");
      } /* if */
      
      
      return result;
      
    } /* py_3dplane */
    
  } /* namespace python */
} /* namespace pink */



UI_EXPORT_ONE_FUNCTION( plane3d, 
			pink::python::plane3d, 
			( arg("image"), arg("coefficient of x"), arg("coefficient of y"), arg("coefficient of z"), arg("absolut part")),
			"this function draws a morphological plane into the image. The "
			"equation of the plane is a*x+b*y+c*z+d==0\n"
  );



// LuM end of file
