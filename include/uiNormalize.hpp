/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#ifndef UINORMALIZE_HPP_
#define UINORMALIZE_HPP_

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiNormalize.hpp: " << msg; call_error(fullmessage.str());}

namespace pink { 

  PTR<float_image> lnormalize( const float_image & I,
			       float_image::pixel_type vmin = 0. ,
			       float_image::pixel_type vmax = 1. 
    );
  
  PTR<char_image> lnormalize( const char_image & I,
			      char_image::pixel_type vmin = 0 ,
			      char_image::pixel_type vmax = 255 
    );





} /* end namespace pink */

#endif /* UINORMALIZE_HPP_ */
/* LuM end of file */

 