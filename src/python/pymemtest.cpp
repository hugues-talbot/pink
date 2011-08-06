/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file contains the wrappers of maxflow functions


#include <pink_python.h>

#include "ui_systest.hpp"

using boost::python::arg;
using boost::python::def;

void pymemtest()
{
  def(
    "memspeed",
    pink::benchmark::memspeed,
    //(arg(),)
    "Tests single processor memory speed of the system."
    );

#   ifdef PINK_HAVE_NUMA
  def(
    "numaspeed",
    pink::benchmark::numaspeed,
    //(arg(),)
    "Tests single processor memory speed of a numa system."
    );
#   endif /* PINK_HAVE_NUMA */

    
}; /* pymemtest */
















// LuM end of file
