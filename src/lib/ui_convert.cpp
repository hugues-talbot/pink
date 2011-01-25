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

#include "uiFibreTypes.h"
#include "ujimage.hpp"
#include "mcutil.h"
#include "lminmax.hpp"
#include "llong2byte.h"




#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in ui_convert.cpp: " << msg; call_error(fullmessage.str());}

// you shouldn't use one character macros
#undef N

using namespace std;

namespace pink {

  char_image long2byte(
    const int_image & src,
    int mode,
    int nbnewval
    )
  {
    int_image local_copy;
    local_copy.copy(src);
  
    xvimage * res = long2byte( local_copy.get_output(), mode, nbnewval );

    char_image result(*res);
    freeimage(res);

    return result;    
  } /* long2byte */
  
  char_image float2byte( const float_image & src, int mode )
  {

    int N = src.get_size().prod();
    char_image result(src.get_size());
    float_image::pixel_type t;
    float_image::pixel_type tmp;
    float_image::pixel_type T;

    switch(mode)
    {

    case 0:
    
      FOR( x, N ) 
      {
        result[x] = static_cast<char_image::pixel_type>( mcmin( arrondi( src[x] ), 255 ) );
      } /* FOR */
      break;

    case 1:

      FOR( x, N ) 
      {
        result[x] = static_cast<char_image::pixel_type>( arrondi( src[x] ) %  256 );
      } /* FOR */
      break;

    case 2:
    
      float_image::pixel_type max;
      float_image::pixel_type min;
    
      max = lmaxval(src);
      min = lminval(src);

      FOR( x, N ) 
      {
        t = ( ( src[x] - min ) * 255.0 ) / static_cast<float_image::pixel_type>( max - min );
        tmp = arrondi(t);
        result[x] = static_cast<char_image::pixel_type>( mcmin( 255, tmp ) );
      } /* FOR */
      break;

    case 4:

      FOR( x, N ) 
      {

        T = sqrt( static_cast<double>( src[x]) );
        tmp = arrondi( T );
        tmp = mcmin( 255, tmp );
        tmp = mcmax( 0, tmp );
        result[x] = static_cast<char_image::pixel_type>(tmp);
      } /* FOR */

      break;



    case 5:

      FOR( x, N ) 
      {

        T = log(static_cast<double>(src[x]));
        tmp = arrondi( T );
        tmp = mcmin( 255, tmp );
        tmp = mcmax( 0, tmp );
        result[x] = static_cast<char_image::pixel_type>(tmp);
      } /* FOR */

      break;



    default:
      error( "mode must be: \nmode = 0 (trunc) \n 1 (modulo) \n 2 (scale) \n 4 (square root) \n 5 (log)\n" );

    } /* switch(mode) */
  

    return result;

  } /* float2byte */


  int_image byte2long( const char_image & image )
  {
    int_image result(image.get_size());

    std::copy( &(image[0]), &(image[image.get_size().prod()-1]), &(result[0]) );
    
    return result;    
  };

#ifdef MCNEW
  
  float_image byte2complex( const byte_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* byte2complex */
  
  float_image long2complex( const int_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* float2complex */
  
  float_image float2complex( const float_image & src )
  {

    int N = src.get_size().prod();
    float_image result(2*src.get_size());
    
    FOR( x, N ) 
    {
        result[x] = static_cast<float_image::pixel_type>(src[x]);
    } /* FOR */
    
    FOR( x, N ) 
    {
        result[N+x] = static_cast<float_image::pixel_type>0;
    } /* FOR */

    return result;

  } /* float2complex */
#endif
  
} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
