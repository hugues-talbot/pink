/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#ifndef PINK_DEVELOPMENT
#  include <pink_python.h>
#else /* PINK_DEVELOPMENT */
#  include <pink_development.h>
#endif /* PINK_DEVELOPMENT */

using namespace boost::python;
using namespace pink;

// maybe I'll get away with it
#include "pyujimage.hpp"
#include "pyexport.hpp"
#include "ui_wrap_function.hpp"
#include "ui_wrap_result.hpp"
#include "ui_wrap_value.hpp"
//#include "ui_wrap_value.hpp"
#include "ui_make_function.hpp"
#include "ui_make_result.hpp"
#include "ui_make_value.hpp"
#include "python_doc.h"


//#include <boost/preprocessor.hpp>
//#include <pyexport.hpp>


/*! \file pypink.c
  For exporting functions from pink you should make them into the following
  format:

  * In C
  int pink_c_function( xvimage * image, type_1 var_1, type_2 var_2, ..., type_n var_n)

  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  def( "function's name in python",
  &make_function<char_image, type_1, type_2, ..., type_n, &pink_c_function>,
  (arg("argument 1 name"), arg("argument 2 name"), ..., arg(argument n name) )
  "documentation of my function"
  )
 
  make_function is a template. In the first parameter you specify the image type. If
  you want all image types to be exported you put 'base_image' as type. Second, you
  specify the types of the parameters and last you put the pointer to your pink function.
  Third you may specify the names of the parameters to appear in python's help. Finally
  you can put a string with the documentation which will appear in the python's help.

  Example:
  def( "ptisolated",
  &make_function<char_image, int, &lptisolated>,
  ( arg("image"), arg("connexity") ),
  doc__ptisolated__c__
  );
  

  * in C++

  You should write your function in format

  char_image cpp_function( const char_image & image, type_1 var_1, type_2 var_2, ..., type_n var_n )
  here you can put any image type: char_image, int_image, float_image...

  or

  template <class image_type>
  image_type cpp_function( const image_type & image, type_1 var_1, type_2 var_2, ..., type_n var_n )

  You can call 'cout <<' as well as you can use exceptions. By convention you should never change the
  input image. Allways make a copy of it before processing. Remember that char_image is a SHALLOW TYPE
  char_image image2 (image1) will NOT make a copy. On the other hand you can use function(char_image I)
  as a parameter-type without the copy penalty.
  To export this function you should call

  UI_DEFINE_ONE_FUNCTION(
  function_name_in_python,
  cpp_function,
  ( arg("parameter 1 name"), ..., arg("parameter n name") ),
  "documentation of cpp_function"
  )

  or, if you have a template function: 

  UI_DEFINE_FUNCTION(
  function_name_in_python,
  cpp_function,
  ( arg("parameter 1 name"), ..., arg("parameter n name") ),
  "documentation of cpp_function"
  )

  Examples:
  UI_DEFINE_ONE_FUNCTION(
  float2byte,
  pink::float2byte,
  ( arg("source image"), arg("mode")=2 ),
  doc__float2byte__c__
  // end of the documenation
  );
    
  UI_DEFINE_FUNCTION(
  convert2float,
  pink::convert2float,
  (arg("image")),
  "converts an image to float type"
  // end of the documenation
  );
   

*/
// declarations
// void deep_xvimage_object_export();void shallow_xvimage_object_export();
// void xvimage_object_export();
// void writeimage_export();
void vint_object_export(); void vector_int_object_export();void wshedtopo_export();
/*void erosball_export()*/;void medianfilter_export();

// skeleton function
void skeleton2_export();void zoom_export();

void dilation_export(); void erosion_export(); void geodilat_export();
void geoeros_export(); void opening_export(); void drawcurve2D_export();
void uiSqhool_object_export(); void gradient_export();

void read_raw_image_export(); void seuil_export(); void plane3d_export(); void draw_plane_export();
void project_plane_export(); void border_export(); void identifyline_export(); void surimp_export();
void generate_rgb_image_export(); void closing_export(); void closeball_export(); void minmax_export();
/*void dilatball_export();*/ void asfbin_export(); 
void skelcurv_export(); void distc_export();
void readimage_export(); void skelsurf_export(); void toposhrink_export(); void htkern_export();
/*void openball_export();*/ void gradmorph_export(); void mcube_export(); void minima_export();


// this part is not yet ready for windows
#ifdef UNIXIO
// not sure this is the correct thing to do here HT
// This is the prototype of the automatically generated export function
void Pimview_export(); void Pimviewlogin_export(); void Pimviewputimage_export();
void PLiarEnableDebug_export() ; void PLiarDisableDebug_export(); void Pimview_force_socket_export();
void Pimview_sendcommand_export();
#endif /* UNIXIO */

// for wrapping every type with one function
// template <class image_type>
// void ujoi_class_export( const char* object_name, const char* object_description );

/// ********************************************************************************************
/// ********************************************************************************************
/// ********************************************************************************************
/// *****                                                                                  *****
/// *****                          ALTERNATIVE 2011 WRAPPING                               *****
/// *****                                                                                  *****
/// ********************************************************************************************
/// ********************************************************************************************
/// ********************************************************************************************


#include <boost/preprocessor/slot/counter.hpp>


UI_WRAP_FUNCTION(
  "ptisolated",
  lptisolated,
  ( arg("image"), arg("connexity") ),
  doc__ptisolated__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptend",
  lptend,
  ( arg("image"), arg("connexity") ),
  doc__ptend__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptcurve",
  lptcurve,
  ( arg("image"), arg("connexity") ),
  doc__ptcurve__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
  "ptjunction",
  lptjunction,
  ( arg("image"), arg("connexity") ),
  doc__ptjunction__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "selectcomp",
  lselectcomp,
  ( arg("image"), arg("connexity"), arg("x"), arg("y"), arg("z") ),
  doc__selectcomp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "holeclosing",
  lfermetrous3dbin,
  ( arg("image"), arg("connexity"), arg("holesize") ),
  doc__holeclosing__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "holeclosing",
  lfermetrous3dbin2,
  ( arg("image"), arg("guiding image") ,arg("connexity"), arg("holesize") ),
  doc__holeclosing__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "lambdaskel",
  llambdakern,
  ( arg("image"), arg("imcond"), arg("connexity"), arg("lambda") ),
  doc__lambdaskel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "lambdaskel",
  llambdakern_short,
  ( arg("image"), arg("connexity"), arg("lambda") ),
  doc__lambdaskel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "rankfilter",
  lfiltreordre,
  ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("r") ),
  doc__rankfilter__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "rankfilter",
  lfiltreordre3d,
  ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z"), arg("r") ),
  doc__rankfilter__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "tuf",
  ltuf,
  ( arg("image"), arg("min connex"), arg("radius") ),
  doc__tuf__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "taf",
  ltaflambda,
  ( arg("image"), arg("min connex"), arg("radius"), arg("lambdapics"), arg("lambdapuits") ),
  doc__taf__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "watershed",
  lwshedtopobin,
  ( arg("image"), arg("mark"), arg("connexity") ),
  doc__watershed__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// by HT for Claire
UI_WRAP_FUNCTION(
  "watershedMeyer2",
  llpemeyer2,
  ( arg("image"), arg("markers"), arg("mask"), arg("connexity") ),
  doc__watershedMeyer2__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// by HT for Claire
UI_WRAP_FUNCTION(
  "watershedMeyer2NM",
  llpemeyer2_nomask,
  ( arg("image"), arg("markers"), arg("connexity") ),
  doc__watershedMeyer2__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
    "labelextrema",
  llabelextrema,
    ( arg("image"), arg("connex"), arg("function"), arg("result"), arg("nblabels") ),
  doc__label__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
  "heightmaxima",
  lheightmaxima,
  ( arg("image"), arg("connexity"), arg("height") ),
  doc__heightmaxima__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "heightminima",
  lheightminima,
  ( arg("image"), arg("connexity"), arg("height") ),
  doc__heightminima__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// functions added by Michel

UI_WRAP_FUNCTION(
  "segmentheight",
  lsegmentheight,
  ( arg("image"), arg("connexity"), arg("height"), arg("mode") ),
  doc__segmentheight__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "segmentarea",
  lsegmentarea,
  ( arg("image"), arg("connexity"), arg("area"), arg("mode") ),
  doc__segmentarea__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "segmentvol",
  lsegmentvol,
  ( arg("image"), arg("connexity"), arg("vol"), arg("mode") ),
  doc__segmentvol__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "heightselnb",
  lheightselnb,
  ( arg("image"), arg("connexity"), arg("height"), arg("mode") ),
  doc__heightselnb__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "areaselnb",
  lareaselnb,
  ( arg("image"), arg("connexity"), arg("area") ),
  doc__areaselnb__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "volselnb",
  lvolselnb,
  ( arg("image"), arg("connexity"), arg("area") ),
  doc__volselnb__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_RESULT(
  int_image,
  "distgeo",
  ldistgeo,
  ( arg("image"), arg("mask"), arg("mode") ),
  doc__distgeo__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// // NOTE: affine's using copyimage
// UI_WRAP_RESULT(  
//   "affine",
//   laffinetransformation,
//   ( arg("image"), arg("hx"), arg("hy"), arg("theta"), arg("tx"), arg("ty")),
//   doc__affine__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "asft",
  lasft_2D3D,
  ( arg("image"), arg("constraint image"), arg("complementary constraint image"), arg("connexity"), arg("radius max") ),
  doc__asft__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "asft",
  lasft_2D3D_null,
  ( arg("image"), arg("connexity"), arg("radius max") ),
  doc__asft__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "gaussianfilter",
  lgaussianfilter,
  ( arg("image"), arg("alpha") ),
  doc__gaussianfilter__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "gaussianfilter",
  lgaussianfilter,
  ( arg("image"), arg("alpha") ),
  doc__gaussianfilter__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "gradientcd",
  lgradientcd,
  ( arg("image"), arg("alpha") ),
  doc__gradientcd__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "exp",
  lexp,
  (arg("image")),
  doc__exp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "print_image",
  print_image,
  (arg("image")),
  "Prints the values in a table. Usefull for debugging small 2D images."
  );
#include BOOST_PP_UPDATE_COUNTER()
  
UI_WRAP_CPP(
  "insert_image",
  insert_image, 
  ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
  "this function inserts an image to another one. The "
  "shift vector indicates the position of the lower corner "
  "of the new image. If the image doesn't fit into the big one, "
  "it is cropped."
  );
#include BOOST_PP_UPDATE_COUNTER()
  
UI_WRAP_CPP(
  "merge_max_image", 
  merge_max_image, 
  ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
  "this function inserts an image to another one. The "
  "shift vector indicates the position of the lower corner "
  "of the new image. If the image doesn't fit into the big one, "
  "it is cropped. During the insertion the pixels are compared "
  "and the bigger value is conserved"
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "frame",
  frame,
  ( arg("image"), arg("with value") ),
  "This function takes an image, and in the result it sets it's most outer rectangle to the given value."
  );
#include BOOST_PP_UPDATE_COUNTER()
  
UI_WRAP_CPP(
  "frame_around",
  frame_around,
  ( arg("image"), arg("with value") ),
  "This function takes an image, and in the result and adds a frame around it, with"
  " a given value. THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "frame_remove",
  frame_remove,
  ( arg("image") ),
  "This function takes an image, and in the result and removes the most outer rectangle from the result. "
  "THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "max",
  immap_max,
  ( arg("image1"), arg("image2") ),      
  "Generates an image result[i]:=max(image1[i],image2[i])"
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "min",
  immap_min,
  ( arg("image1"), arg("image2") ),      
  "Generates an image result[i]:=min(image1[i],image2[i])"
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "inverse",
  pink::inverse,
  ( arg("image") ),
  doc__inverse__c__
// end of the documenation
  );    
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "convert2float",
  pink::convert2float,
  (arg("image")),
  "converts an image to float type"
  // end of the documenation
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_CPP(
  "extractplane",
  pink::extractplane,
  ( arg("image"), arg("number of the plane"), arg("mode") ),
  doc__extractplane__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_VALUE(
  "area",
  larea,
  ( arg("image") ),
  doc__area__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_VALUE(
  "average",
  laverage1,
  ( arg("image") ),
  doc__average__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_VALUE(
  "average",
  laverage2,
  ( arg("image"), arg("mask") ),
  doc__average__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


// UI_WRAP_VALUE(
//   "l2dinvariants",
//   l2dinvariants,
//   ( arg("image") ),
//   doc__2dinvariants__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()


// UI_WRAP_VALUE(
//   "countvalues",
//   lcountvalues,
//   ( arg("image"), arg("make") ),
//   "WRITE ME!!"//doc__countvalues__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_VALUE(
//   "invariants3d",
//   l3dinvariants,
//   ( arg("image"), arg("make") ),
//   "WRITE ME!!"//doc__countvalues__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
  "barycentre",
  lbarycentre,
  (arg("image"), arg("connvex")),
  doc__barycentre__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_RESULT(
  float_image,
  "lmedialaxis_lbisector",
  lmedialaxis_lbisector,
  (arg("image"), arg("mask")),
  doc__bisector__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "drawline",
  ldrawline,
  (arg("image"), arg("x1"), arg("y1"), arg("x2"), arg("y2")),
  doc__drawline__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "drawline",
  ldrawline3d,
  (arg("image"), arg("x1"), arg("y1"), arg("z1"), arg("x2"), arg("y2"), arg("z2")),
  doc__drawline__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "border2d",
  l2dborder,
  (arg("image") ),
  doc__2dborder__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "color2d",
  l2dcolor,
  (arg("image") ),
  doc__2dcolor__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "color2d",
  l2dcolor,
  (arg("image") ),
  doc__2dcolor__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "makecomplex2d",
  l2dmakecomplex,
  (arg("image") ),
  doc__2dmakecomplex__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "closebeta2d",
  l2dclosebeta,
  (arg("image") ),
  doc__2dmakecomplex__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pardircollapse2d",
  l2dpardircollapse,
  (arg("image"), arg("nsteps"), arg("inhibit") ),
  doc__2dpardircollapse__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pardircollapse2d",
  l2dpardircollapse_short,
  (arg("image"), arg("nsteps") ),
  doc__2dpardircollapse__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "seltype2d",
  l2dseltype,
  (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")),
  doc__2dseltopo__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "2dthin",
  l2dthin,
  (arg("image") ),
  doc__2dthin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "alpha3d",
  l3dalpha,
  (arg("image") ),
  doc__3dalpha__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "beta3d",
  l3dbeta,
  (arg("image") ),
  doc__3dbeta__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "color3d",
  l3dcolor,
  (arg("image") ),
  doc__3dcolor__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "isthmus3d",
  l3disthmus,
  (arg("image") ),
  doc__3disthmus__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "makecomplex3d",
  l3dmakecomplex,
  (arg("image") ),
  doc__3dmakecomplex__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pardircollapse3d",
  l3dpardircollapse,
  (arg("image"), arg("nsteps"), arg("inhibit") ),
  doc__3dpardircollapse__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pardircollapse3d",
  l3dpardircollapse_short,
  (arg("image"), arg("nsteps") ),
  doc__3dpardircollapse__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "plane3d",
  l3dplane,
  ( arg("image"), arg("a"), arg("b"), arg("c"), arg("d") ),
  doc__3dplane__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "seltype3d",
  l3dseltype,
  (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")),
  doc__3dseltopo__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "skelsurf3d",
  l3dskelsurf,
  (arg("image"), arg("nsteps") ), 
  doc__3dskelsurf__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "sphere3d",
  l3dsphere,
  (arg("image"), arg("x0"), arg("y0"), arg("z0"), arg("radius")),
  doc__3dsphere__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "3dthin",
  l3dthin,
  (arg("image"), arg("nsteps") ),
  doc__3dthin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "alpha",
  lalpha,
  ( arg("image"), arg("connexity"), arg("sign") ),
  doc__alpha__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "amont",
  lamont,
  ( arg("image"), arg("connexity"), arg("sign") ),
  doc__amont__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


// NOTE: 3dlabel input byte 3d output int 3d

// UI_WRAP_FUNCTION(
//   "",
//   l,
//   (arg("image") ),
//   doc____c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "areaclosing",
  lareaclosing,
  (arg("image"),arg("connexity"), arg("area")),
  doc__areaclosing__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "areaopening",
  lareaopening,
  (arg("image"),arg("connexity"), arg("area")),
  doc__areaopening__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "areaselnb",
  lareaselnb,
  (arg("image"),arg("connexity"), arg("nb")),
  doc__areaselnb__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "asftmed",
  lasftmed_2D3D,
  (arg("image"),arg("connexity"), arg("max radius")),
  doc__asftmed__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "boxmin",
  lboxmin,
  (arg("image")),
  doc__boxmin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ccv",
  lccv,
  (arg("image"), arg("elem")),
  doc__ccv__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "contours",
  lcontours,
  (arg("image"), arg("higher threshold"), arg("lower threshold")),
  doc__contours__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "convol3",
  lconvol3,
  (arg("image"), arg("mask"), arg("")),
  doc__convol3__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "deletecomp",
  ldeletecomp,
  (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")),
  doc__deletecomp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// NOTE: lderiche the question of l parameter
UI_WRAP_FUNCTION(
  "deriche",
  lderiche,
  (arg("image"), arg("alpha"), arg("function"), arg("l") ),
  doc__deriche__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "detectcercles",
  ldetectcercles,
  (arg("image"), arg("radius")),
  doc__detectcercles__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "dilatbin",
  ldilateros_ldilatbin,
  (arg("image"), arg("elem"), arg("x"), arg("y")),
  doc__dilatbin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "dilatbin",
  ldilatbin3d,
  (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")),
  doc__dilatbin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "dilatnum",
  ldilatnum,
  (arg("image"), arg("elem"), arg("x"), arg("y")),
  doc__dilatnum__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


//NOTE: consider directionalfilter because of the converfloat

UI_WRAP_FUNCTION(
  "dir",
  ldir,
  (arg("image"), arg("dir")),
  doc__dir__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "directions",
  ldirections,
  (arg("image"), arg("connex"), arg("nbpoints"), arg("normale")),
  doc__directions__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "drawball",
  ldrawball,
  (arg("image"), arg("radius"), arg("xc"), arg("yc"), arg("zc")),
  doc__drawball__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "drawtorus",
  ldrawtorus,
  (arg("image"), arg("r1"), arg("r2"), arg("xc"), arg("yc"), arg("zc")),
  doc__drawtorus__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "dynrecons",
  ldynrecons,
  (arg("image"), arg("cond image"), arg("connexity")),
  doc__dynrecons__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ecarttype",
  lecarttype,
  (arg("image"), arg("elem")),
  doc__ecarttype__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "egalise",
  legalise,
  (arg("image"), arg("n")),
  doc__egalise__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ellipticite",
  lellipticite,
  (arg("image"), arg("connex")),
  doc__ellipticite__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "lerosbin",
  ldilateros_lerosbin,
  (arg("image"), arg("elem"), arg("x"), arg("y")),
  doc__erosbin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "lerosbin",
  ldilateros3d_lerosbin3d,
  (arg("image"), arg("elem"), arg("x"), arg("y"), arg("z")),
  doc__erosbin3d__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "erosnum",
  lerosnum,
  (arg("image"), arg("elem"), arg("x"), arg("y")),
  doc__erosnum__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "erosplan",
  lerosplan,
  (arg("image"), arg("elem"), arg("x"), arg("y")),
  doc__erosplan__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "fermetrous3d",
//   lfermetrous3dbin,
//   (arg("image"), arg("connex"), arg("hole size")),
//   doc__fermetrous3d__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()


// NOTE: fft up for discussion
// NOTE: gettree return value type

// UI_WRAP_FUNCTION(
//   "gradill",
//   lgradill,
//   ( arg("image"), arg("alpha") ),
//   doc__gradill__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradinf",
//   lgradinf,
//   (arg("image")),
// /*  doc__gradinf__c__*/ "WRITE ME!!!"
//   );
// #include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradinf3d",
//   lgradinf3d,
//   (arg("image")),
//   /* doc__gradinf3d__c__ */ "WRITE ME!!!"
//   );
// #include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "gradsup",
//   lgradsup,
//   (arg("image")),
//   /*doc__gradsup__c__*/ "WRITE ME!!!"
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "grayskel",
  lgrayskel,
  (arg("image"), arg("cond image"), arg("connex"), arg("lambda")),
  doc__grayskel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "grayskel",
  lgrayskel_short,
  (arg("image"), arg("connex"), arg("lambda")),
  doc__grayskel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "histscal",
  lhistscal3,
  (arg("image"), arg("a"), arg("A"), arg("b"), arg("B"), arg("c"), arg("C")),
  doc__histscal__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "histscal",
  lhistscal,
  (arg("image"), arg("a"), arg("A"), arg("b"), arg("B")),
  doc__histscal__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_RESULT(
  char_image,
  "houghcircles",
  lhoughcercles,
  (arg("image"), arg("min radius"), arg("pas radius"), arg("nb pas")),
  doc__houghcercles__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "hthiniso",
  lhthiniso,
  (arg("image"), arg("dmax"), arg("connex"), arg("pixwhratio")),
  doc__hthiniso__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "jones",
  ljones,
  (arg("image"), arg("connex")),
  doc__jones__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "kern",
  lkern,
  (arg("image"), arg("connex")),
  doc__kern__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


// optionally lrao instead of lleviandi
UI_WRAP_FUNCTION(
  "levialdi",
  llevialdi,
  (arg("image"), arg("connex"), arg("n")),
  doc__levialdi__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
  "line",
  ldrawline2,
  (arg("image")),
  doc__line__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// // NOTE: localextrema's allocating the same type
// UI_WRAP_RESULT(
//   "localextrema",
//   llocalextrema,
//   (arg("image"), arg("connex"), arg("minim")),
//   doc__localextrema__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "log",
  llog,
  (arg("image")),
  doc__log__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "matchellipse",
  lmatchellipse,
  (arg("image"), arg("connex")),
  doc__matchellipse__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "matchrect",
  lmatchrect,
  (arg("image"), arg("connex"), arg("mode")),
  doc__matchrect__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "maxdiameter",
  lmaxdiameter,
  (arg("image"), arg("connex")),
  doc__maxdiameter__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


UI_WRAP_FUNCTION(
  "nbvois",
  lnbvois,
  (arg("image"), arg("connex")),
  doc__nbvois__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pow",
  lpow,
  (arg("image"), arg("p")),
  doc__pow__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "pthseparatinggray",
  lpthseparatinggray,
  (arg("image"), arg("connex"), arg("h")),
  doc__pthseparatinggray__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptinterior",
  lptinterior,
  (arg("image"), arg("connex")),
  doc__ptinterior__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptjunction",
  lptjunction,
  (arg("image"), arg("connex")),
  doc__ptjunction__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptmultiple",
  lptmultiple,
  (arg("image"), arg("connex")),
  doc__ptmultiple__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptseparating",
  lptseparating,
  (arg("image"), arg("connex")),
  doc__ptseparating__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptseparatinggray",
  lptseparatinggray,
  (arg("image"), arg("connex")),
  doc__ptseparatinggray__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "ptsimple",
  lptsimple,
  (arg("image"), arg("connex")),
  doc__ptsimple__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "quasishear",
  lquasishear,
  (arg("image"), arg("theta"), arg("x"), arg("y")),
  doc__quasishear__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


// NOTE: this function returns a pointer
// UI_WRAP_FUNCTION(
//   "quasishear",
//   lquasishear2,
//   (arg("image"), arg("theta")),
//   doc__quasishear__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()


// NOTE: recalagerigide returns a list

UI_WRAP_FUNCTION(
  "reconsplateaux",
  lreconsplateaux,
  (arg("image1"), arg("image2"), arg("connexity")),
  doc__reconsplateaux__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "remspnoise",
  lremspnoise,
  (arg("image"), arg("g"), arg("k")),
  doc__remspnoise__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// NOTE: think about image resizing
// UI_WRAP_FUNCTION(
//   "rotate",
//   lrotationInner1,
//   (arg("image"), arg("theta")),
//   doc__rotate__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

// UI_WRAP_FUNCTION(
//   "rotate",
//   lrotationInner2,
//   (arg("image"), arg("theta"), arg("x"), arg("y")),
//   doc__rotate__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "segmentarea",
  lsegmentarea,
  (arg("image"), arg("connexity"), arg("area"), arg("m")),
  doc__segmentarea__c__
  );
#include BOOST_PP_UPDATE_COUNTER()


// NOTE: the implementation not found
// UI_WRAP_FUNCTION(
//   "segmenti",
//   lsegmenti,
//   (arg("image"), arg("connexity")),
//   doc__segmenti__c__
//   );
// #include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "segmentlines",
  lsegmentlignes,
  (arg("image"), arg("connexity"), arg("lower threshold"), arg("higher threshold")),
  doc__segmentlignes__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "segmentvol",
  lsegmentvol,
  (arg("image"), arg("connexity"), arg("param"), arg("m")),
  doc__segmentvol__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "selectcomp",
  lselectcomp,
  (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")),
  doc__selectcomp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "selrect",
  lselrect,
  (arg("image"), arg("x"), arg("y"), arg("w"), arg("h")),
  doc__selrect__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "seltopo",
  lseltopo,
  (arg("image"), arg("connexity"), arg("t-"), arg("t+"), arg("tb-"), arg("tb+")),
  doc__seltopo__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "shencastan",
  lshencastan,
  (arg("image"), arg("beta")),
  doc__shencastan__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "simplepair",
  lsimplepair,
  (arg("image"), arg("all")),
  doc__simplepair__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "skew",
  lskew,
  (arg("image"), arg("skew")),
  doc__skew__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "squel",
  lsquel,
  (arg("image"), arg("threshold"), arg("ni threshold")),
  doc__squel__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "squelbin",
  lsquelbin,
  (arg("image"), arg("connexity"), arg("ni threshold")),
  doc__squelbin__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "stretch",
  lstretch,
  (arg("image")),
  doc__stretch__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "surfacerestoration",
  lsurfacerestoration,
  (arg("image"), arg("number of maximal iterations"), arg("connexity")),
  doc__surfacerestoration__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "surrect",
  lrectangleincluant,
  (arg("image"), arg("connexity")),
  doc__surrect__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "sym",
  lsym,
  (arg("image"), arg("mode")),
  doc__sym__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t26pp",
  ltopotypes_t26pp, 
  (arg("image")),
  doc__t26pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t4pp",
  lt4pp,
  (arg("image")),
  doc__t4pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "t8pp",
  lt8pp,
  (arg("image")),
  doc__t8pp__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "tlf",
  ltlf,
  (arg("image"), arg("connexmin"), arg("radius")),
  doc__tlf__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "translate",
  loffset,
  (arg("image"), arg("ox"), arg("oy"), arg("oz"), arg("mode")=0),
  doc__translate__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_VALUE(
  "variance",
  lvariance1,
  (arg("image")),
  doc__variance1__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_VALUE(
  "variance",
  lvariance2,
  (arg("image"), arg("mask")),
  doc__variance1__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "volmaxima",
  lvolmaxima,
  (arg("image"), arg("connex"), arg("param")),
  doc__volmaxima__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "volselnb",
  lvolselnb,
  (arg("image"), arg("connex"), arg("param")),
  doc__volselnb__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// NOTE: volume's using &vol

UI_WRAP_FUNCTION(
  "wshedval",
  lwshedval,
  (arg("image"), arg("connex")),
  doc__wshedval__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

UI_WRAP_FUNCTION(
  "xor",
  lxor,
  (arg("image1"), arg("image2")),
  doc__xor__c__
  );
#include BOOST_PP_UPDATE_COUNTER()

// // UI_WRAP_FUNCTION(
// //   "ccv",
// //   lccv,
// //   (arg("image"), arg("elem")),
// //   doc__ccv__c__
// //   );
// // #include BOOST_PP_UPDATE_COUNTER()

BOOST_PYTHON_MODULE(libcpp_pink)
// the final module will be named pink, but that will also contain
// some python functions, like readimage, so it will be created as
// a tree hierarchy
{

  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
  
  def("greet", greet, "Gently greets the user.");  
  vector_int_object_export();
  
  // obsolete 
  // xvimage_object_export(); // self explaining
  // deep_xvimage_object_export(); // self explaining
  // shallow_xvimage_object_export(); // self explaining

  // created as a method of the object
  //  writeimage_export(); // self explaining

  vint_object_export(); // self explaining

  wshedtopo_export(); /* erosball_export();*/  medianfilter_export();

  // the skeleton function
  skeleton2_export();  zoom_export();

  dilation_export(); erosion_export();  geodilat_export();  geoeros_export();
  opening_export();  drawcurve2D_export();
  
//  specialize_export();

  uiSqhool_object_export();
//  gradient_export();

  read_raw_image_export();  seuil_export();  plane3d_export();
//  draw_plane_export();
//  project_plane_export();
  border_export();  identifyline_export();  surimp_export();  generate_rgb_image_export();
  closing_export(); /* closeball_export();*/  minmax_export();  /* dilatball_export();*/
  asfbin_export();  skelcurv_export();
  readimage_export();  distc_export();  skelsurf_export();  toposhrink_export();
  htkern_export();  /*openball_export();*/    gradmorph_export(); mcube_export(); minima_export();
  


  // this part is not yet ready for windows
# ifdef UNIXIO
  // is this enough ? HT
  // this calls the definition in the right place
  Pimview_export();
  Pimviewlogin_export();
  Pimviewputimage_export();
  PLiarEnableDebug_export();
  Pimview_force_socket_export();
  Pimview_sendcommand_export();
# endif /* UNIXIO */

  def(
    "float2byte",
    pink::float2byte,
    ( arg("source image"), arg("mode")=2 ),
    doc__float2byte__c__
    // end of the documenation
    );
   

  def(
    "long2byte",
    pink::long2byte, 
    ( arg("image"),arg("mode"),arg("nb_new_val") ),
    doc__long2byte__c__
    // end of the documenation
    );
    
  def(
    "genball",
    pink::genball,
    ( arg("radius"), arg("dimension")=2 ),
    "Generates a discrete Euclidean ball with given radius and dimensions."
    );

  def(
    "normalize",
    &normalize<char_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );
    
  def(
    "normalize",
    &normalize<int_image, 0, 255>,
    ( arg("image"), arg("minval")=0, arg("maxval")=255),
    doc__normalize__c__
    );
    
  def(
    "normalize",
    &normalize<float_image, 0, 1>,
    ( arg("image"), arg("minval")=0, arg("maxval")=1),
    doc__normalize__c__
    );


#ifdef PINK_DEVELOPMENT
  
  
  def( "circle_tangent", &pink::gsl::circle_tangent,
       (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"), boost::python::arg("point of derivation")),
       "This function estimates the derivativ of the function given by points. It "
       " models a circle and returns its tangent at the given point."
    );

  def( "fit_circle", &pink::py_fit_circle,
       (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"),boost::python::arg("filename")=""),
       "This function fits a circle to the given points. It returns a vector for the following formula:\n"
       "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
       "If the argument filename is given, than the function will generate an example Mathematica\n"
       "code with the circle and the points."
       "The algorithm is a modified version of that in:\n"
       "Andrew Fitzgibbon, Maurizio Pilu, Robert B. Fisher, 'Direct Least Square Fitting of Ellipses', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 21, no. 5, pp. 476-480, May 1999, doi:10.1109/34.765658\n"
    );

  def( "circle_equation_to_coordinates", &pink::py_circle_equation_to_coordinates,
       (boost::python::arg("equation")),
       "This function converts the vector of equation coordinates to a vector of geometric coordinates:\n"
       "the input vector is [a,b,c,d], where\n"
       "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
       "the output vector is [center_x, center_y, r], where\n"
       "  (x-center_x)^2 + (y-center_y)^2 == r^2 "
    );

  def(
    "maxflow",
    pink::maxflow_float,
    (arg("source and sink"), arg("constraint image"), arg("iterations"), arg("tau"), arg("number of threads")=0),
    "WRITE ME!"
    );

  
  def(
    "gradient_abs",
    pink::uiGradientAbs,
    (arg("image")),
    "WRITE ME!"
    );


//  def( ""
  
#endif /* PINK_DEVELOPMENT */
  
  // here are the image objects ( char_image, short_image, int_image, float_image, double_image )
  // there is a template in pyujimage.hpp to help them export without rewritting all the declarations

  // exposing the base image function
  class_<pink_image_wrap, boost::noncopyable> ("any_image");
  
  ujoi_class_export < char_image > (
    "char_image",
    "This is the image class for 1 byte 'char' images"
    );

  ujoi_class_export < short_image > (
    "short_image",
    "This is the image class for 2 byte 'short int' images"
    );


  ujoi_class_export < int_image > ( // LONG INT is 8 byte's on 64 bit system
    "int_image",
    "This is the image class for 4 byte 'long int' images. \n"
    "Note that on 32bit systems it's equivalent with c++ 'int'. \n"
    "However while 'int' can change on 64bit systems, 'long int' \n"
    "should remain 32bit." 
    );

  ujoi_class_export < float_image > (
    "float_image",
    "This is the image class for 4 byte 'float' images"
    );

  ujoi_class_export < double_image > (
    "double_image",
    "This is the image class for 8 byte 'double' images"
    );

} /* BOOST_PYTHON_MODULE */



































// LuM end of file
