/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2014 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/


// Pink NumPy Conversion


#ifdef PINK_HAVE_NUMPY
# include <Python.h>
# include <boost/cstdint.hpp>
# include <numpy/arrayobject.h>
# include <boost/smart_ptr.hpp>
#endif /* PINK_HAVE_NUMPY */

#include "ujimage.hpp"

#include <boost/python.hpp>

namespace pink {
  namespace python {

    namespace detail {

      template<class T>
      struct numpy_type_map {
        static const int typenum;
      };
      
    } // namespace detail

    boost::python::object numpy2pink( boost::python::object & array );

    template <class image_type>
    boost::python::object
    wrap2numpy( image_type & image )
    {
      // the dimension of the image
      index_t nd = image.size().size();
      // the sizes of the image
      boost::shared_array<npy_intp> dims( new npy_intp[nd] );            
      for ( index_t q=0; q<nd; ++q ) dims[q] = image.size()[q];
            
      PyObject* numpy_array;
      numpy_array =
        PyArray_SimpleNewFromData (
          nd,
          dims.get(),
          detail::numpy_type_map<typename image_type::pixel_type>::typenum,
          reinterpret_cast<void*>(image.get())
          );
      
      if (numpy_array == NULL)
        pink_error("Couldn't allocate the NumPy array.");

      boost::python::handle<> handle(numpy_array); // making boost handle the array as a smart pointer
      boost::python::object result(handle);
      return result;      
    }

        template <class image_type>
    boost::python::object
    pink2numpy( image_type & image )
    {
      // the dimension of the image
      index_t nd = image.size().size();
      // the sizes of the image
      boost::shared_array<npy_intp> dims( new npy_intp[nd] );            
      for ( index_t q=0; q<nd; ++q ) dims[q] = image.size()[q];
            
      PyObject* numpy_array;
      numpy_array =
        PyArray_SimpleNew (
          nd,
          dims.get(),
          detail::numpy_type_map<typename image_type::pixel_type>::typenum
          );

      if (numpy_array == NULL)
        pink_error("Couldn't allocate the NumPy array.");

      if (! PyArray_Check(numpy_array) )
        pink_error("Internal error during the conversion 01.");

      PyArrayObject * tmparray = reinterpret_cast<PyArrayObject*>(numpy_array);

      if (!PyArray_ISCONTIGUOUS(tmparray))
        pink_error("Internal error during the conversion 02.");      
       
      // copying the values to the numpy array
      typedef typename image_type::pixel_type type;      
      type * data = reinterpret_cast<type*>(PyArray_DATA(tmparray));
      
      std::copy( (&image(0)), (&image(pink::prod(image.size()))), data );      

      boost::python::handle<> handle(numpy_array); // making boost handle the array as a smart pointer
      boost::python::object result(handle);
      return result;      
    }


    template <class image_type>
    image_type
    numpy2pink_detail( boost::python::object & array )
    {
      if (! PyArray_Check(array.ptr())) // verifying that the object is indeed a numpy array
        pink_error("This function can only convert numpy arrays");

      PyArrayObject* original_array = reinterpret_cast<PyArrayObject*>(array.ptr());

      if ( PyArray_TYPE(original_array) != detail::numpy_type_map<typename image_type::pixel_type>::typenum )
        pink_error("Invalid image type. Conversion failed!");
            
      PyArrayObject* tmparray = PyArray_GETCONTIGUOUS(original_array); // making sure that the object is continuous; // if it is continuous than no copy is done here

      if (!PyArray_ISCONTIGUOUS(tmparray)) // the array is not continuous
        pink_error("Internal error, the array should be continuous by now.");

      npy_intp nd = PyArray_NDIM(tmparray);
      npy_intp * dims = PyArray_DIMS(tmparray);
      std::vector<index_t> dim( nd, 0 );

      FOR(q, nd) dim[q]=dims[q];

      typedef typename image_type::pixel_type pixel_type;
            
      if ( detail::numpy_type_map<pixel_type>::typenum == PyArray_TYPE(tmparray) ) // one more check to be sure
      {
        pixel_type * data = reinterpret_cast<pixel_type *>(PyArray_DATA(tmparray));
        image_type result(dim, data);
        return result;
      }
      else
        pink_error("Cannot convert into Pink Image. Internal error 03.");        

      return image_type();      
    } // numpy2pink
    

  } // namespace pink
} // namespace python



// LuM end of file
