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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyopening.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



char_image py_opening(
  const char_image & src, 
  const char_image & elem
  )
{

  if (not (elem.get_center()[0]>=0) and (elem.get_center()[1]>=0))
  {
    error("The center of 'elem' must be specified.");
  }

  if (src.get_size().size()!=elem.get_size().size()){
    error("error: the dimensions of 'src' and 'elem' must be equal");
  }

  char_image result;
  result = src;
  char_image elem_const_away;
  elem_const_away = elem;

  if (result.get_size()==2)
  {
    int rs = result.get_size()[0];
    int cs = result.get_size()[1];
    int x = elem_const_away.get_center()[0];
    int y = elem_const_away.get_center()[1];

    if (! ldilateros_leros( result.get_output(), elem_const_away.get_output(), x, y))
    {
      error("function leros failed");
    }      
    
    if (! lsym( elem_const_away.get_output(), 'c'))
    {
      error("function lsym failed");
    }
    
    if (! ldilateros_ldilat( result.get_output(), &(* elem_const_away ), rs - 1 - x, cs - 1 - y)) 
    {
      error("function ldilat failed");
    }

  } 
  else /* NOT result.get_size()==2 */ 
  {
    if (result.get_size()==3)
    {

      if (not ((elem.get_center())[3]>=0))
      {
	error("The center of 'elem' must be specified.");
      };

      int rs = result.get_size()[0];
      int cs = result.get_size()[1];
      int ds = result.get_size()[2];
      int x = elem_const_away.get_center()[0];
      int y = elem_const_away.get_center()[1];
      int z = elem_const_away.get_center()[2];

      if (! leros3d( result.get_output(), elem_const_away.get_output(), rs - 1 - x, cs - 1 - y, ds - 1 - z))
      {
	error("function leros3d failed");
      }
      
      if (! lsym(elem_const_away.get_output(), 'c'))
      {
	error("function lsym failed");
      }
      
      if (! ldilat3d( result.get_output(), elem_const_away.get_output() , x, y, z))
      {
	error("function leros3d failed");
      }

    } 
    else /* NOT result.get_size()==3 */
    {
      error("pyopening: the image must be 2D or 3D");
    } /* NOT result.get_size()==3 */
    
  } /* NOT result.get_size()==2 */ 
  
  
  return result;    
} /* py_opening */


UI_EXPORT_ONE_FUNCTION( opening,
                        py_opening,
                        ( arg("src"), arg("elem") ),
                        " \n"
                        "morphological opening by a plane structuring element \n"
                        " \n"
                        "Description: \n"
                        "Morphological opening by a plane structuring element. \n"
                        "The (plane) structuring element is given by the non-null values in 'elem',  \n"
                        "its origin is the 'center' of the 'elem' \n"
                        " \n"
                        "Types supported: byte 2d, byte 3d \n"
                        " \n"
                        "Category: morpho \n"
                        " \n"
                        "author Michel Couprie 2002 \n"
                        " \n"
                        // end of the documenation
  )























// LuM end of file
