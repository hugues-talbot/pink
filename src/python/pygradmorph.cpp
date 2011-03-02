/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink_python.h>

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {


    // if there will be many scripts, than i will need to
    // move these functions into the library

    char_image dilation(
      const char_image & src, 
      char_image elem
      );
    
    char_image erosion(
      const char_image & src, 
      char_image elem
      );
    

    
    char_image gradmorph(
      const char_image & image,
      const char_image & elem      
      )
    {
      char_image result;
      //result.copy(image);

      char_image dilated;
      char_image eroded;

      dilated = dilation(image, elem);
      eroded = erosion(image, elem);
      result = dilated - eroded;
      
      return result;      
    } /* ptcurve */
    

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  gradmorph,
  pink::python::gradmorph,
  ( arg("image"),  arg("structuring element") ),
  "Calculated the morphologic gradient according to the structuring element."
  );



















































// LuM end of file
