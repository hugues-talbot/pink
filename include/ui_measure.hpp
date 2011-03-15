/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI__MEASURE_HPP_
#define UI__MEASURE_HPP_

namespace pink {

  const double epsilon=0.00001;  
  
  template <class image_type>
  image_type lmeasure( const image_type & image )
  {

    image_type result;
    result.copy(image);    
    
    float min, max;
    std::pair<float, float> mm;
  
  
    mm = lminmaxval(image);
    min = mm.first;
    max = mm.second;
  
    if ( min == max )
    {
      pink_error("All pixels are equal in the image.");
    } /* if */

    result = normalize<image_type, 0, 1>(image);

    result = uiGradientAbs(result);

    FOR(q, result.get_size().prod())
    {
      result[q] =  1. / ( epsilon + result[q] );
    } /* FOR */

    result = normalize<image_type, 0, 1>(result);

    return result;    
  } /* lmeasure*/ 

}; /* namespace pink */


#endif /*UI__MEASURE_HPP_*/
/* LuM end of file */
