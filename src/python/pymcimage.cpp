/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>

using namespace boost::python;
using namespace pink;

// PTR<deep_xvimage> py_readimage( string filename ){
//   // readimage takes 'char *', while 'c_str()' gives 'const char *'. 
//   // I don't want to cast unnecesserily, so I'll just copy it.

//   char * cstr = new char [filename.size()+1]; // converting the filename for 'readimage'
//   strcpy ( cstr, filename.c_str() );

//   xvimage * tmp = readimage( cstr ); // reading the image
  
//   PTR< deep_xvimage > result( new deep_xvimage( *tmp ) ); // upcast deep copying the xvimage

//   freeimage( tmp ); // freeing xvimage

//   return result;
// };


// void py_writeimage( xvImage & image, string filename ){
//   char * cstr = new char [filename.size()+1];
//   strcpy (cstr, filename.c_str());
//   writeimage( &image, cstr );
// };


void writeimage(
  struct xvimage * image,
  char *filename
  );


void py_readimage_export(){
  def("cpp_readimage", py_readimage, 
      args("filename"), 
      "Reads an image from filename and returns an 'xvimage' object."
    );
};

// void py_writeimage_export(){
//   def("cpp_writeimage", py_writeimage, 
//       args("image", "filename"), 
//       "Write the xvimage object to a pgm file."
//     );
// };




  




  































// LuM end of file
