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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyeros.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



char_image py_eros(
  const char_image & src, 
  const char_image & elem
  )
{

  if ((src.get_size().size()!=2) and (elem.get_size().size()!=2))
  {
    error("Only 2D images are supported, both 'src' and 'elem'.");
  }

  if (not (elem.get_center()[0]>=0) and ((elem.get_center()[1]>=0)))
  {
    error("The center of 'elem' must be specified.");
  }

  char_image result;
  result = src;
  char_image elem_const_away;
  elem_const_away=elem;

  if (! ldilateros_leros( result.get_output(),
			  elem_const_away.get_output(),
			  elem_const_away.get_center()[0],
			  elem_const_away.get_center()[0]
	)
    )
  {
    error("function leros failed");
  } /* NOT (! ldilateros_leros( src, elem_const_away, x, y)) */

  return result;    
} /* py_eros */


UI_EXPORT_ONE_FUNCTION(
  eros,
  py_eros,
  (arg("src"), arg("elem")),
  "eros - morphological erosion by a plane structuring element \n"
  " \n"
  "DESCRIPTION \n"
  " \n"
  "The (plane) structuring element is given by the non-null values in 'elem',  \n"
  "its origin is given by the center of elem. \n"
  " \n"
  "Types supported: byte 2D. \n"
  " \n"
  "category morph \n"
  " \n"
  "author Michel Couprie - mars 1997 \n"
  " \n"
  // end of the documenation
  );




















































// LuM end of file
