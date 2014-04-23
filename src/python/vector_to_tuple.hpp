/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// This file converts boost::mpl::vector to std::tuple
// using template metaprogramming

#ifndef VECTOR_TO_TUPLE__HPP__
#define VECTOR_TO_TUPLE__HPP__

#include "sequence.hpp"

#include <tuple>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>

namespace pink {
  namespace tmp {

    using std::tuple;
    using boost::mpl::at_c;
    using boost::mpl::vector;
        
    template <class vector_t, class sequence_t>
    struct select;

    template <class vector_t, int...ARGS>
    struct select< vector_t, sequence_t<ARGS...> > {
      typedef tuple< typename at_c<vector_t, ARGS>::type... > type;
    }; // struct select
  
    template <class vector_t>
    struct vector_to_tuple {
      typedef typename generate_sequence< boost::mpl::size<vector_t>::value >::type sequence;
      typedef typename select<vector_t, sequence>::type type;
    }; // struct vector_to_tuple

    template <class tuple_t>
    struct tuple_to_vector;

    template <class...ARGS>
    struct tuple_to_vector<tuple<ARGS...> > {
      typedef boost::mpl::vector<ARGS...> type;      
    }; // tuple_to_vector


    template <class result_type, class tuple_t>
    struct push_result_front;

    template <class result_type, class...ARGS>
    struct push_result_front<result_type, tuple<ARGS...> > {
      typedef boost::mpl::vector<result_type, ARGS...> type;      
    }; // tuple_to_vector

    template <class V1, class V2>
    struct concat;

    template <class...V1_ARGS, class...V2_ARGS>
    struct concat< tuple<V1_ARGS...>, tuple<V2_ARGS...> > {
      typedef tuple<V1_ARGS..., V2_ARGS...> type;
    }; // struct concat 

    template <class V1, class V2>
    struct vconcat {
      typedef typename tuple_to_vector< typename concat< typename vector_to_tuple<V1>::type, typename vector_to_tuple<V2>::type >::type >::type type;
    }; // struct vconcat
        
    
    template <class tuple_t>
    struct remove_first;

    template <class FIRST, class...REST>
    struct remove_first< tuple<FIRST, REST...> > {
      typedef tuple<REST...> type;
    }; // struct remove_first
        
    template <class tuple_t>
    struct remove_last;

    template <class...ARGS>
    struct remove_last< tuple<ARGS...> > {
      const static int size = sizeof...(ARGS);
      typedef typename select< vector<ARGS...>, typename generate_sequence<size-1>::type >::type type;
    }; // struct remove_last
          
    
  } // namespace tmp
} // namespace pink

#endif /* VECTOR_TO_TUPLE__HPP__ */

// LuM end of file
