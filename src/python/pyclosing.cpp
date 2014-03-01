/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include "lsym.h"
#include "ldilateros.h"
#include "pink_python.h"
#include "ldilateros3d.h"

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {

    template <class image_t>
    image_t closing
    (
      const image_t & image, 
      const char_image & elem
      )
    {
      image_t result;
      result = image.clone();
      char_image elem_ce; // this image is usually small so it's ok to copy it
      elem_ce = elem.clone();
      

      if (! (pink::inside(elem.size(), elem.center())))
        pink_error("The center of the structuring element must be defined");

      if (! ((image.size().size()==2) || (image.size().size()==3)) )
        pink_error("Only 2D and 3D images are supported.");

      if (image.size().size() == 2)
      {
        index_t x = elem.center()[0];
        index_t y = elem.center()[1];
        index_t rs = elem.rows();
        index_t cs = elem.cols();

        if (! ldilateros_ldilat(result, elem_ce, rs - 1 - x, cs - 1 - y)) pink_error("function ldilat failed");

        if (! lsym(elem_ce, 'c')) pink_error("function lsym failed");

        if (!ldilateros_leros(result, elem_ce, x, y)) pink_error("function leros failed");
      }
      else /* NOT image.size().size() == 2 */
      {
        index_t x  = elem.center()[0];        
        index_t y  = elem.center()[1];
        index_t z  = elem.center()[2];
        index_t rs = elem.rows();
        index_t cs = elem.cols();
        index_t ds = elem.depth();
        
        if (! ldilat3d(result, elem_ce, x, y, z)) pink_error("function leros3d failed");

        if (! lsym(elem_ce, 'c')) pink_error("function lsym failed");

        if (! leros3d(result, elem_ce, rs - 1 - x, cs - 1 - y, ds - 1 - z)) pink_error("function leros3d failed");
      }  /* NOT image.size().size() == 2 */

  
      return result;    
    } /* py_closing */

  } /* namespace python */
} /* namespace pink */


// UI_EXPORT_ONE_FUNCTION(
//   closeball, 
//   pink::python::closeball, 
//   ( arg("image"), arg("the ray of the ball"), arg("distance function")=0 ),
//   doc__closeball__c__
//   );



// UI_EXPORT_FUNCTION(
//   closing, 
//   pink::python::closing,
//   ( arg("src"), arg("elem") ),
//   doc__closing__c__
//   // end of the documenation
//   );


// LuM end of file
