/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2006-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIIMAGE_H_
#define UIIMAGE_H_

#include "ui_pink_types.hpp"
#include "ujimage.hpp"

namespace pink
{
  
  const int longestline = 100;
  
  void uiWriteImage ( const pink::image<float> & image, const std::string & filename );
  void uiWriteFloatImage3D( const pink::image<float> & image, const std::string & filename );
  
//// in fact next two functions should not be exported
  void uiWriteImage2D ( const pink::image<float> & image, const std::string & filename );
  void uiWriteImage3D ( const pink::image<float> & image, const std::string & filename );
  
} /* namespace pink*/




#endif /*UIIMAGE_H_*/
/* LuM end of file */

