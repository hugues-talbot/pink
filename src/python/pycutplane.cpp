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
#define error(msg) {stringstream fullmessage; fullmessage << "in pycutplane.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


UI_EXPORT_ONE_FUNCTION( cpp_draw_plane, 
			draw_plane, 
			args("image", "coefficient of x", "coefficient of y", "coefficient of z", "absolut part"),
			"this function draws a topological plane into the image. The "
			"equation of the plane is a*x+b*y+c*z+d==0\n"
  )





// LuM end of file
