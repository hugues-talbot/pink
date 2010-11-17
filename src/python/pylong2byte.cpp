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
#define error(msg) {stringstream fullmessage; fullmessage << "in pylong2byte.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image long2byte(
      const int_image & src,
      int mode,
      int nbnewval
      )
    {
      int_image local_copy;
      local_copy = src;
  
      xvimage * res = long2byte( local_copy.get_output(), mode, nbnewval );

      char_image result(*res);
      freeimage(res);

      return result;    
    } /* py_long2byte */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  long2byte,
  pink::python::long2byte, 
  ( arg("image"),arg("mode"),arg("nb_new_val") ),
  "converts a 'int32_t' image to a 'byte' image \n"
  "\n"
  "Usage: long2byte in.pgm [mode] [n] out.pgm\n"
  "\n"
  "Description:\n"
  "\n"
  "Depending on the value given for the (optional) parameter mode:\n"
  " *  mode = 0 (default) : for all x, out[x] = min(255, in[x]).\n"
  " *  mode = 1 : for all x, out[x] = in[x] modulo 256.\n"
  " *  mode = 2 : scales values in the range 0-255.\n"
  " *  mode = 3 : sorts the values by decreasing number of occurence in the image.\n"
  "Replaces the values by their order.\n"
  "Only the n (default 255) first values are kept.\n"
  "Useful for label images.\n"
  " *  mode = 4 : truncation of the square root in the range 0-255.\n"
  " *  mode = 5 : truncation of the log in the range 0-255.\n"
  " \n"
  "Types supported: int32_t 2d, int32_t 3d\n"
  "\n"
  "Category: convert\n"
  "\n"
  "author Michel Couprie\n"
  );






















































// LuM end of file
