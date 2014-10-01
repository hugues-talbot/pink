/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
  (C) UjoImro, 2011-2012
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file


#include "pyexport.hpp"

#include "lattribute.h"
#include "python_doc.h"
#include "lattribvol.h"
#include "lattribarea.h"
#include "llabelextrema.h"
#include "lattribheight.h"
#include "ui_extractplane.hpp"

  
// UI_WRAP_CPP (
//   "extractplane",
//   pink::extractplane,
//   ( arg("image"), arg("number of the plane"), arg("mode") )
//   doc__extractplane__c__
//   );


void
pyconnect()
{
  using pink::allocdef;
  using pink::resultdef;
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;

  // make "from mypackage.connex import <whatever>" work
  object connex_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.connex"))));
  
  // make "from mypackage import connex" work
  scope().attr("connex") = connex_module;
  
  // set the current scope to the new sub-module
  scope connex_scope = connex_module;

  // boost::python::class_<pink::cxvimage>("xvimage");

  
  // def (
  //   "extractplane",
  //   pink::extractplane< pink::image<uint8_t> >,
  //   ( arg("image"), arg("number of the plane"), arg("mode") ),
  //   doc__extractplane__c__
  //   );

  functiondef( "heightselnb", lheightselnb, ( arg("image"), arg("connexity"), arg("height") ), doc__heightselnb__c__ );
  
  functiondef( "areaselnb", lareaselnb, ( arg("image"), arg("connexity"), arg("area") ), doc__areaselnb__c__ );
  
  functiondef( "volselnb", lvolselnb, ( arg("image"), arg("connexity"), arg("area") ), doc__volselnb__c__ );
  
  functiondef( "volmaxima", lvolmaxima, (arg("image"), arg("connex"), arg("param")), doc__volmaxima__c__ );

  resultdef( VFF_TYP_4_BYTE, "labelfgd", llabelfgd,  ( arg("image"), arg("connex") ), doc__labelfgd__c__ );

  // functions added by Michel
  resultdef( VFF_TYP_4_BYTE, "attribute", lattribute, ( arg("image"), arg("connex"), arg("typregion"), arg("attrib"), arg("seuil") ), doc__attribute__c__ );

  // functions added by Michel
  resultdef( VFF_TYP_4_BYTE, "planarity", lplanarity, ( arg("image"), arg("connex") ), doc__planarity__c__ );

  functiondef( "labelextrema",  llabelextrema,  ( arg("image"), arg("connex"), arg("function"), arg("result"), arg("nblabels") ), doc__label__c__ );

} // pyconnect



// LuM end of file
