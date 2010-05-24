// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pyerosball.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_erosball( const char_image & src, float r, int mode ){

  PTR<char_image> result( new char_image(src) );
  
  if ( src.get_size().size() == 2 ){ // the image is 2D
    
    if ( 1 != lerosdisc( &(*result), r, mode ) ) // '&(*result)' == 'result->operator&' and returns an 'xvimage*'
      error("erosball: lerosdisc failed");

  } else if ( src.get_size().size() == 3 ) {// the image is 3D
    if ( 1 != lerosball( &(*result), r, mode ) ) // '&(*result)' == 'result->operator&' and returns an 'xvimage*'
      error("erosball: lerosball failed");
    
  } else { // the image is 4D or bad
    
    error("erosball: only 2D and 3D images are supported");
    
  };

  return result;    
};



void py_erosball_export(){

  def( "erosball", &py_erosball, 
       args("image","r","mode"),
       "pink's help:\n\n"
       "morphological binary erosion by a ball\n"
       " \n"
       "Usage: erosball in.pgm r [dist] out.pgm \n"
       " \n"
       "Description: Morphological binary erosion by a metric ball. This erosion is the dual of the dilatball operator, i.e. erosball(X) = inverse(dilatball(inverse(X))). The structuring element for this erosion is a ball (or a disc) of radius r. The erosion is computed by thresholding a distance map. The distance used depends on the optional parameter dist (default is 0) : \n"
       " \n"
       "    * 0: approximate euclidean distance \n"
       "    * 2: chamfer distance \n"
       "    * 4: 4-distance in 2d \n"
       "    * 8: 8-distance in 2d \n"
       "    * 6: 6-distance in 3d \n"
       "    * 18: 18-distance in 3d \n"
       "    * 26: 26-distance in 3d \n"
       " \n"
       "Warning: \n"
       "    The input image in.pgm must be a binary image. No test is done. \n"
       " \n"
       "Types supported: byte 2d, byte 3d \n"
       " \n"
       "Category: morpho \n"
       " \n"
       "Author: \n"
       "    Michel Couprie 2002 \n"
       " \n"
       "Example: \n"
       " \n"
       "erosball cells 3 cells_erosball \n"
// end of the documenation
    );


};


















































// LuM end of file
