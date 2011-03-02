/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UI_CONVERT_HPP_
#define UI_CONVERT_HPP_

namespace pink {


  char_image float2byte( const float_image & src, int mode=2 );

  char_image long2byte(
    const int_image & src,
    int mode,
    int nbnewval
    );
  
  template<class image_type>
  float_image convert2float( const image_type & image )
  {
    float_image result(image.get_size());
    
    FOR(q, image.get_size().prod())
    {
      result[q]=image[q];        
    } /* FOR */
    
    return result;      
  } /* convert2float */
  
#ifdef MCNEW
  template<class image_type>
  float_image convert2complex( const image_type & image )
  {
    float_image result(image.get_size());
    int N = src.get_size().prod();
    
    FOR(q, N)
    {
      result[q] = image[q];        
    } /* FOR */
    
    FOR(q, N)
    {
      result[N+q] = 0;        
    } /* FOR */
    
    return result;      
  } /* convert2complex */
#endif
  
  int_image byte2long( const char_image & image );
  
} /* namespace pink */
#endif /* UI_CONVERT_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
