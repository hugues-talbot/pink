/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyfloat2byte.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {

    char_image float2byte( const float_image & src, int mode=2 )
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
  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  float2byte,
  pink::python::float2byte,
  ( arg("source image"), arg("mode")=2 ),
  "converts a 'float_image' to a 'char_image' image\n\n"
  "Usage: float2byte( in ujoi, mode )\n"
  "Description:\n\n"
  "Depending on the value given for the (optional) parameter 'mode':\n"
  "    mode = 0 : for all x, out[x] = min(255, arrondi(in[x])).\n"
  "    mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"
  "    mode = 2 (default): scales values in the range 0-255.\n"
  "    mode = 4 : truncation of the square root in the range 0-255.\n"
  "    mode = 5 : truncation of the log in the range 0-255.\n\n"
  "Types supported: float_image\n"
  "Category: convert\n"
  "group convert\n"
  "author Michel Couprie\n"
  "adjusted Laszlo Marak 2009\n"
  " \n"
// end of the documenation
  );



//   def( "cpp_maxflow_cami_float", &maxflow_cami,
//        args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
//        "maximal flows, returns the flow (float array of size pixels * dimension ) \n"
//        " \n"
// // end of the documenation
//     );
  


