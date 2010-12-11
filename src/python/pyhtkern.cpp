/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyhtkern.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;




namespace pink {
  namespace python {

    char_image htkern(
      const char_image & image,
      char_image imcond,
      int connexity
      )
    {
      char_image result;
      result.copy(image);      
      
      
      if ((connexity != 4) && (connexity != 8) && (connexity != 6) && (connexity != 26))
      {
        error("connexity = <4|8|6|26>");
      }
      
      if ((connexity == 4) || (connexity == 8)) // the image is 2D
      {
        if (! lhtkern(result, can_be_null(imcond), connexity))
        {
          error("lhtkern failed");
        }
      }
      else // NOT the image is 2D
      {
        if (! mctopo3d_lhtkern3d(result, can_be_null(imcond), connexity))
        {
          error("lhtkern3d failed");
        }
      } // NOT the image is 2D
      
      return result;
      
    } /* htkern */

    char_image htkern_short(
      const char_image & image,
      int connexity
      )
    {
      char_image result = htkern(image, char_image(), connexity);      
      return result;      
    } /* htkern_short */


    

  } /* namespace python */
} /* namespace pink */


void htkern_export()
{
  UI_DEFINE_ONE_FUNCTION(
    htkern,
    pink::python::htkern,
    ( arg("image"), arg("image condition"), arg("connexity")),
    doc__htkern__c__
    );
  
  UI_DEFINE_ONE_FUNCTION(
    htkern,
    pink::python::htkern_short,
    ( arg("image"), arg("connexity")),
    doc__htkern__c__
    );
  
} /* htkern_export */










// LuM end of file
