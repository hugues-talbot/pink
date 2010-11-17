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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyborder.cpp: " << msg; call_error(fullmessage.str());}

/// !!! macros should be named upper case
#undef border

using namespace boost::python;

namespace pink {
  namespace python {


    char_image border(const char_image & src, int connex)
    {
      char_image result;
      result = src;

      mctopo3d_lborder(result.get_output(), connex);

      return result;
    } /* py_border */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION( border, 
			pink::python::border, 
			(arg("image"), arg("connexity")),
			"\brief border of a binary image\n"
			"\n"
			"<B>Usage:</B> border in connex out\n"
			"\n"
			"<B>Description:</B>\n"
			"Let X be the subset of Zn that corresponds to the input image \b in, and \n"
			"let k be the value of the parameter \b connex (that is, 4, 8 (2d) or 6, 26 (3d)), which is the connectivity chosen for X.\n"
			"\n"
			"The border B(X) of X is defined by:\n"
			"B(X) = {x in X, (Gamma_nk(x) inter Xbar) not empty}\n"
			"where nk is the complementary connectivity for k.\n"
			"\n"
			"<B>Types supported:</B> byte 2d, byte 3d\n"
			"\n"
			"<B>Category:</B> topobin\n"
			"ingroup  topobin\n"
			"\n"
			"\author Michel Couprie\n"
			"*/\n"
			"\n"
  );






// LuM end of file
