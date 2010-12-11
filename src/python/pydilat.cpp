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

    char_image dilation(
      const char_image & src, 
      char_image elem
      )
    {
  
      if ( not elem.get_size().inside(elem.get_center()) )
      {
        error("The center of 'elem' must be specified.");
      } /* if center not inside*/
      
      char_image result;
      result.copy(src);
      
      
      if ( src.get_size().size()==2) // the image is 2D
      {        
        if (! ldilateros_ldilat( result.get_output(), elem.get_output(),
                                elem.get_center()[0], elem.get_center()[1]))
        {
          error("function ldilateros_ldilat failed");
        } /* (! ldilateros_ldilat( src, elem_const_away, x, y)) */
      }
      else  // NOT the image is 2D
      {
        if (! ldilat3d( result.get_output(), elem.get_output(),
                       elem.get_center()[0], elem.get_center()[1], elem.get_center()[2]))          
        {
          error("function ldilat3d failed");
        } /* (! ldilat3d( src, elem_const_away, x, y)) */        
      } // NOT the image is 2D

      return result;    
    } /* dilation */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  dilation,
  pink::python::dilation,
  ( arg("src"), arg("elem") ),
  doc__dilation__c__
// end of the documenation
  );
















































// LuM end of file
