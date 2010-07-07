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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyinsert.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


UI_EXPORT_FUNCTION( cpp_insert_image, 
		    insert_image, 
		    args("big_image", "small_image", "shift_vector"),
		    "this function inserts an image to another one. The "
		    "shift vector indicates the position of the lower corner "
		    "of the new image. If the image doesn't fit into the big one, "
		    "it is cropped."
  )

UI_EXPORT_FUNCTION( cpp_merge_max_image, 
		    merge_max_image, 
		    args("big_image", "small_image", "shift_vector"),
		    "this function inserts an image to another one. The "
		    "shift vector indicates the position of the lower corner "
		    "of the new image. If the image doesn't fit into the big one, "
		    "it is cropped. During the insertion the pixels are compared "
		    "and the bigger value is conserved"
  )


// LuM end of file
