/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by aplicable law.

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
    image_t liaropen
    (
      const image_t & src,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {
      image_t result;
      result.copy(src);

      pink_error("This function has been switched off!!!! You forgot to add some files!");
      
      
      // if ( src.get_size().size()==2) // the image is 2D
      // {
      //   if (! imfopen_rect( result.get_output(), SEnx, SEny, result.get_output()) )
      //   {
      //     pink_error("function imfopen_rect failed");
      //   }
      // }
      // else  // NOT the image is 2D
      // {
      //   if (! imfopen3D_rect( result.get_output(), SEnx, SEny, SEnz, result.get_output()) )
      //   {
      //     pink_error("function imfopen3D_rect failed");
      //   }
      // } // NOT the image is 2D

      return result;
    } /* liaropen */

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  liaropen,
  pink::python::liaropen,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  doc__opening__c__
  // end of the documenation
  );
























// LuM end of file
