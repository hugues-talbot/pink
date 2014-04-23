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

#include "pyexport.hpp"

#include "llpemeyer.h"
#include "python_doc.h"
//#include "llpemeyer.h"
#include "lwshedtopo.h"
#include "powerwatsegm.h"
#include "ldynamique_grimaud.h"

using namespace pink;
using boost::python::arg;
using boost::python::def;

void
pywshed()
{
  using boost::python::scope;
  using boost::python::handle;  
  using boost::python::object;
  using boost::python::borrowed;
  
  // we create the wshed submodule
  // motivation:
  // http://bfroehle.com/2011/07/boost-python-submodules/
  // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
  
  // make "from mypackage.IO import <whatever>" work
  object wshed_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.wshed"))));
  
  // make "from mypackage import IO" work
  scope().attr("wshed") = wshed_module;
  
  // set the current scope to the new sub-module
  scope wshed_scope = wshed_module;

  resultdef( VFF_TYP_4_BYTE, "powerwatershedBW", compute_power_watershed_bw, (arg("image_bw"), arg("seeds"), arg("geodesic"), arg("multiple")), "WRITE ME!!!" /*doc__powerwshedval__c__*/ );

  resultdef( VFF_TYP_4_BYTE, "powerwatershedCol", compute_power_watershed_col, (arg("image_r"), arg("image_g"), arg("image_b"), arg("seeds"), arg("geodesic"), arg("multiple")), "WRITE ME!!!" /*doc__powerwshedval__c__*/ );

  functiondef( "watershed", lwshedtopobin, ( arg("image"), arg("mark"), arg("connexity") ), doc__watershed__c__ );

// by HT for Claire
  functiondef( "watershedMeyer2", llpemeyer_llpemeyer2, ( arg("markers"), arg("image"), arg("mask"), arg("connexity") ), doc__watershedMeyer2__c__ );

// by MC
  functiondef( "watershedMeyer2lab", llpemeyer2lab, ( arg("markers"), arg("image"), arg("mask"), arg("connexity") ), doc__watershedMeyer2lab__c__ );

// by MC
  functiondef( "watershedMeyer2labnomask", llpemeyer2lab_nomask, ( arg("markers"), arg("image"), arg("connexity") ), doc__watershedMeyer2lab__c__ );

// by HT for Claire
  functiondef( "watershedMeyer2NM", llpemeyer2_nomask, ( arg("markers"), arg("image"), arg("connexity") ), doc__watershedMeyer2__c__ );

  functiondef( "wshedval", lwshedval, (arg("image"), arg("connex")), doc__wshedval__c__ );

  import_array();  // numpy initialization

  return;
}



// LuM end of file
