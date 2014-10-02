/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The Pink Signal Processing Operators

#include "pyexport.hpp"

#include "lderiche.h"
#include "ldiZenzo.h"
#include "lconvol3.h"
#include "lattribvol.h"
#include "python_doc.h"
#include "lattribarea.h"

void
pysignal()
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

  // make "from mypackage.signal import <whatever>" work
  object signal_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.signal"))));
  
  // make "from mypackage import signal" work
  scope().attr("signal") = signal_module;
  
  // set the current scope to the new sub-module
  scope signal_scope = signal_module;

  functiondef( "convol3", lconvol3, (arg("image"), arg("mask"), arg("")), doc__convol3__c__ );

  functiondef( "gaussianfilter", lgaussianfilter, ( arg("image"), arg("alpha") ), doc__gaussianfilter__c__ );

  functiondef( "gradientcd", lgradientcd, ( arg("image"), arg("alpha") ), doc__gradientcd__c__ );
  
  functiondef( "segmentvol", lsegmentvol, (arg("image"), arg("connexity"), arg("param"), arg("m")), doc__segmentvol__c__ );
  
  functiondef( "segmentarea", lsegmentarea, (arg("image"), arg("connexity"), arg("area"), arg("m")), doc__segmentarea__c__ );

  resultdef( VFF_TYP_4_BYTE, "diZenzo", ldiZenzoGradient, (arg("imageR"), arg("imageG"), arg("imageB"), arg("alpha")), doc__diZenzo__c__ );

  resultdef( VFF_TYP_4_BYTE, "diZenzodirection", ldiZenzoDirection, (arg("imageR"), arg("imageG"), arg("imageB"), arg("alpha")), doc__diZenzo__c__ );
  
  return;  
} // pysignal
