/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_FRAME__
#define UI_FRAME__

#include "mcimage.h"
#include "ujimage.hpp"
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

namespace pink { 

  template <class image_type>
  image_type
  frame(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {
    image_type result;
    result = src_image.clone();

    for ( auto pixel = src_image.begin(); pixel != src_image.end(); ++pixel )
      if ( pink::on_side(src_image.size(), pixel.pos() ) ) *pixel = withval;

    return result;

  } /* frame */


  template <class image_type>
  image_type frame_around(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {
    std::vector<index_t> new_size(src_image.size());
    
    // calculating the size of the framed image
    for ( auto & coord : new_size ) coord += 2;
    
    image_type result(new_size);
    // saving the center of the image
    result.center() = src_image.center();

    for ( auto pixel = result.begin(); pixel != result.end(); ++pixel )
    {
      std::vector<index_t> pos = pixel.vpos();
      if (pink::on_side(new_size, pos))
        *pixel = withval;
      else
      {
        std::vector<index_t> source(pos);
        for ( auto & coord : source ) coord+=1;
        *pixel = src_image(source);        
      }
    }
  }
  
  template <class image_type>
  image_type
  frame_remove( const image_type & src_image )
  {
    
    std::vector<index_t> new_size(src_image.size());
    
    // calculating the size of the framed image    
    for( auto & coord : new_size ) coord -= 2;

    image_type result(new_size);

    // saving the center of the image
    result.center() = src_image.center();
    
    // copying the original image into the new image
    for ( auto pixel = result.begin(); pixel!=result.end(); ++pixel ) {
      std::vector<index_t> source(pixel.vpos());
      for ( auto & coord : source ) coord += 1;
      *pixel = src_image(source);
    } /* FOR */
   
    return result;

  } /* remove_frame */



  template <class image_type>
  image_type
  align_size( const image_type & image,
              index_t group_size,
              typename image_type::pixel_type fillval )
  {
    std::vector<index_t> new_size( image.size() );
    for ( auto & coord : new_size ) coord = pink::round_up( coord + 1, group_size );
    
    image_type result(new_size);

    pink::fill( result, fillval );

    for ( auto pixel = image.begin(); pixel != image.end(); ++pixel ) {
      std::vector<index_t> pos = pixel.vpos();
      for ( auto & coords : pos ) coords++;
      result(pos) = *pixel;      
    }

    return result;
  } /* align_size */
  






























  } /* namespace pink */


#endif /* UI_FRAME__ */
/* LuM end of file */
