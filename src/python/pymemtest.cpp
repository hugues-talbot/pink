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
    "memspeed64",
    pink::benchmark::memspeed<double>,
    (arg("memory size (MiB)")=100, arg("number_of_threads")=0),
    "Tests non-numa parallel memory speed of the system."
    );

#   ifdef PINK_HAVE_NUMA

  def(
    "numaspeed64",
    pink::benchmark::numa::memspeed<double>,
    (arg("memory size (MiB)")=100, arg("number_of_threads")=0),
    "Tests numa parallel memory speed of the system."
    );

  
#   endif /* PINK_HAVE_NUMA */

    
}; /* pymemtest */
















// LuM end of file
