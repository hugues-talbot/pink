/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by aplicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// Wrapper file for ex-LIAR stuff.
// pypink.cpp was beginning to be too big

#ifndef PINK_DEVELOPMENT
#  include <pink_python.h>
#else /* PINK_DEVELOPMENT */
#  include <pink_development.h>
#endif /* PINK_DEVELOPMENT */

using namespace boost::python;
using namespace pink;

// maybe I'll get away with it
#include "pyujimage.hpp"
#include "pyexport.hpp"
#include "ui_wrap_function.hpp"
#include "ui_wrap_result.hpp"
#include "ui_wrap_value.hpp"
//#include "ui_wrap_value.hpp"
#include "ui_make_function.hpp"
#include "ui_make_result.hpp"
#include "ui_make_value.hpp"

//#include "python_doc.h"


//#include <boost/preprocessor.hpp>
//#include <pyexport.hpp>




// using "modern" wrapping


// #include <boost/preprocessor/slot/counter.hpp>

// #define BOOST_PP_COUNTER 1000

// UI_WRAP_RESULT(
//   char_image,
//   "imferode3d_rect",
//   imferode3D_rect,
//   (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
//   "This performs a fast erosion by a parallelepiped."
//   );
// #include BOOST_PP_UPDATE_COUNTER()

