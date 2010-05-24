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
#define error(msg) {stringstream fullmessage; fullmessage << "in pynormalize.cpp: " << msg; call_error(fullmessage.str());}


template <class image_type>
PTR<image_type> py_normalize( const image_type & src,
			      typename image_type::pixel_type a,
			      typename image_type::pixel_type b
			       
			 
  )
{


} /* py_normalize */


















































// LuM end of file
