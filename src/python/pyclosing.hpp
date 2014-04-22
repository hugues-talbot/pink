/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/

#ifndef PYCLOSING__HPP__
#define PYCLOSING__HPP__

#include "pyexport.hpp"

namespace pink {
  namespace python {
    boost::python::object closing ( const boost::python::object & image, const boost::python::object & elem );
  } // namespace python
} // namespace pink

#endif /* PYCLOSING__HPP__ */

// LuM end of file
