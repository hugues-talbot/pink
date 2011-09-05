/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#define BOOST_DISABLE_ASSERTS
#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api

#include <iostream>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "pink.h"
#include "ui_simd.hpp"

namespace pink
{
  namespace benchmark
  {

    double now()
    {
      struct timeval tp;
      double sec, usec, result;
// Time stamp before the computations
      gettimeofday( &tp, NULL );
      sec    = static_cast<double>( tp.tv_sec );
      usec   = static_cast<double>( tp.tv_usec )/1E6;
      result = sec + usec;
      return result;
    } /* now */




    
  } /* namespace benchmark */
} /* namespace pink */


// LuM end of file
