/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/

#ifndef PYASFBIN__HPP__
#define PYASFBIN__HPP__

#include "pyexport.hpp"

namespace pink {
  namespace python {
    
    char_image asfbin( const char_image & image, const index_t & max_radius, const index_t & min_radius = 1 );
    
  } // namespace python
} // namespace pink

#endif /* PYASFBIN__HPP__ */
// LuM end of file
