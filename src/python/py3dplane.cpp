/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include "ujimage.hpp"
#include "l3dkhalimsky.h"


namespace pink {
  namespace python {


    char_image plane3d(
      const char_image & image, 
      const double &a,
      const double & b,
      const double & c,
      const double & d ) {
      
      char_image result;
      result = image.clone();
      
      if ( !l3dplane( result, a, b, c, d ) ) pink_error("function l3dplane failed");
      
      return result;
      
    } /* py_3dplane */
    
  } /* namespace python */
} /* namespace pink */



// UI_EXPORT_ONE_FUNCTION(
//   plane3d, 
//   pink::python::plane3d, 
//   ( arg("image"), arg("coefficient of x"), arg("coefficient of y"), arg("coefficient of z"), arg("absolut part")),
//   doc__3dplane__c__
//   );



// LuM end of file
