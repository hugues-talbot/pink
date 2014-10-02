/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The Pink Graphic Primitives

#include "pyexport.hpp"

#include "ldraw.h"
#include "ujimage.hpp"
#include "python_doc.h"

void genball_export();

void
pyprimitives () {

  using pink::allocdef;
  using pink::resultdef;
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;
  
  
  // we create the primitives submodule
  // motivation:
  // http://bfroehle.com/2011/07/boost-python-submodules/
  // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
  
  // make "from mypackage.primitives import <whatever>" work
  object primitives_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.primitives"))));
  
  // make "from mypackage import primitives" work
  scope().attr("primitives") = primitives_module;
  
  // set the current scope to the new sub-module
  scope primitives_scope = primitives_module;

  functiondef( "drawball", ldrawball, (arg("image"), arg("radius"), arg("xc"), arg("yc"), arg("zc")), doc__drawball__c__ );

  functiondef( "drawline", ldrawline, ( arg("image"), arg("x1"), arg("y1"), arg("x1"), arg("x2") ), doc__drawline__c__ );

  functiondef( "drawfilledellipse", ldrawfilledellipse, ( arg("image"), arg("R"), arg("S"), arg("T"), arg("U"), arg("V"), arg("Z") ), doc__drawellipse__c__ );

  allocdef ( "drawfield", ldrawfield, ( arg("field"), arg("len") ), doc__drawfield__c__ );

  functiondef( "drawtorus", ldrawtorus, ( arg("image"), arg("c"), arg("a"), arg("xc"), arg("yc"), arg("zc") ), doc__drawtorus__c__ );

  genball_export();
    
  // curve2spline
  // animscenes
  // drawcurve
  // drawrect 
  // drawspline
  // drawsplineorient.c
  // drawsplines.c
  // drawsplinesorient.c
  // drawtorus.c
  // drawtriangulation.c
  // genball.c
  // line.c
  // points2spline.c
  // samplespline.c

  import_array();  // numpy initialization

  return;
} // pyprimitives
