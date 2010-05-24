// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pymedianfilter.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


PTR<char_image> py_medianfilter( const char_image & image, 
				 const char_image & structuring_element				 
  ){
  
  // testing if the center of the 'structuring_element' is set
  if ( image.get_size().operator[](1) == -1 )
    ( error("medianfilter: the structuring element must be set") );
  
  
  PTR<char_image> result( new char_image( image ) );
  PTR<char_image> non_const_structuring_element( new char_image( structuring_element ) );
  
  if ( image.get_size().size() == 2 ){ // the image is 2D
    
    if ( 1 != 
	 lfiltreordre( &(* result ), // '&(*result)' == 'result->operator&' and returns an 'xvimage*'
		       &(* non_const_structuring_element ),
		       non_const_structuring_element->get_center()[0], 
		       non_const_structuring_element->get_center()[1],
		       0.5
	   ) 
      )
      
      error("medianfilter: lfiltreordre failed");
    
  } else if ( image.get_size().size() == 3 ) {// the image is 3D
    
    if ( 1 != 
	 lfiltreordre3d( &(* result ), // '&(*result)' == 'result->operator&' and returns an 'xvimage*'
			 &(* non_const_structuring_element ),
			 non_const_structuring_element->get_center()[0], 
			 non_const_structuring_element->get_center()[1],
			 non_const_structuring_element->get_center()[2],
			 0.5
	   ) 
      )     
      error("medianfilter: lfiltreordre failed");
    
  } else { // the image is 4D or bad
    
    error("erosball: only 2D and 3D images are supported");
    
  };

  
  
  
  return result;
};




void py_medianfilter_export(){

  def( "medianfilter", &py_medianfilter,
       args("image", "structuring_element"),
       "pink's help: \n"
       "Detailed Description \n"
       "median filter \n"
       " \n"
       "Usage: medianfilter in.pgm el.pgm out.pgm \n"
       " \n"
       "Description: Let F be the input image, G be the output image, and E the structuring element. then for each pixel p, G[p] is the median element of the sorted list (by increasing order) of the pixel values in the set { F[q], q in E[p] }. \n"
       " \n"
       "The median filter is a particular case of rank filter (see rankfilter.c), with rank = 0.5 . \n"
       " \n"
       "Types supported: byte 2d, byte 3d \n"
       " \n"
       "Category: morpho \n"
    );

};


















































// LuM end of file
