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
#define error(msg) {stringstream fullmessage; fullmessage << "in uiCutPlane.cpp: " << msg; call_error(fullmessage.str());}


namespace pink {
  
  PTR<char_image> draw_plane( 
    const char_image & original, 
    float a, 
    float b, 
    float c, 
    float d )
  {
    PTR<char_image> plane(new char_image(original.get_size()));
  
    PTR<char_image> result;

    vint curr( plane->get_size().size(), "curr"  );
    
    FOR(q, plane->get_size().prod())
    {
      plane->get_size().nextStep( q, curr );
      
      if ( uiAbs(a*curr[0]+b*curr[1]+c*curr[2]+d) 
	   / sqrt( a*a + b*b + c*c ) <= 0.5 )
      {
    	(*plane)[q]=255;	
      } /* if */
      
    } /* FOR */
    

    result = immap_min( original, *plane );

    return result;
  }

  

 






} /* namespace pink */
/* LuM end of file */

