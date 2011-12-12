/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// Wrapper file for ex-LIAR stuff.
// pypink.cpp was beginning to be too big

#include "pink_python.h"
#include "liar_fseries.h"
#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using namespace boost::python;

// using "modern" wrapping

// here we use namespaces, so the export_functions
// would not interfere with each other.
// c++ is a cool language, ain't it?

namespace liar
{

  UI_WRAP_RESULT(
    char_image,
    "imferode3d_rect",
    imferode3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast erosion by a parallelepiped."
    );
# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "feroderect",
    imferode_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

   # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "fopenrect",
    imfopen_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

    # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "fdilaterect",
    imfdilat_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

    # include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "fcloserect",
    imfclose_rect,
    (arg("input"), arg("SEnx"), arg("SEny")),
    "This performs a fast erosion by a rectangle."
        );

    
# include BOOST_PP_UPDATE_COUNTER()
    
  
  UI_WRAP_RESULT(
    char_image,
    "ferode3drect",
    imferode3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast erosion by a parallelepiped."
      );
# include BOOST_PP_UPDATE_COUNTER()
    
    UI_WRAP_RESULT(
    char_image,
    "fdilate3drect",
    imfdilat3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast dilation by a parallelepiped."
          );
    
# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "fopen3drect",
    imfopen3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast opening by a parallelepiped."
        );

# include BOOST_PP_UPDATE_COUNTER()

    UI_WRAP_RESULT(
    char_image,
    "fclose3drect",
    imfclose3D_rect,
    (arg("input"), arg("SEnx"), arg("SEny"), arg("SEnz")),
    "This performs a fast closing by a parallelepiped."
      );
# include BOOST_PP_UPDATE_COUNTER()
    
} /* namespace liar */

using namespace liar;

void pyliar()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}
