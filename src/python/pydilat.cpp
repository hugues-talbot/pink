/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include "ldilateros.h"
#include "ldilateros3d.h"

namespace pink {
  namespace python {

    template <class image_t>
    image_t
    dilation ( const image_t & image, const char_image & elem )
    {
  
      if (! pink::inside( elem.size(), elem.center() ) )
        pink_error("The center of 'elem' must be specified.");
      
      image_t result = image.clone();
            
      if ( image.size().size()==2) // the image is 2D
      {        
        if (! ldilateros_ldilat( result, elem, elem.center()[0], elem.center()[1]))
          pink_error("function ldilateros_ldilat failed");
      }
      else  // NOT the image is 2D
      {
        if (! ldilat3d( result, elem, elem.center()[0], elem.center()[1], elem.center()[2]))          
          pink_error("function ldilat3d failed");
      } // NOT the image is 2D

      return result;    
    } /* dilation */

  } /* namespace python */
} /* namespace pink */


// UI_EXPORT_FUNCTION(
 
//   dilation,
//   pink::python::dilation,
//   ( arg("image"), arg("elem") ),
//   doc__dilation__c__
//   );
















































// LuM end of file
