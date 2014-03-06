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

namespace pink {
  namespace tmp {

    template<int...>
    struct sequence_t { };
    
    template<int N, int...S>
    struct generate_sequence : generate_sequence<N-1, N-1, S...> { };
    
    template<int...S>
    struct generate_sequence<0, S...> {
      typedef sequence_t<S...> type;
    }; // generate_sequence
    
  } // namespace tmp
} // namespace pink

#endif /* SEQUENCE__HPP__ */



// LuM end of file
