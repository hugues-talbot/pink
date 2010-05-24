// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pylong2byte.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_long2byte( const int_image & src, int mode, int nbnewval ){

  PTR<int_image> local_copy( new int_image(src) );
  
  xvimage * res = long2byte( &(*local_copy), mode, nbnewval ); // '&(*local_copy)' == 'local_copy->operator&' and returns an 'xvimage*'

  PTR<char_image> result( new char_image(*res) );
  freeimage(res);


  return result;    
};



void py_long2byte_export(){

  def( "cpp_long2byte", &py_long2byte, 
       args("image","mode","nb_new_val"),
       "converts a 'int32_t' image to a 'byte' image \n"
       "\n"
       "Usage: long2byte in.pgm [mode] [n] out.pgm\n"
       "\n"
       "Description:\n"
       "\n"
       "Depending on the value given for the (optional) parameter mode:\n"
       " *  mode = 0 (default) : for all x, out[x] = min(255, in[x]).\n"
       " *  mode = 1 : for all x, out[x] = in[x] modulo 256.\n"
       " *  mode = 2 : scales values in the range 0-255.\n"
       " *  mode = 3 : sorts the values by decreasing number of occurence in the image.\n"
       "Replaces the values by their order.\n"
       "Only the n (default 255) first values are kept.\n"
       "Useful for label images.\n"
       " *  mode = 4 : truncation of the square root in the range 0-255.\n"
       " *  mode = 5 : truncation of the log in the range 0-255.\n"
       " \n"
       "Types supported: int32_t 2d, int32_t 3d\n"
       "\n"
       "Category: convert\n"
       "\n"
       "author Michel Couprie\n"
    );


};


















































// LuM end of file
