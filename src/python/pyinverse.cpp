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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyinverse.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;
 
namespace pink {
  namespace python {
    
    template<class image_type>
    image_type inverse( const image_type & image )
    {
      image_type result;
      result.copy(image);
      
      if (!linverse(result.get_output()))
      {
        error("function linverse failed");        
      }       
      
      return result;      
    } /* py_inverse */
    
    
  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  inverse,
  pink::python::inverse,
  ( arg("image") ),
  "WRITE ME!!!"
// end of the documenation
  );    

// LuM end of file
