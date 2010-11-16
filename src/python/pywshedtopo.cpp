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
#define error(msg) {stringstream fullmessage; fullmessage << "in pywshedtopo.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;




// int32_t lwshedtopo_lwshedtopo( struct xvimage *image, int32_t connex );


char_image py_wshedtopo( const char_image & orig, int connex )
{

  char_image result;
  result = orig; 
  if (! lwshedtopo_lwshedtopo(  result.get_output(), connex ) )
  {
    error("lwshedtopo_lwshedtopo failed");
  }
  
  return result;
} /* py_wshedtopo */



UI_EXPORT_ONE_FUNCTION(
  wshedtopo,
  py_wshedtopo, 
  ( arg("image"), arg("connex") ),
  "topological grayscale watershed (Couprie-Bertrand algorithm)\n"
  "Usage: wshedtopo in.pgm connex [i] out.pgm\n"
  "Description: Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity connex If the parameter i is given, then the dual operator is applied.\n"
  "    [CB97] M. Couprie and G. Bertrand: ``Topological Grayscale Watershed Transformation'', SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"
  "Types supported: byte 2d, byte 3d\n"
  "Category: connect\n"
  "Author:\n"
  "    Michel Couprie\n"
  "Example:\n"
  "wshedtopo ur1 4 ur1_wshedtopo"
  )


































// LuM end of file
