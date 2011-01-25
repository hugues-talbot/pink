/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include <pink_python.h>

#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in pyborder.cpp: " << msg; call_error(fullmessage.str());}

/// !!! macros should be named upper case
#undef border

using namespace boost::python;

namespace pink {
  namespace python {


    char_image border(const char_image & src, int connex)
    {
      char_image result;
      result.copy(src);

      mctopo3d_lborder(result.get_output(), connex);

      return result;
    } /* py_border */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  border, 
  pink::python::border, 
  (arg("image"), arg("connexity")),
  doc__border__c__
  );






// LuM end of file
