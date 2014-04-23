/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "ujimage.hpp"
#include "lfiltreordre.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image medianfilter(
      const char_image & image, 
      const char_image & structuring_element,
      const std::vector<index_t> & center
      )
    {
      char_image result;
      result = image.clone();
  
      if ( image.size().size() == 2 ){ // the image is 2D    
        if (! lfiltreordre( result, structuring_element, center[0], center[1], 0.5 ) ) 
          pink_error("medianfilter: lfiltreordre failed");
      }
      else /* NOT image.size().size() == 2 */
      {    
        if ( image.size().size() == 3 ) // the image is 3D
        {    
          if (! lfiltreordre3d( result, structuring_element, center[0], center[1], center[2], 0.5 ) )
            pink_error("medianfilter: lfiltreordre failed");
        }
        else  /* NOT image.size().size() == 3 */ // the image is 4D or bad
          pink_error("erosball: only 2D and 3D images are supported");
      } /* NOT image.size().size() == 2 */
   
      return result;
    } /* medianfilter */

  } /* namespace python */
} /* namespace pink */

// UI_EXPORT_ONE_FUNCTION (
//   medianfilter,
//   pink::python::medianfilter,
//   ( arg("image"), arg("structuring_element") ),
//   doc__medianfilter__c__
//   );




















































// LuM end of file
