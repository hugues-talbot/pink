/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyreadraw.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    image_type read_raw_image(
      const string & filename,
      const boost::python::list & python_dim
      )
    {
  
      typedef typename image_type::pixel_type pixel_type;

      vint dim(python_dim);
  
      ifstream file;
  
      if ( file_size(filename) != sizeof(pixel_type) * dim.prod() )
      {
        cout << "requested file size = " << sizeof(pixel_type) * dim.prod() << "\n";
        cout << "but '" << filename << "' is only " << file_size(filename) << " byte-long\n"; 
        error( "the file size does not equal with the image size");
      } /* if */

      file.open( filename.c_str(), ios_base::binary );
  
      ARRAY<pixel_type> data(new pixel_type[dim.prod()]);
    
      try 
      {    
        file.read( 
          reinterpret_cast<char*>(data.get()), 
          sizeof(pixel_type)*dim.prod()
          );
      }
      catch (...)
      {
        error("the raw data couldn't be read from the file");
      }
  

      file.close();
  
      image_type result( dim, data );

      return result;
  
    } /* read_raw_image */

  } /* namespace python */
} /* namespace pink */



UI_EXPORT_FUNCTION( read_raw_image, 
                    pink::python::read_raw_image, 
		    ( arg("filename"), arg("dimensions of the image") ),
		    "this function reads a raw image from a file the"
		    " image type is specified by the suffix and the "
		    "size is given in the 'dim' argument\n"
  );



// LuM end of file
