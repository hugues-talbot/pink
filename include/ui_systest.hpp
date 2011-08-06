/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// memory speed test, parallel memory speed test
// computation power test, simd computational power test
// and parallel computational power test

#ifndef UI_SYSTEST_HPP___
#define UI_SYSTEST_HPP___

#include <iostream>

#include "pink.h"
#include "ui_simd.hpp"

namespace pink
{
  namespace benchmark
  {

    void memspeed();

#   ifdef PINK_HAVE_NUMA
    void numaspeed();
#   endif /* PINK_HAVE_NUMA */

        
  } /* namespace benchmark */
} /* namespace pink */


#endif /* UI_SYSTEST_HPP___ */

// LuM end of file
