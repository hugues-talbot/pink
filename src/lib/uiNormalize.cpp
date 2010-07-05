/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiNormalize.cpp: " << msg; call_error(fullmessage.str());}



namespace pink 
{
  
  PTR<float_image> lnormalize( const float_image & I,
			       float_image::pixel_type vmin /* = 0.*/  ,
			       float_image::pixel_type vmax/* = 1.*/ 
    )
  {
    PTR<float_image> result( new float_image(I) );
    
    lnormalize( result->get_output(), vmin, vmax );
    
    return result;
    
  } /* lnormalize */
  
  
  PTR<char_image> lnormalize( const char_image & I,
			      char_image::pixel_type vmin /* = 0*/  ,
			      char_image::pixel_type vmax  /*= 255 */
    )
  {
    PTR<char_image> result( new char_image(I) );
    
    lnormalize( result->get_output(), vmin, vmax );
    
  return result;
  
  } /* lnormalize */

} /* namespace pink */






  






/* LuM end of file */

