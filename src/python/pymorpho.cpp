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

#include "ldir.h"
#include "ldist.h"
#include "lasft.h"
#include "lhtkern.h"
#include "lgradill.h"
#include "lgradsup.h"
#include "lgradinf.h"
#include "ldistgeo.h"
#include "lgeodesic.h"
#include "lhistscal.h"
#include "lerosplan.h"
#include "ldilateros.h"
#include "lgradinf3d.h"
#include "python_doc.h"
#include "pyasfbin.hpp"
#include "pyclosing.hpp"
#include "lmedialaxis.h"
#include "llambdakern.h"
#include "ldirections.h"
#include "lattribarea.h"
#include "lfiltreordre.h"
#include "ldilateros3d.h"
#include "lattribheight.h"
#include "llocalextrema.h"

void closing_export();
void opening_export();

//   UI_WRAP_CPP(
//     "insert_image",
//     insert_image,
//     ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
//     "this function inserts an image to another one. The "
//     "shift vector indicates the position of the lower corner "
//     "of the new image. If the image doesn't fit into the big one, "
//     "it is cropped."
//     );


//   UI_WRAP_CPP(
//     "merge_max_image",
//     merge_max_image,
//     ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
//     "this function inserts an image to another one. The "
//     "shift vector indicates the position of the lower corner "
//     "of the new image. If the image doesn't fit into the big one, "
//     "it is cropped. During the insertion the pixels are compared "
//     "and the bigger value is conserved"
//     );


//   UI_WRAP_CPP(
//     "frame",
//     frame,
//     ( arg("image"), arg("with value") ),
//     "This function takes an image, and in the result it sets it's most outer rectangle to the given value."
//     );


//   UI_WRAP_CPP(
//     "frame_around",
//     frame_around,
//     ( arg("image"), arg("with value") ),
//     "This function takes an image, and in the result and adds a frame around it, with"
//     " a given value. THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
//     );


//   UI_WRAP_CPP(
//     "frame_remove",
//     frame_remove,
//     ( arg("image") ),
//     "This function takes an image, and in the result and removes the most outer rectangle from the result. "
//     "THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
//     );


//   UI_WRAP_CPP(
//     "max",
//     immap_max,
//     ( arg("image1"), arg("image2") ),
//     "Generates an image result[i]:=max(image1[i],image2[i])"
//     );


//   UI_WRAP_CPP(
//     "min",
//     immap_min,
//     ( arg("image1"), arg("image2") ),
//     "Generates an image result[i]:=min(image1[i],image2[i])"
//     );


  


void pymorpho()
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
  
  // make "from mypackage.morpho import <whatever>" work
  object morpho_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.morpho"))));
  
  // make "from mypackage import morpho" work
  scope().attr("morpho") = morpho_module;
 
  // set the current scope to the new sub-module
  scope morpho_scope = morpho_module;

  // NOT IMPLEMENTED resultdef( "localextrema", llocalextrema, (arg("image"), arg("connex"), arg("minim")), doc__localextrema__c__ );
 
  // NOT IMPLEMENTED functiondef( "gradsup", lgradsup, (arg("image")), /*doc__gradsup__c__*/ "WRITE ME!!!" );
 
  // NOT IMPLEMENTED functiondef( "gradinf3d", lgradinf3d, (arg("image")), /* doc__gradinf3d__c__ */ "WRITE ME!!!" );
 
  functiondef( "gradill", lgradill, ( arg("image"), arg("alpha") ), doc__gradill__c__ );
 
  // !!! functiondef( "gradinf", lgradinf, (arg("image")), /* doc__gradinf__c__*/ "WRITE ME!!!" );
 
  functiondef( "dynrecons", ldynrecons, (arg("image"), arg("cond image"), arg("connexity")), doc__dynrecons__c__ );

  functiondef( "dir", ldir, (arg("image"), arg("dir")), doc__dir__c__ );

  functiondef( "directions", ldirections, (arg("image"), arg("connex"), arg("nbpoints"), arg("normale")), doc__directions__c__ );

  functiondef( "rankfilter", lfiltreordre, ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("r") ), doc__rankfilter__c__ );

  functiondef( "rankfilter", lfiltreordre3d, ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z"), arg("r") ), doc__rankfilter__c__ );

  functiondef( "heightminima", lheightminima, ( arg("image"), arg("connexity"), arg("height") ), doc__heightminima__c__ );

  functiondef( "asf", ldilateros_lasf, (arg("image"), arg("radius max"), arg("radius min")=1), doc__asf__c__ );
 
  functiondef( "asft", lasft_2D3D, ( arg("image"), arg("constraint image"), arg("complementary constraint image"), arg("connexity"), arg("radius max") ), doc__asft__c__ );
 
  functiondef( "asft", lasft_2D3D_null, ( arg("image"), arg("connexity"), arg("radius max") ), doc__asft__c__ );

  functiondef( "erosnum", lerosnum, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__erosnum__c__ );
 
  functiondef( "erosplan", lerosplan, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__erosplan__c__ );

  functiondef( "dilation", ldilateros_ldilat, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__dilat__c__ );

  functiondef( "dilation", ldilat3d, (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")), doc__dilat__c__ );

  functiondef( "dilatbin", ldilateros_ldilatbin, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__dilatbin__c__ );

  functiondef( "dilatbin", ldilateros3d_ldilatbin3d, (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")), doc__dilatbin__c__ );

  functiondef( "dilatnum", ldilatnum, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__dilatnum__c__ );

  functiondef( "lerosbin", ldilateros_lerosbin, (arg("image"), arg("elem"), arg("x"), arg("y")), doc__erosbin__c__ );

  functiondef( "lerosbin", ldilateros3d_lerosbin3d, (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")), doc__erosbin3d__c__ );

  functiondef( "asftmed", lasftmed_2D3D, (arg("image"),arg("connexity"), arg("max radius")), doc__asftmed__c__ );

  functiondef( "areaopening", lareaopening, (arg("image"),arg("connexity"), arg("area")), doc__areaopening__c__ );

  functiondef( "areaclosing", lareaclosing, (arg("image"),arg("connexity"), arg("area")), doc__areaclosing__c__ );

  functiondef( "openball", lopenball, (arg("image"), arg("radius"), arg("mode")=0), doc__openball__c__ );

  functiondef( "closeball", lcloseball, (arg("image"), arg("radius"), arg("mode")=0), doc__closeball__c__ );

  functiondef( "dilatball", ldilatball, (arg("image"), arg("radius"), arg("mode")=0), doc__dilatball__c__ );

  functiondef( "erosball", lerosball, (arg("image"), arg("radius"), arg("mode")=0), doc__erosball__c__ );

  functiondef( "geodilat", lgeodilat, (arg("image"), arg("constraint image"), arg("connex"), arg("nb iterations")=-1 ), doc__geodilat__c__ );

  functiondef( "geoeros",  lgeoeros,  (arg("image"), arg("constraint image"), arg("connex"), arg("nb iterations")=-1 ), doc__geoeros__c__ );

  def( "asfbin", pink::python::asfbin, (arg("image"), arg("max_radius"), arg("min_radius") ), doc__asfbin__c__ );

  resultdef( VFF_TYP_4_BYTE, "distgeo", ldistgeo, ( arg("image"), arg("mask"), arg("mode") ), doc__distgeo__c__ );

  resultdef( VFF_TYP_FLOAT, "lmedialaxis_lbisector", lmedialaxis_lbisector, (arg("image"), arg("mask")), doc__bisector__c__ );
  
  resultdef( VFF_TYP_FLOAT, "lambdamedialaxis", lmedialaxis_lambdamedialaxis, (arg("image")), doc__lambdamedialaxis__c__ );

  resultdef( VFF_TYP_4_BYTE, "medialaxis", lmedialaxis_lmedialaxis, (arg("image"), arg("distance")), doc__medialaxis__c__ );  
  
  closing_export();  

  opening_export();  
  
  import_array();  // numpy initialization
  
  return; 
} // pymorpho



// LuM end of file
