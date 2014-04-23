/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include "lfft.h"
#include "larith.h"
#include "python_doc.h"
#include "ui_any_to_complex.hpp"

// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ************** FUNCTIONS DEALING WITH COMPLEX IMAGES ******************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************

void
pycomplex()
{
  using pink::type;
  using pink::allocdef;
  using pink::resultdef;
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;

  // make "from mypackage.complex import <whatever>" work
  object complex_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.complex"))));
  
  // make "from mypackage import complex" work
  scope().attr("complex") = complex_module;
  
  // set the current scope to the new sub-module
  scope complex_scope = complex_module;

  // UI_WRAP_CPP( "any2complex", pink::lany2complex, (arg("real part"), arg("imaginary part")), doc__ui_any_to_complex__c__ );

  // functiondef( "fft", lfft, (arg("image"), arg("direction")), doc__fft__c__ );

  resultdef( VFF_TYP_FLOAT, "complex_modulus", lmodulus, (arg("image")), doc__complex_modulus__c__ );

  resultdef( VFF_TYP_FLOAT, "modulus", lmodulus, (arg("image")), "WRITE ME!!!" );

  resultdef( VFF_TYP_FLOAT, "real", lreal, (arg("image")), "WRITE ME!!!" );

  resultdef( VFF_TYP_FLOAT, "imaginary", limaginary, (arg("image")), "WRITE ME!!!" );
   
  import_array();  // numpy initialization

  return;  
} // pycomplex


