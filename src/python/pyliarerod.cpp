/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "liar_fseries.h"
#include "pink_python.h"


using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {


    template <class image_t>
    image_t liarerod
    (
      const image_t & src,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {

      image_t result;
      result.copy(src);

     // The low-level function imferod_rect etc return 0 to indicate success
    // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
         if (imferode_rect( result.get_output(), SEnx, SEny, result.get_output()) )
         {
           pink_error("function imferode_rect failed");
         }
       }
       else  // NOT the image is 2D
       {
         if (imferode3D_rect( result.get_output(), SEnx, SEny, SEnz, result.get_output()) )
         {
           pink_error("function imferode3D_rect failed");
         }
       } // NOT the image is 2D

      return result;
    } /* liarerod */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
  liarerod,
  pink::python::liarerod,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  doc__erosion__c__
  // end of the documenation
  );




















































// LuM end of file
