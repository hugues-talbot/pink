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

  template<class TO>
  TO
  cppconvert( boost::python::object object ) {
    pink_error("invalid conversion (1012).")
  }
    
  namespace tmp {

    using boost::mpl::at;    
    using boost::mpl::if_;
    using boost::mpl::map;
    using boost::mpl::pair;
    using boost::mpl::has_key;    
    using boost::python::object;
    using boost::mpl::transform;
    using boost::mpl::placeholders::_1;
    
            
    typedef map<
      pair<xvimage*,       object>,
      pair<const xvimage*, object>,
      pair<cxvimage,       object>,
      pair<const cxvimage, object>
      > mapping_to_python;
    
    typedef map<
      pair<object, cxvimage>
      > mapping_to_pink;
    

    typedef map<
      pair< xvimage*,       object>,
      pair< cxvimage,       object>,
      pair< char_image,     object>,
      pair< short_image,    object>,
      pair< int_image,      object>,
      pair< float_image,    object>,
      pair< double_image,   object>,
      pair< fcomplex_image, object>,
      pair< dcomplex_image, object>
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
      typedef typename wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >::Fn Fn;
      typedef          wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> > base_t;
            
      allocwrapper_t( Fn fn ) : base_t(fn) { }
      
      object
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
      typedef typename wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> >::Fn Fn;
      typedef wrapper_t< result_type, std::tuple<PINK_ARGS...>, std::tuple<PYTHON_ARGS...> > base_t;
            
      exportwrapper_t( Fn fn ) : base_t(fn) { }
      
      result_type
      operator () ( PYTHON_ARGS...args ) {
        return this->m_fn( convert<convert_to_pink>(args)...);
      }
      
    }; // struct exportwrapper_t


    /**
       Resultwrapper wraps the functions, where the result is added
       into the final variable.
    */
    template <class PINK_TUPLE, class PYTHON_TUPLE>
    struct resultwrapper_t;
    
    template <class...PINK_ARGS, class...PYTHON_ARGS >
    struct resultwrapper_t< std::tuple<PINK_ARGS...>,
                            std::tuple<PYTHON_ARGS...> >
    {
      typedef int32_t (*Fn) ( PINK_ARGS... );
      
      Fn m_fn;
      int32_t m_type;
            
      resultwrapper_t( Fn fn, int32_t type ) : m_fn(fn), m_type(type) { }
            
      object
      operator () ( PYTHON_ARGS...args ) {
        const cxvimage input_image( std::get<0>(std::make_tuple(args...)) );
        cxvimage result = cxvimage( m_type, input_image.size() );
        
        if (! m_fn( convert<convert_to_pink>(args)..., result ) )
          pink_error("The Pink operator failed!");

        return result.steel();
      } // operator()
      
    }; // struct resultwrapper_t

    /**
       Functionwrapper is the most common wrapper. The first variable
       is cloned and in the end the result is copied to the cloned
       variable.
    */
    template <class PINK_TUPLE, class PYTHON_TUPLE>
    struct functionwrapper_t;
    
    template <class...PINK_ARGS, class...PYTHON_ARGS >
    struct functionwrapper_t< std::tuple<PINK_ARGS...>,
                              std::tuple<PYTHON_ARGS...> >
    {
      typedef int32_t (*Fn) ( PINK_ARGS... );

      Fn m_fn;
            
      functionwrapper_t( Fn fn ) : m_fn(fn) { }
            
      object
      operator () ( object pyimage, PYTHON_ARGS...args ) {
        const cxvimage input(pyimage);
        cxvimage result = input.clone();
        if (! m_fn( result, convert<convert_to_pink>(args)...) )
          pink_error("The Pink operator failed!");

        return result.steel();
      } // operator()
      
    }; // struct functionwrapper_t

    
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
      typedef typename caller_t<result_type, ARGS...>::tuple_pink_t   tuple_pink_t;
      typedef typename caller_t<result_type, ARGS...>::tuple_python_t tuple_python_t;

      
      return wrapper_class<result_type, tuple_pink_t, tuple_python_t>(fn);
    } // wrap

    template <class result_type, class...ARGS>
    resultwrapper_t< typename caller_t<result_type, ARGS...>::tuple_pink_t,
                     typename remove_last<typename caller_t<result_type, ARGS...>::tuple_python_t>::type >
    wrap_result ( result_type (*fn) ( ARGS...), int32_t type ) {
      typedef typename caller_t<result_type, ARGS...>::tuple_pink_t tuple_pink_t;
      typedef typename remove_last<typename caller_t<result_type, ARGS...>::tuple_python_t>::type tuple_python_t;
            
      return resultwrapper_t<tuple_pink_t, tuple_python_t>( fn, type );      
    } // wrap_result
    
    template <class result_type, class...ARGS>
    functionwrapper_t< typename caller_t<result_type, ARGS...>::tuple_pink_t,
                       typename remove_first<typename caller_t<result_type, ARGS...>::tuple_python_t >::type >
    wrap_function( result_type (*fn) (ARGS...) ) {
      return  functionwrapper_t< typename caller_t<result_type, ARGS...>::tuple_pink_t,
                                 typename remove_first<typename caller_t<result_type, ARGS...>::tuple_python_t >::type > (fn);
    } // wrap_function 
    
    
  } // namespace tmp
} // namespace pink


namespace boost {
  namespace python {
    namespace detail {

      using pink::tmp::vconcat;
      using boost::mpl::vector;
      using boost::python::object;
      using pink::tmp::allocwrapper_t;
      using pink::tmp::exportwrapper_t;
      using pink::tmp::resultwrapper_t;
      using pink::tmp::tuple_to_vector;
      using pink::tmp::functionwrapper_t;
      using pink::tmp::push_result_front;
      using pink::tmp::convert_to_python;
                  
      template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
      typename vconcat< vector<object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type
      get_signature( allocwrapper_t<result_type, PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename vconcat< vector<object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type result_t;

        return result_t();
      }

      template <class result_type, class PINK_TUPLE, class PYTHON_TUPLE>
      typename vconcat< vector<result_type>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type
      get_signature( exportwrapper_t<result_type, PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename vconcat< vector<result_type>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type result_t;

        return result_t();
      }

      template <class PINK_TUPLE, class PYTHON_TUPLE>
      typename vconcat< vector<object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type
      get_signature( resultwrapper_t<PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename vconcat< vector<object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type result_t;

        return result_t();
      }

      template <class PINK_TUPLE, class PYTHON_TUPLE>
      typename vconcat< vector<object, object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type
      get_signature( functionwrapper_t<PINK_TUPLE, PYTHON_TUPLE>& )
      {
        typedef typename vconcat< vector<object, object>, typename tuple_to_vector<PYTHON_TUPLE>::type >::type result_t;

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
  } // allocdef

  template <class T0, class Fn, class...ARGS >
  void
  exportdef( T0 t0, Fn fn, ARGS...args ) {
    
    auto wrapper = tmp::wrap<tmp::exportwrapper_t>(fn);
    
    boost::python::def( t0, wrapper, args... );
    return;
  } // exportdef

  template <class T0, class Fn, class...ARGS >
  void
  resultdef( int32_t type, T0 t0, Fn fn, ARGS...args ) {
    
    auto wrapper = tmp::wrap_result(fn, type);
    
    boost::python::def( t0, wrapper, args... );
    
    return;
  } // resultdef

  template <class T0, class Fn, class...ARGS >
  void
  functiondef( T0 t0, Fn fn, ARGS...args ) {
    
    auto wrapper = tmp::wrap_function(fn);
    
    boost::python::def( t0, wrapper, args... );
    
    return;
  } // functiondef
  
} // namespace pink
  


#endif /* PYEXPORT__HPP__ */



// LuM end of file
