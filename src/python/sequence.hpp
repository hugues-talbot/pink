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

#ifndef SEQUENCE__HPP__
#define SEQUENCE__HPP__

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

namespace pink {
  namespace tmp {

    using boost::mpl::if_;
    using boost::mpl::bool_;
    
        
    template<int...>
    struct sequence_t { };
    
    template<int N, int...S>
    struct generate_sequence : generate_sequence<N-1, N-1, S...> { };
    
    template<int...S>
    struct generate_sequence<0, S...> {
      typedef sequence_t<S...> type;
    }; // generate_sequence


    template<int from, int to, int...S>
    struct generate_range {
      typedef typename
      if_< bool_<from==to>,
           sequence_t<S...>,
           generate_range<from, to-1, to-1, S...>
           >::type type;
    }; // generate_range
    
      
    
  } // namespace tmp
} // namespace pink

#endif /* SEQUENCE__HPP__ */



// LuM end of file
