/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI__MEASURE_HPP_
#define UI__MEASURE_HPP_

#include "larith.h"
#include "lminmax.hpp"
#include "pynumpy.hpp"
#include "uiNormalize.hpp"

namespace pink {

  const double epsilon=0.00001;  
 
  template <class image_type>
  image_type
  lmeasure( const image_type image )
  {

    image_type result;
    
    result = normalize<image_type, 0, 1>(image);
    
    result = uiGradientAbs(result);

    for ( auto & pixel : result )
      pixel =  1. / ( epsilon + pixel );

    result = normalize<image_type, 0, 1>(result);

    return result;    
  } /* lmeasure*/ 



  template <class image_type>
  image_type
  analytical_inverse( const image_type & image, typename image_type::pixel_type epsilon )
  {
    image_type result;
    result.copy(image);

    for ( auto & pixel : result )
      pixel = 1. / ( epsilon + pixel );
        
    return result;    
  }
  
  template <class image_type>
  boost::python::object
  pymeasure( boost::python::object image ) {
    
    image_type p0(image);

    auto result = lmeasure<image_type>(p0);
    writeimage( result, "_final_result.pgm" );
    
    return result.steel();
  }
    
}; /* namespace pink */


#endif /*UI__MEASURE_HPP_*/
/* LuM end of file */
