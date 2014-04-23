/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2012-2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "pyexport.hpp"

#include "lseuil.h"
#include "larith.h"
#include "python_doc.h"
#include "ldetectcercles.h"

void pyarithmetic()
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

  // make "from mypackage.arith import <whatever>" work
  object arith_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.arith"))));
  
  // make "from mypackage import arith" work
  scope().attr("arith") = arith_module;
  
  // set the current scope to the new sub-module
  scope arith_scope = arith_module;

  functiondef( "detectcercles", ldetectcercles, (arg("image"), arg("radius")), doc__detectcercles__c__ );

  functiondef( "exp", lexp, (arg("image")), doc__exp__c__ );

  functiondef( "pow", lpow, (arg("image"), arg("p")), doc__pow__c__ );
  
  functiondef( "log", llog, (arg("image")), doc__log__c__ );
    
  functiondef( "inverse", linverse, (arg("image")), doc__inverse__c__ );

  functiondef( "xor", lxor, (arg("image1"), arg("image2")), doc__xor__c__ );

  functiondef( "seuil", lseuil, (arg("image"), arg("threshold")), doc__seuil__c__ );

  functiondef( "threshold", lseuil, (arg("image"), arg("threshold")), doc__seuil__c__ );

  functiondef( "seuil", lseuil3, (arg("image"), arg("low threshold"), arg("high threshold")), doc__seuil__c__ );

  functiondef( "threshold", lseuil3, (arg("image"), arg("low threshold"), arg("high threshold")), doc__seuil__c__ );
  
  import_array();  // numpy initialization

} // pyaritmetic



// LuM end of file
