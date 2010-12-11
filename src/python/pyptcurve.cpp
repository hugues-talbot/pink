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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyptcurve.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {

    char_image ptcurve( const char_image & image,
                        int connex
      )
    {
      char_image result;
      result.copy(image);
      if (! lptcurve(result.get_output(), connex))
      {
        error("function lptcurve failed");
      } /* if */

      return result;      
    } /* ptcurve */
    

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  ptcurve,
  pink::python::ptcurve,
  ( arg("image"),  arg("connex") ),
  doc__ptcurve__c__
  // end of the documentation
  );



















































// LuM end of file
