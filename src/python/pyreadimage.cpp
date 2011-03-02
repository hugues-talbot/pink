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

#include <pink_python.h>

/// !!! macros should be named upper case
#undef border

using namespace boost::python;

namespace pink {
  namespace python {

    
    boost::python::object py_readimage( std::string filename )
    {
      boost::python::object * result;
      
      
      xvimage * tmp=NULL;
      tmp=readimage( const_cast<char*>(filename.c_str())); // reading the image
      
      if (tmp==NULL)
      {
        pink_error("error reading file: '" + filename + "'" );
      }

      // determining the image type and creating
      // the appropriate python object
      switch (tmp->data_storage_type)
      {        
      case VFF_TYP_1_BYTE: /* pixels are byte (uint8_t) */
      {
        char_image a(*tmp);
        result = new boost::python::object(a);
      }      
      break;
        
      case VFF_TYP_2_BYTE: /* pixels are two byte (int16_t) */
      {        
        short_image a(*tmp);        
        result = new boost::python::object(a);
      }      
      break;
        
      case VFF_TYP_4_BYTE: /* pixels are four byte (integer) */
      {        
        int_image a(*tmp);        
        result = new boost::python::object(a);
      }
      break;
        
      case VFF_TYP_FLOAT: /* pixels are float (single precision) */
      {        
        float_image a(*tmp);        
        result = new boost::python::object(a);
      }
      break;
        
      case VFF_TYP_DOUBLE: /* pixels are float (double precision)*/
      {        
        double_image a(*tmp);        
        result = new boost::python::object(a);
      }
      break;

      case VFF_TYP_COMPLEX: /* pixels are float (single precision) */
      {        
        fcomplex_image a(*tmp);        
        result = new boost::python::object(a);
      }
      break;
        
      case VFF_TYP_DCOMPLEX: /* pixels are float (double precision)*/
      {        
        dcomplex_image a(*tmp);        
        result = new boost::python::object(a);
      }
      break;
      
      default:
        pink_error("c_readimage returned bad image type");
      } /* switch im_type */
      
      // freeimage( tmp ); // tmp had become result, so we don't free it. it will be freed by smart ptr      
      return *result;
    } /* py_readimage */
    
  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION (
  readimage,
  pink::python::py_readimage,
  ( arg("filename") ),
  "reads an image from a 'pgm' file and returns an object with the appropriate "
  "type"  
  );






// LuM end of file
