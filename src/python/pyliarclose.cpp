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
    image_t liarclose
    (
      const image_t & image,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {
      image_t result;
      result.copy(image);

      pink_error("This function has been switched off!!!! You forgot to add some files!");
      
      
      // if (! ((image.get_size().size()==2) || (image.get_size().size()==3)) )
      // {
      //   pink_error("Only 2D and 3D images are supported.");
      // }


      // if (image.get_size().size() == 2) /* 2Dimage */
      // {
      //  if (! imfclose_rect( result.get_output(), SEnx, SEny, result.get_output()) )
      //   {
      //     pink_error("function imfclose_rect failed");
      //   }
      // }
      // else /* 3D image  */
      // {
      //   if (! imfclose3D_rect( result.get_output(), SEnx, SEny, SEnz, result.get_output()) )
      //   {
      //     pink_error("function imfdilat3D_rect failed");
      //   }
      // }  /* NOT image.get_size().size() == 2 */


      return result;
    } /* py_closing */
  } /* namespace python */
} /* namespace pink */


  UI_EXPORT_FUNCTION(

  liarclose,
  pink::python::liarclose,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  doc__closing__c__
  );






















































// LuM end of file
