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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyptend.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image ptend(
      const char_image & image,
      int connex
      )
    {
      char_image result;
      result.copy(image);

      if (! lptend(result.get_output(), connex))
      {
        error("function lptend failed\n");
      }
      
      return result;      
    } /* ptend */
    
    


  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  ptend,
  pink::python::ptend,
  ( arg("image"), arg("connexity")),
  doc__ptend__c__
  );



















































// LuM end of file
