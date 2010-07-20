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

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymaxflow.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

template <class image_type>
PTR<image_type> py_seuil( 
  const image_type & image,  
  typename image_type::pixel_type threshold,
  typename image_type::pixel_type val_min,
  typename image_type::pixel_type val_max
  )
{

  PTR<image_type> result( new image_type(image.get_size()));
  
  FOR(q, image.get_size().prod())
  {
    if (image[q]>=threshold)
    {
      (*result)[q]=val_max;
    }
    else /* NOT image[q]>=threshold */
    {
      (*result)[q]=val_min;
    } /* NOT image[q]>=threshold */
  } /* FOR */

  return result;

} /* py_seuil */


UI_EXPORT_FUNCTION( cpp_seuil, 
		    py_seuil, 
		    args("image", "threshold value", "lower value", "upper_value"),
		    "this function takes an image, and replaces the values under 'threshold' with"
		    " minval, and the values over or equal with the 'threshold' with 'max_val'"
  )









// LuM end of file