/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "pyexport.hpp"

#include "larith.h"
#include "lderiche.h"
#include "uiCircle.hpp"
#include "python_doc.h"
#include "ui_measure.hpp"
#include "uiGradient.hpp"
#include "ui_fit_circle.hpp"

void
pyutil()
{
  using pink::allocdef;
  using pink::exportdef;
  using pink::resultdef;
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;
  
  // make "from mypackage.util import <whatever>" work
  object util_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.util"))));
  
  // make "from mypackage import util" work
  scope().attr("util") = util_module;
 
  // set the current scope to the new sub-module
  scope util_scope = util_module;

  functiondef( "gammacor", lgammacor, (arg("image"), arg("gamma")), "WRITE ME!!! doc__gammacor__c__" );

  
// NOTE: lderiche the question of l parameter
  functiondef( "deriche", lderiche_lderiche, (arg("image"), arg("alpha"), arg("function"), arg("l") ), doc__deriche__c__ );

  functiondef( "shencastan", lshencastan, (arg("image"), arg("beta")), doc__shencastan__c__ );

  allocdef( "gradientabs", pink::uiGradientAbs, (arg("image")), doc__gradient_abs__cpp__ );

  def( "measure", pink::pymeasure<pink::float_image>, (arg("image")), doc__measure__cpp__    );

  exportdef( "circle_tangent", &pink::gsl::circle_tangent,
             (arg("x coordinates"), arg("y coordinates"), arg("point of derivation")),
             "This function estimates the derivativ of the function given by points. It "
             " models a circle and returns its tangent at the given point."
    );

  //!!!!! allocdef( "fit_circle", &pink::py_fit_circle,
  //      (arg("x coordinates"), arg("y coordinates"),arg("filename")=""),
  //      "This function fits a circle to the given points. It returns a vector for the following formula:\n"
  //      "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
  //      "If the argument filename is given, than the function will generate an example Mathematica\n"
  //      "code with the circle and the points."
  //      "The algorithm is a modified version of that in:\n"
  //      "Andrew Fitzgibbon, Maurizio Pilu, Robert B. Fisher, 'Direct Least Square Fitting of Ellipses', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 21, no. 5, pp. 476-480, May 1999, doi:10.1109/34.765658\n"
  //   );

  exportdef( "circle_equation_to_coordinates", &pink::py_circle_equation_to_coordinates,
            (arg("equation")),
            "This function converts the vector of equation coordinates to a vector of geometric coordinates:\n"
            "the input vector is [a,b,c,d], where\n"
            "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
            "the output vector is [center_x, center_y, r], where\n"
            "  (x-center_x)^2 + (y-center_y)^2 == r^2 "
    );

  import_array();  // numpy initialization
    
  return; 
} // pyutil



// LuM end of file
