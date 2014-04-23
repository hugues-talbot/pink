/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include "pyexport.hpp"

#include <fstream>
#include <iostream>

#include "uiFileIO.hpp"


namespace pink {
  namespace python {


    boost::python::object
    py_read_raw_image (
      const std::string & filename,
      const boost::python::list & python_dim,
      const int inttype
      )
    {
      
      std::vector<index_t> dim;

      // converting the python list into std::vector
      for ( index_t coord = 0; coord<boost::python::len(python_dim); coord++ ) {
        try {
          dim.push_back( boost::python::extract<index_t>(python_dim[coord]) );
        }
        catch (...)
        {
          pink_error("the elements of the list must be convertible to integers.");
        }
      } // for coord
      
      std::ifstream file;
  
      if ( file_size(filename) != pink::typesize(inttype) * pink::prod(dim) )
      {
        std::cout << "requested file size = " << pink::typesize(inttype) * pink::prod(dim) << "\n";
        std::cout << "but '" << filename << "' is only " << file_size(filename) << " byte-long\n"; 
        pink_error( "the file size does not equal with the image size");
      } /* if */

      file.open( filename.c_str(), std::ios_base::binary );

      cxvimage image( inttype, dim );
      try {
        file.read(
          image.pdata<char>(),
          pink::typesize(inttype) * pink::prod(dim) );
      }
      catch (...)
      {
        pink_error("the raw data couldn't be read from the file");
      }
  
      boost::python::object result(image);
      
      return result;      
    } /* py_read_raw_image */
    

  } /* namespace python */
} /* namespace pink */

using boost::python::arg;
using boost::python::def;


void read_raw_image_export()
{
  def(
    "read_raw_image",
    pink::python::py_read_raw_image, 
    ( arg("filename"), arg("dimensions of the image"), arg("image mode") = VFF_TYP_1_BYTE),
    "this function reads a raw image from a file the"
    " image type is specified by the suffix and the "
    "size is given in the 'dim' argument\n" );
  
}




// LuM end of file
