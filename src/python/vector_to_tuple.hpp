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

    template <class vector_t, class sequence_t>
    struct select;

    template <class vector_t, int...ARGS>
    struct select< vector_t, sequence_t<ARGS...> > {
      typedef typename std::tuple< typename boost::mpl::at_c<vector_t, ARGS>::type... > type;
    }; // struct select
  
    template <class vector_t>
    struct vector_to_tuple {
      typedef typename generate_sequence< boost::mpl::size<vector_t>::value >::type sequence;
      typedef typename select<vector_t, sequence>::type type;
    }; // struct vector_to_tuple

    template <class tuple_t>
    struct tuple_to_vector;

    template <class...ARGS>
    struct tuple_to_vector<std::tuple<ARGS...> > {
      typedef boost::mpl::vector<ARGS...> type;      
    }; // tuple_to_vector


    template <class result_type, class tuple_t>
    struct push_result_front;

    template <class result_type, class...ARGS>
    struct push_result_front<result_type, std::tuple<ARGS...> > {
      typedef boost::mpl::vector<result_type, ARGS...> type;      
    }; // tuple_to_vector

  } // namespace tmp
} // namespace pink

#endif /* VECTOR_TO_TUPLE__HPP__ */

// LuM end of file
