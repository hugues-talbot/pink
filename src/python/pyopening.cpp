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

#include "lsym.h"
#include "lgeodesic.h"
#include "python_doc.h"
#include "ldilateros.h"
#include "ldilateros3d.h"


namespace pink {
  namespace python {

    template <class image_t>
    image_t opening(
      const image_t    & image, 
      const char_image & c_elem
      )
    {

      if (!pink::inside( c_elem.size(), c_elem.center()))
        pink_error("The center of 'elem' must be specified.");

      if (image.size().size()!=c_elem.size().size())
        pink_error("error: the dimensions of 'image' and 'elem' must be equal");

      image_t result;
      result = image.clone();

      char_image elem = c_elem.clone();
            
      if (result.size().size()==2)
      {
        index_t rs = elem.size()[0];
        index_t cs = elem.size()[1];
        index_t x = elem.center()[0];
        index_t y = elem.center()[1];

        if (! ldilateros_leros( result, elem, x, y)) pink_error("function leros failed");
        
        if (! lsym( elem, 'c')) pink_error("function lsym failed");
        
        if (! ldilateros_ldilat( result, elem, rs - 1 - x, cs - 1 - y)) pink_error("function ldilat failed");
      }
      else /* NOT result.size()==2 */ 
      {
        if (result.size().size()==3)
        {

          index_t rs = elem.size()[0];
          index_t cs = elem.size()[1];
          index_t ds = elem.size()[2];
          index_t x = elem.center()[0];
          index_t y = elem.center()[1];
          index_t z = elem.center()[2];

          if (! leros3d( result, elem, x, y, z)) pink_error("function leros3d failed");
      
          if (! lsym(elem, 'c')) pink_error("function lsym failed");
      
          if (! ldilat3d( result, elem, rs - 1 - x, cs - 1 - y, ds - 1 - z)) pink_error("function leros3d failed");

        } 
        else /* NOT result.size()==3 */
        {
          pink_error("pyopening: the image must be 2D or 3D");
        } /* NOT result.size()==3 */
    
      } /* NOT result.size()==2 */ 
  
  
      return result;    
    } /* opening */


    // char_image openball(
    //   const char_image & image,
    //   int radius,
    //   int mode = 0
    //   )     
    // {

    //   char_image result;
    //   result.copy(image);      
      
    //   if ((mode != 0) && (mode != 2) && (mode != 4) && 
    //       (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
    //   {
    //     pink_error("dist = [0|2|4|8|6|18|26] ");
    //   }

    //   if (result.size().size()==2) // the result is 2D
    //   {
    //     if (! lerosdisc(result, radius, mode))
    //     {
    //       pink_error("function ldilatdisc failed");
    //     }
    //     if (! ldilatdisc(result, radius, mode))
    //     {
    //       pink_error("function lerosdisc failed");
    //     }
    //   }
    //   else // NOT the result is 2D
    //   {
    //     if (! lerosball(result, radius, mode))
    //     {
    //       pink_error("function ldilatball failed");
    //     }
    //     if (! ldilatball(result, radius, mode))
    //     {
    //       pink_error("function lerosball failed");
    //     }
    //   } // NOT the result is 2D

    //   return result;      
    // } /* openball */
    








  } /* namespace python */
} /* namespace pink */

// UI_EXPORT_FUNCTION(
//   opening,
//   pink::python::opening,
//   ( arg("image"), arg("elem") ),
//   doc__opening__c__
//   // end of the documenation
//   );


// UI_EXPORT_ONE_FUNCTION(
//   openball,
//   pink::python::openball,
//   ( arg("image"), arg("radius"), arg("mode")=0 ),
//   doc__openball__c__
//   );






















// LuM end of file
