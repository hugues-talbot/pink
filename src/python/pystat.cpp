/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include "lccv.h"
#include "lstat.h"
#include "larith.h"
#include "python_doc.h"
#include "ldetectcercles.h"

void pystat()
{

  using pink::allocdef;
  using pink::resultdef;
  using pink::exportdef;  
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;

  // make "from mypackage.stat import <whatever>" work
  object stat_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.stat"))));
  
  // make "from mypackage import stat" work
  scope().attr("stat") = stat_module;
  
  // set the current scope to the new sub-module
  scope stat_scope = stat_module;

  functiondef( "detectcercles", ldetectcercles, (arg("image"), arg("radius")), doc__detectcercles__c__ );

  functiondef( "ccv", lccv, (arg("image"), arg("elem")), doc__ccv__c__ );

  exportdef( "area", larea, ( arg("image") ), doc__area__c__ );

  exportdef( "average", laverage1, ( arg("image") ), doc__average1__c__ );

  exportdef( "average", laverage2, ( arg("image"), arg("mask") ), doc__average1__c__ );

  functiondef( "ecarttype",  lecarttype, (arg("image"), arg("elem")), doc__ecarttype__c__ );
  
  exportdef( "variance", lvariance1, (arg("image")), doc__variance1__c__ );
  
  exportdef( "variance", lvariance2, (arg("image"), arg("mask")), doc__variance1__c__ );

  import_array();  // numpy initialization

  return;
} // pystat



// LuM end of file
