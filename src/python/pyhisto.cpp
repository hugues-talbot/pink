/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// Histogram Based Operators


#include "pyexport.hpp"

#include "lskew.h"
#include "lsquel.h"
#include "legalise.h"
#include "lstretch.h"
#include "lcontours.h"
#include "lsquelval.h"
#include "lsquelbin.h"
#include "lhistscal.h"
#include "python_doc.h"

void
pyhisto() {

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
  
  // make "from mypackage.histo import <whatever>" work
  object histo_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.histo"))));
  
  // make "from mypackage import histo" work
  scope().attr("histo") = histo_module;
  
  // set the current scope to the new sub-module
  scope histo_scope = histo_module;


  functiondef( "skew", lskew, (arg("image"), arg("skew")), doc__skew__c__ );

  functiondef( "squel", lsquel, (arg("image"), arg("threshold"), arg("ni threshold")), doc__squel__c__ );

  functiondef( "squelbin", lsquelbin, (arg("image"), arg("connexity"), arg("ni threshold")), doc__squelbin__c__ );

  functiondef( "stretch", lstretch, (arg("image")), doc__stretch__c__ );

  functiondef( "histscal", lhistscal3, (arg("image"), arg("a"), arg("A"), arg("b"), arg("B"), arg("c"), arg("C")), doc__histscal__c__ );

  functiondef( "histscal", lhistscal, (arg("image"), arg("a"), arg("A"), arg("b"), arg("B")), doc__histscal__c__ );

  functiondef( "histstretch", lhiststretch, (arg("image"), arg("vmin"), arg("vmax"), arg("p")), doc__histstretch__c__ );

  functiondef( "egalise", legalise, (arg("image"), arg("n")), doc__egalise__c__ );

  functiondef( "contours", lcontours, (arg("image"), arg("higher threshold"), arg("lower threshold")), doc__contours__c__ );

  import_array();  // numpy initialization

  return;

} // pyhisto

  
