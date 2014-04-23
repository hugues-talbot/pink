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

#include "python_doc.h"
#include "llong2byte.h"
#include "ui_convert.hpp"


void
pyconversion()
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
  
  // make "from mypackage.conv import <whatever>" work
  object conv_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.conv"))));
  
  // make "from mypackage import conv" work
  scope().attr("conv") = conv_module;
 
  // set the current scope to the new sub-module
  scope conv_scope = conv_module;

  //   UI_WRAP_CPP(
//     "convert2float",
//     pink::convert2float,
//     (arg("image")),
//     "converts an image to float type"
//     // end of the documenation
//     );


//   UI_WRAP_CPP(
//     "convert2float",
//     pink::convert2float,
//     (arg("image")),
//     "converts an image to float type"
//     // end of the documenation
//     );

//   UI_WRAP_CPP(
//     "convert2long",
//     pink::convert2long,
//     (arg("image")),
//     "converts an image to long type"
//     // end of the documenation
//     );

  //!! def( "float2byte", float2byte, ( arg("source image"), arg("mode")=2 ), doc__float2byte__c__ );

  //!! def( "long2byte",  long2byte,  ( arg("image"),arg("mode"),arg("nb_new_val") ), doc__long2byte__c__ );

  //!! def( "short2byte", short2byte, ( arg("image"), arg("mode") ), "converts a short image to long type" );

  //!! def( "long2short", long2short, ( arg("image"), arg("mode") ), "converts a long image to short type" );

  // #ifdef JULIETTE
  // resultdef( VFF_TYP_4_BYTE, "short2long", short2long, (arg("image")), "This operator converts the short images to long." );

  //  resultdef( VFF_TYP_2_BYTE, "long2short", long2short, (arg("image")), "This operator converts the long images to short." );
  // #endif // JULIETTE

  return;
} // pyconversion


// LuM end of file
