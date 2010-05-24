// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>


#define error(msg) {stringstream fullmessage; fullmessage << "in pygeoeros.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_geoeros( const char_image & F, 
			    const char_image & G, 
			    int connex, 
			    int numb_iter ){

  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    error("error: bad connexity - use one of the following:\n"
	  "4, 8 (in 2d), 6, 18, 26 (in 3d)\n"
      );
  } /* ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26)) */


  PTR<char_image> result( new char_image(F) );
  PTR<char_image> G_const_away( new char_image(G) );


  if (result->get_size().size()!=result->get_size().size()){
    error("error: the dimensions of F and G must be equal");
  };


  if (result->get_size().size()==2)
  {
    lgeoeros( &(*result), &(*G_const_away), connex, numb_iter ); 
    // &(*result) == result->operator& and it returns an "xvimage*"
    // &(*G_const_away) == G_const_away->operator& and it returns an "xvimage*"
  } 
  else 
  { /* NOT result.get_size().size()==2 */
    if (result->get_size().size()==3)
    {      
      lgeoeros3d( &(*result), &(*G_const_away), connex, numb_iter );
      // &(*result) == result->operator& and it returns an "xvimage*"
      // &(*G_const_away) == G_const_away->operator& and it returns an "xvimage*"
    } 
    else 
    { /* NOT result.get_size().size()==3 */
      error("error: only 2D and 3D images are supported");
    }; /* NOT result.get_size().size()==3 */
  }; /* NOT result.get_size().size()==2 */
  
  return result;    
};



void py_geoeros_export(){

  def( "cpp_geoeros", &py_geoeros,
       "geodesic (grayscale or binary) erosion \n"
       " \n"
       "Usage: geoeros g.pgm f.pgm connex niter out.pgm \n"
       " \n"
       "Description: \n"
       "Geodesic erosion of g.pgm over f.pgm \n"
       "Let G and F be the two input images. If G is not over F, then  \n"
       "G is replaced initially by max(G,F). \n"
       "The structuring element is specified by the value of the parameter connex, \n"
       "which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d. \n"
       "The parameter \b niter sets the number of iterations. If \b niter = -1, \n"
       "then the iterations continue until stability. \n"
       " \n"
       "Types supported: byte 2d, byte 3d. \n"
       " \n"
       "Category: connect, morpho \n"
       "group  connect, morpho \n"
       " \n"
       "author Michel Couprie - juillet 1996 \n"
       " \n"
// end of the documenation
    );
  

};


















































// LuM end of file
