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
  



} /* end namespace pink */

#endif /*LMINMAX_H_*/
/* LuM end of file */

 
