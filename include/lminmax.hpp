#ifndef LMINMAX_H_
#define LMINMAX_H_
/*
This software is ment to be free.
(C) UjoImro, 2009-2010
ujoimro@gmail.com
*/

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in lminmax.hpp: " << msg; call_error(fullmessage.str());}

namespace pink { 

  template <class image_type>  
  typename image_type::pixel_type lmaxval( const image_type & I ){

    // getting the pixel type of the image
    typedef typename image_type::pixel_type pixel_type;

    // the variable to hold the maximum value
    pixel_type max = I[0];
    

    //calculating the number of pixels in the image
    int length_glob = I.get_size().prod();


    FOR(q, length_glob)
    {
      if (I[q] > max)
      {
	max = I[q];
      }; /* if (I[q] > max) */
	
    }; /* FOR */

    return max;
  } /* image::pixel_type maxval */


  template <class image_type>
  typename image_type::pixel_type lminval( const image_type & I ){

    // getting the pixel type of the image
    typedef typename image_type::pixel_type pixel_type;

    // the variable to hold the minimum value
    pixel_type min = I[0];
    

    //calculating the number of pixels in the image
    int length_glob = I.get_size().prod();


    FOR(q, length_glob)
    {
      if (I[q] < min)
      {
	min = I[q];
      }; /* if (I[q] > min) */
	
    }; /* FOR */

    return min;
  } /* image::pixel_type minval */

  
  template <class image_type>
  void lminmaxval( const image_type & I,
		  typename image_type::pixel_type & min, 
		  typename image_type::pixel_type & max
    )
  {

    typedef typename image_type::pixel_type pixel_type;
    
    pair< const pixel_type * , const pixel_type *> 
      result = boost::minmax_element( (&I[0]), (&I[I.get_size().prod()]) ); // here the prod is the correct position because it iterates up to

    max = (*result.first);
    min = (*result.second);
    
    
  } /* minmaxval */
  

  template <class image_type> 
  PTR<image_type> immap( const image_type & image1,
			 const image_type & image2,
			 bool (*map_function)( typename image_type::pixel_type val1,  
					       typename image_type::pixel_type val2 
			   )
    )
  {
    PTR<image_type> result (new image_type(image1.get_size()));
    
    FOR(q, image1.get_size().prod())
    {
      map_function( image1[q], image2[q]) ? (*result)[q]=image1[q] : (*result)[q]=image1[q];
      
    } /* FOR */
    
    return result;
  } /* immap */
  
  namespace lminmax 
  {

    template <class pixel_type>
    bool max_map( pixel_type val1, pixel_type val2 )
    {
      return (val1>=val2);
    } /* max_map */

    template <class pixel_type>
    bool min_map( pixel_type val1, pixel_type val2 )
    {
      return (val1<=val2);
    } /* min_map */
        

  } /* namespace lminmax */
  

  template <class image_type> 
  PTR<image_type> immap_max( const image_type & image1,
			     const image_type & image2
    )
  {

    typedef typename image_type::pixel_type pixel_type;

    return immap( image1, image2, &lminmax::max_map<pixel_type> );
    
  } /* immap_max */
  


  template <class image_type> 
  PTR<image_type> immap_min( const image_type & image1,
			     const image_type & image2
    )
  {

    typedef typename image_type::pixel_type pixel_type;

    return immap( image1, image2, &lminmax::min_map<pixel_type> );
  } /* immap_min */
    



} /* end namespace pink */

#endif /*LMINMAX_H_*/
/* LuM end of file */

 
