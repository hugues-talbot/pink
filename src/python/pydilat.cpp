/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pydilat.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image dilat(
      const char_image & src, 
      const char_image & elem
      )
    {
  
      if ((src.get_size().size()!=2) and (elem.get_size().size()!=2))
      {
        error("only 2D images are supported, both 'src' and 'elem'");
      }
  
      if (not (elem.get_center()[0]>=0) and (elem.get_center()[1]>=0))
      {
        error("The center of 'elem' must be specified.");
      }

      char_image result;
      result = src;
      char_image elem_const_away;
      elem_const_away =elem;

      if (! ldilateros_ldilat( result.get_output(),
                               elem_const_away.get_output(),
                               elem_const_away.get_center()[0],
                               elem_const_away.get_center()[0]
            )
        )
      {
        error("function ldilat failed");
      } /* NOT (! ldilateros_ldilat( src, elem_const_away, x, y)) */

      return result;    
    } /* dilat */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  dilat,
  pink::python::dilat,
  ( arg("src"), arg("elem") ),
  " \n"
  "<B>dilat</B> - morphological dilation by a plane structuring element \n"
  " \n"
  "SYNOPSIS \n"
  " \n"
  "DESCRIPTION \n"
  " \n"
  "The (plane) structuring element is given by the non-null values in 'elem',  \n"
  "its origin is given by the 'center' of 'elem'  \n"
  " \n"
  "Types supported: byte 2D. \n"
  " \n"
  "category  \n"
  "morph \n"
  "author Michel Couprie \n"
  " \n"
// end of the documenation
  );
















































// LuM end of file
