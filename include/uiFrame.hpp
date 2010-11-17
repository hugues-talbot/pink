/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_FRAME__
#define UI_FRAME__

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiFrame.hpp: " << msg; call_error(fullmessage.str());}

namespace pink { 

  template <class image_type>
  image_type frame(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {

    image_type result;
    result = src_image;

    vint curr( src_image.get_size() );
    
    FOR(q, src_image.get_size().prod())
    {
      src_image.get_size().nextStep( q, curr );
      if (src_image.get_size().on_side( curr ))
      {
	result[q]=withval;	
      } /* if */
    } /* FOR */

    return result;

  } /* frame */
  
  template <class image_type>
  image_type frame_around(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {

    vint new_size(src_image.get_size().size(), "new_size" );
    
    // calculating the size of the framed image
    FOR(q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] + 2;      
    } /* FOR */
    
    image_type result(new_size);

    vint curr( src_image.get_size().size(), "curr"  );

    // copying the original image into the new image
    FOR( q, src_image.get_size().prod() )
    {
      src_image.get_size().nextStep(q, curr);
      FOR( w, new_size.size() )
      {
    	curr[w]+=1;
      } /* FOR */
      //cout << curr.repr() << "\n";
      result[curr]=src_image[q];
    } /* FOR */
   
    
    // filling up the border with 'withval'
    FOR(q, result.get_size().prod())
    {
      result.get_size().nextStep( q, curr );
      if (result.get_size().on_side( curr ))
      {
    	result[q]=withval;	
      } /* if */
    } /* FOR */
    
    return result;

  } /* frame_around */


  template <class image_type>
  image_type frame_remove( const image_type & src_image )
  {
    
    vint new_size(src_image.get_size().size());
    
    // calculating the size of the framed image
    FOR( q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] - 2;
    } /* FOR */
    
    image_type result(new_size);
    
    vint curr( src_image.get_size() );
        
    // copying the original image into the new image
    FOR( q, result.get_size().prod() )
    {
      result.get_size().nextStep(q, curr);
      FOR( w, new_size.size() )
      {
	curr[w]+=1;  // same as with frame around
      } /* FOR */
      result[q]=src_image[curr];
    } /* FOR */
   
    return result;

  } /* remove_frame */

































  } /* namespace pink */


#endif /* UI_FRAME__ */
/* LuM end of file */
