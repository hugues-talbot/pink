/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// This file generates variadic C++ sequences

#ifndef PYEXPORT__HPP__
#define PYEXPORT__HPP__

#include <tuple>
#include <Python.h>
#include <boost/mpl/at.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/python/tuple.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/python/object.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/placeholders.hpp>

#include "ujimage.hpp"
#include "vector_to_tuple.hpp"

namespace pink {

  template <class T0>
  const xvimage *
  can_be_null( const T0 & image ) {
    if (image.isnull())
      return NULL;        
    else /* NOT image.isnull() */
      return image;        
  } // can be null
    
  namespace tmp {

    using boost::mpl::at;    
    using boost::mpl::if_;
    using boost::mpl::map;
    using boost::mpl::pair;
    using boost::mpl::has_key;    
    using boost::mpl::transform;
    using boost::mpl::placeholders::_1;
    
            
    typedef map<
      pair<xvimage*,             boost::python::object>,
      pair<const xvimage*, const boost::python::object>,
      pair<cxvimage,             boost::python::object>,
      pair<const cxvimage, const boost::python::object>
      > mapping_to_python;
    
    typedef map<
      pair<boost::python::object, cxvimage>,
      pair<const boost::python::object, const cxvimage>
      > mapping_to_pink;

    typedef map<
      pair< xvimage*,       boost::python::object>,
      pair< cxvimage,       boost::python::object>,
      pair< char_image,     boost::python::object>,
      pair< short_image,    boost::python::object>,
      pair< int_image,      boost::python::object>,
      pair< float_image,    boost::python::object>,
      pair< double_image,   boost::python::object>,
      pair< fcomplex_image, boost::python::object>,
      pair< dcomplex_image, boost::python::object>
      > result_mapping;

    template<class map_t, class source_t>
    struct convert_if {
      typedef typename
      if_< has_key<map_t, source_t>,
           /* then */ typename at<map_t, source_t>::type,
           /* else */ source_t // we leave the type unchanged
           >::type type;  
    }; // convert_if

    template <class source_t>
    using convert_to_python = convert_if<mapping_to_python, source_t>;

    template <class source_t>
    using convert_to_pink   = convert_if<mapping_to_pink, source_t>;

    template <class source_t>
    using convert_result    = convert_if<result_mapping, source_t>;
    
    template <template <class input_t> class convertor_t, class input_t>
    typename convertor_t<input_t>::type
    convert( input_t python_input ) {
      return python_input;      
    }
    
    template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
    struct wrapper_t;
    
    template <class result_type, class...PINK_ARGS, class...PYTHON_ARGS>
    struct wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> > {
      
      typedef result_type (*Fn)( PINK_ARGS... );

      Fn m_fn;
      
      wrapper_t(Fn fn) : m_fn(fn){ }      
    }; // struct wrapper_t

    /**
       Allocwrapper is used for operators, which allocate a new image
       for the return.
     */
    template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
    struct allocwrapper_t;
        
    template <class result_type, class...PINK_ARGS, class...PYTHON_ARGS>
    struct allocwrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >
      : wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >
    {
      // template<class Fn, class First, class...REST>
      // static xvimage*
      // caller( Fn fn, First first, REST...rest ) {
      //   cxvimage python_image(first);
      //   cxvimage result = python_image.clone();
        
      //   return fn( result, convert<convert_to_pink>(rest)...);
      // }
      typedef typename wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >::Fn Fn;
      typedef          wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> > base_t;
            
      allocwrapper_t( Fn fn ) : base_t(fn) { }
      
      boost::python::object
      operator () ( PYTHON_ARGS...args ) {
        cxvimage result(this->m_fn( convert<convert_to_pink>(args)...));
        return result.steel();        
      }
      
    }; // struct allocwrapper_t


    /**
       Exportwrapper is a class which wraps operators without a result
       type and without an altered image as results. These are
       typically the exporting functions, such as writeimage.       
    */
    template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
    struct exportwrapper_t;
        
    template <class result_type, class...PINK_ARGS, class...PYTHON_ARGS>
    struct exportwrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >
      : wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >
    {
      // template<class Fn, class First, class...REST>
      // static xvimage*
      // caller( Fn fn, First first, REST...rest ) {
      //   cxvimage python_image(first);
      //   cxvimage result = python_image.clone();
        
      //   return fn( result, convert<convert_to_pink>(rest)...);
      // }
      typedef typename wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >::Fn Fn;
      typedef wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> > base_t;
            
      exportwrapper_t( Fn fn ) : base_t(fn) { }
      
      result_type
      operator () ( PYTHON_ARGS...args ) {
        return this->m_fn( convert<convert_to_pink>(args)...);
      }
      
    }; // struct exportwrapper_t

    
    
    template <class result_type, class...ARGS>
    struct caller_t {
      typedef result_type(*Fn)(ARGS...);
      static const std::size_t arity = sizeof...(ARGS);
  
      typedef typename std::tuple<ARGS...> tuple_pink_t;
      typedef typename boost::mpl::vector<ARGS...> vector_pink_t;
      typedef typename boost::mpl::transform< boost::mpl::vector<ARGS...>, convert_to_python<_1> >::type vector_python_t;
      typedef typename vector_to_tuple<vector_python_t>::type tuple_python_t;  
  
    }; // struct caller_t 

    template < template <class result_type, class T1, class T2> class wrapper_class,
               class result_type,
               class...ARGS >
    wrapper_class< result_type,
                   typename caller_t<result_type, ARGS...>::tuple_pink_t,
                   typename caller_t<result_type, ARGS...>::tuple_python_t >
    wrap( result_type(*fn)(ARGS...) ) {
      typedef typename caller_t<result_type, ARGS...>::tuple_python_t tuple_python_t;
      typedef typename caller_t<result_type, ARGS...>::tuple_pink_t   tuple_pink_t;
      
      return wrapper_class<result_type, tuple_pink_t, tuple_python_t>(fn);
    } // wrap
    
    // template <class result_type, class...ARGS>
    // exportwrapper_t< typename caller_t<result_type, ARGS...>::tuple_pink_t,
    //                  typename caller_t<result_type, ARGS...>::tuple_python_t >
    // wrap_export( result_type(*fn)(ARGS...) ) {
    //   typedef typename caller_t<result_type, ARGS...>::tuple_python_t tuple_python_t;
    //   typedef typename caller_t<result_type, ARGS...>::tuple_pink_t   tuple_pink_t;
      
    //   return exportwrapper_t<tuple_pink_t, tuple_python_t>(fn);
    // } // wrap_export
   
  } // namespace tmp
} // namespace pink


namespace boost {
  namespace python {
    namespace detail {

      using boost::python::object;
      using pink::tmp::allocwrapper_t;
      using pink::tmp::exportwrapper_t;
      using pink::tmp::push_result_front;      
                  
      template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
      typename push_result_front< object, PYTHON_TUPLE >::type
      get_signature( allocwrapper_t<result_type, PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename push_result_front< object, PYTHON_TUPLE >::type result_t;

        return result_t();
      }

      template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
      typename push_result_front< result_type, PYTHON_TUPLE >::type
      get_signature( exportwrapper_t<result_type, PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename push_result_front< result_type, PYTHON_TUPLE >::type result_t;

        return result_t();
      }

      
    } // namespace detail
  } // namespace python
} // namespace boost

#ifdef _WINDOWS
# define BOOST_PYTHON_STATIC_LIB
#endif /* _WINDOWS */
#include <boost/python.hpp>


namespace pink {
  

  template <class T0, class Fn, class...ARGS >
  void
  allocdef( T0 t0, Fn fn, ARGS...args ) {
    
    auto wrapper = tmp::wrap<tmp::allocwrapper_t>(fn);
    
    boost::python::def( t0, wrapper, args... );
    return;
  }

  template <class T0, class Fn, class...ARGS >
  void
  exportdef( T0 t0, Fn fn, ARGS...args ) {
    
    auto wrapper = tmp::wrap<tmp::exportwrapper_t>(fn);
    
    boost::python::def( t0, wrapper, args... );
    return;
  }
  
} // namespace pink
  


#endif /* PYEXPORT__HPP__ */



// LuM end of file
