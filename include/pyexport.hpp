/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/**   \file pyexport.hpp

      \ingroup development

      \brief This file defines the macros, which facilitate the export
      of functions in Python. The emphasis is given to multiple
      inclusion of template functions with all different types. As
      python cannot 'compile' in runtime, you must compile the
      template functions with all the possible image types. The macros
      and templates in this file ease you do that.
*/

// boost python wrapper
// This macro exports a template function to all possible image types
#ifndef PYEXPORT_HPP
#define PYEXPORT_HPP

#include "ujimage.hpp"

namespace pink {
  

  const xvimage *
  can_be_null( const pink::cxvimage & image );


} // namespace pink






#endif /* PYEXPORT_HPP */
// LuM end of file
