/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

// #ifndef PINK_DEVELOPMENT
// #  include <pink_python.h>
// #else /* PINK_DEVELOPMENT */
// #  include <pink_development.h>
// #endif /* PINK_DEVELOPMENT */

#include "pyexport.hpp"

#include "ujimage.hpp"
#include "ui_genball.hpp"
#include "uiNormalize.hpp"
#include "python_doc.h"

using boost::python::arg;
using boost::python::def;


void pynormalization()
{
// !!!!!
  
  def(
    "normalize",
    &pink::normalize<pink::char_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );

  def(
    "normalize",
    &pink::normalize<pink::int_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );

  def(
    "normalize",
    &pink::normalize<pink::float_image, 0, 1>,
    ( arg("image"), arg("minval")=0, arg("maxval")=1),
    doc__normalize__c__    
    );
  
}


// LuM end of file
