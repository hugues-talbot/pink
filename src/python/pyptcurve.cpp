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
  "brief detects curve points in a binary image"
  ""
  "<B>Usage:</B> ptcurve in.pgm connex out.pgm"
  ""
  "<B>Description:</B>"
  "An curve point is a white point, which has exactly 2 white n-neighbours "
  "that are not n-neighbours to each other. In other words, Tn = 2 and "
  "each neighboring n-connected component is made of exactly one point"
  "(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter b connex)"
  ""
  "<B>Types supported:</B> byte 2d, byte 3d"
  ""
  "<B>Category:</B> topobin"
  "ingroup  topobin"
  ""
  "author Michel Couprie 2002"


  );



















































// LuM end of file
