/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// Pink NumPy Conversion routine

#include <boost/python.hpp>

#include "ujimage.hpp"
#include "pynumpy.hpp"

using boost::python::arg;
using boost::python::def;


namespace pink {
  namespace python {

    // part of this code has been borrowed from
    // Copyright (c) 2008, Michael Droettboom
#   ifdef PINK_HAVE_NUMPY

    boost::python::object
    numpy2pink( boost::python::object & array )
    {
      if (! PyArray_Check(array.ptr())) // verifying that the object is indeed a numpy array
          pink_error("This function can only convert numpy arrays");
      
      PyArrayObject* original_array = reinterpret_cast<PyArrayObject*>(array.ptr());
      
      if (PyArray_TYPE(original_array) == NPY_UINT8) {
        char_image image = detail::numpy2pink<char_image>(array);
        return boost::python::object(image);        
      }      
      else if (PyArray_TYPE(original_array) == NPY_UINT16) {        
        short_image image = detail::numpy2pink<short_image>(array);
        return boost::python::object(image);
      }
      else if (PyArray_TYPE(original_array) == NPY_INT32) {
        int_image image = detail::numpy2pink<int_image>(array);
        return boost::python::object(image);
      }
      else if (PyArray_TYPE(original_array) == NPY_FLOAT) {
        float_image image = detail::numpy2pink<float_image>(array);
        return boost::python::object(image);
      }
      else if (PyArray_TYPE(original_array) == NPY_DOUBLE) {
        double_image image = detail::numpy2pink<double_image>(array);
        return boost::python::object(image);
      }
      else
        pink_error("Cannot convert into Pink Image. The type of the array is not supported in Pink");        

      return boost::python::object();      
    } // numpy2pink

    
#   endif /* PINK_HAVE_NUMPY */
  } /* namespace python */
} /* namespace pink */


const char * wrap2numpy_doc =
                     "Converts a pink::ujimage object to a numpy array. The data is NOT copied"
                     ", only the pointer is carried, which means that if you change the numpy "
                     "array the image values change too. In exchange the conversion does no copy and "
                     "is therefore in constant time.";

const char * pink2numpy_doc =
                     "Converts a pink::ujimage object to a numpy array. The data IS copied. If You would like a shallow copy, use wrap2numpy.";

const char * numpy2pink_doc =
                     "Converts a pink::ujimage object to a numpy array. The data IS copied.";

using pink::python::wrap2numpy;
using pink::python::pink2numpy;
using pink::python::numpy2pink;
                             
void numpy_export()
{
# ifdef PINK_HAVE_NUMPY
  def( "wrap2numpy", &wrap2numpy<pink::char_image>,     ( arg("image") ), wrap2numpy_doc );
  def( "wrap2numpy", &wrap2numpy<pink::short_image>,    ( arg("image") ), wrap2numpy_doc );
  def( "wrap2numpy", &wrap2numpy<pink::int_image>,      ( arg("image") ), wrap2numpy_doc );
  def( "wrap2numpy", &wrap2numpy<pink::float_image>,    ( arg("image") ), wrap2numpy_doc );
  def( "wrap2numpy", &wrap2numpy<pink::double_image>,   ( arg("image") ), wrap2numpy_doc );
//  def( "wrap2numpy", &wrap2numpy<pink::fcomplex_image>, ( arg("image") ), wrap2numpy_doc );
//  def( "wrap2numpy", &wrap2numpy<pink::dcomplex_image>, ( arg("image") ), wrap2numpy_doc );

  def( "numpy2pink", &numpy2pink, ( arg("array") ), numpy2pink_doc );

  def( "pink2numpy", &pink2numpy<pink::char_image>,     ( arg("image") ), pink2numpy_doc );
  def( "pink2numpy", &pink2numpy<pink::short_image>,    ( arg("image") ), pink2numpy_doc );
  def( "pink2numpy", &pink2numpy<pink::int_image>,      ( arg("image") ), pink2numpy_doc );
  def( "pink2numpy", &pink2numpy<pink::float_image>,    ( arg("image") ), pink2numpy_doc );
  def( "pink2numpy", &pink2numpy<pink::double_image>,   ( arg("image") ), pink2numpy_doc );
//  def( "pink2numpy", &pink2numpy<pink::fcomplex_image>, ( arg("image") ), pink2numpy_doc );
//  def( "pink2numpy", &pink2numpy<pink::dcomplex_image>, ( arg("image") ), pink2numpy_doc );
  
  import_array();  // numpy initialization
# endif /* PINK_HAVE_NUMPY */
} // numpy_export 

// LuM end of file
