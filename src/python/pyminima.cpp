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

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyminima.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {


    template<class image_type>
    char_image minima(
      image_type image,
      string mode
      )
    {
      char_image result(image.get_size());

      lminima( image.get_output(), const_cast<char*>(mode.c_str()));

      // copying the values back to the result
      // note the values never depass 255 so copying is fine
      std::copy(
        &image[0],
        &image[image.get_size().prod()],
        &result[0]
        );
      
      
      return result;      
    } /* minima */

   
    

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  minima,
  pink::python::minima,
  ( arg("image"), arg("mode")),
  "WRITE ME!!"
// end of documentation
  );

















































// LuM end of file
