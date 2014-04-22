/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The Pink Geometrical Operators

#include "pyexport.hpp"

#include "lgeo.h"
#include "lsym.h"
#include "lcrop.h"
#include "ldraw.h"
#include "lzoom.h"
#include "loffset.h"
#include "lzoomint.h"
#include "ujimage.hpp"
#include "lattribute.h"
#include "python_doc.h"
#include "lrotations.h"
#include "lbarycentre.h"

void
pygeom () {

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

  // we create the geom submodule
  // motivation:
  // http://bfroehle.com/2011/07/boost-python-submodules/
  // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
  
  // make "from mypackage.geom import <whatever>" work
  object geom_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.geom"))));
  
  // make "from mypackage import geom" work
  scope().attr("geom") = geom_module;
  
  // set the current scope to the new sub-module
  scope geom_scope = geom_module;
  
  // export stuff in the geom namespace

  allocdef( "rotate", lrotationInter, (arg("image"), arg("theta"), arg("x"), arg("y"), arg("mode")), doc__rotate__c__ );

  resultdef( type<pink::char_image>(), "affine", laffinetransformation, ( arg("image"), arg("hx"), arg("hy"), arg("theta"), arg("tx"), arg("ty")), doc__affine__c__ );
 
  resultdef( type<pink::int_image>(), "attribute", lattribute, ( arg("image"), arg("connex"), arg("typregion"), arg("attrib"), arg("seuil") ), doc__attribute__c__ ); 
 
  allocdef( "autocrop", lautocrop, (arg("image"), arg("threshold")), doc__autocrop__c__ );
  
  functiondef ( "barycentre", lbarycentre, (arg("image"), arg("connex")), doc__barycentre__c__ );
  
  functiondef( "barycentrelab", lbarycentrelab, (arg("image")), doc__barycentrelab__c__ ); 
  
  functiondef( "boxmin", lboxmin, (arg("image")), doc__boxmin__c__ );
  
  // !! circleincl
  // !! convexhull

  allocdef( "crop", lcrop, (arg("image"), arg("x"), arg("y"), arg("w"), arg("h")), doc__crop__c__ );

  allocdef( "crop", lcrop3d, (arg("image"), arg("x"), arg("y"), arg("z"), arg("w"), arg("h"), arg("d")), doc__crop__c__ );

  functiondef( "line", ldrawline2, (arg("image")), doc__line__c__ );

  // !! curvatures
  // !! curve2segments
  // !! curve2spline
  // !! deframe
  // !! delanuay
  // !! delanuaymask
  // !! drawellipse
  // !! drawscene
  // !! drawspline
  // !! drawsplineorient
  // !! drawsplines
  // !! drawsplinesorient
  // !! drawtriangulation
  // !! ledengrowth
  // !! ellipsefit
  // !! ellipseincl
  // !! ellipseparams
  // !! enframe
  // !! extractline
  // !! extractplane
  // !! fitcircle
  // !! frame
  // !! genfield
  // !! genimage
  // !! genplane
  // !! identifyline
  // !! identifyparabola2
  // !! identifyparabola3
  // !! identifyplane
  // !! insert
  // !! isometry

  allocdef( "zoom", lzoom, ( arg("image"), arg("zoomx"), arg("zoomy"), arg("zoomz") ), doc__zoom__c__ );

  allocdef( "zoom", lzoom2, ( arg("image"), arg("newdim"), arg("dim") ), doc__zoomint__c__ );

  functiondef( "translate", loffset, (arg("image"), arg("ox"), arg("oy"), arg("oz"), arg("mode") =0), doc__translate__c__ );
 
  functiondef( "sym", lsym, (arg("image"), arg("mode")), doc__sym__c__ );

  functiondef( "drawtorus", ldrawtorus, (arg("image"), arg("r1"), arg("r2"), arg("xc"), arg("yc"), arg("zc")), doc__drawtorus__c__ );
  
  functiondef( "drawline", ldrawline, (arg("image"), arg("x1"), arg("y1"), arg("x2"), arg("y2")), doc__drawline__c__ );

  functiondef( "drawline", ldrawline3d, (arg("image"), arg("x1"), arg("y1"), arg("z1"), arg("x2"), arg("y2"), arg("z2")), doc__drawline__c__ );
 
  // NOTE: affine's using copyimage
  resultdef( VFF_TYP_1_BYTE, "affine", laffinetransformation, ( arg("image"), arg("hx"), arg("hy"), arg("theta"), arg("tx"), arg("ty")), doc__affine__c__ );

  import_array();  // numpy initialization
  
  return;
} // pyio
