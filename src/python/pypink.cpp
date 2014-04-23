/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "pyexport.hpp"

#include "lhello.h"
#include "lseltopo.h"
#include "python_doc.h"

/*! \file pypink.c
  You can export functions from Pink to Python in one of the following formats:
  
  * In C
  int32_t pink_c_function( xvimage * image, type_1 var_1, type_2 var_2, ..., type_n var_n)
  struct xvimage * pink_c_function( type_0 var_0, type_1 var_1, type_2 var_2, ..., type_n var_n)
  int32_t pink_c_function( const xvimage * image, type_1 var_1, type_2 var_2, ..., type_n var_n, xvimage * result )
  result_t pink_c_function(type_0 var_0, type_1 var_1, type_2 var_2, ..., type_n var_n) // note result_t is NOT an image
  
  You may use printf and you may call exit(1) if there's an error.
  type_k can be any type that python recognizes (int, float, ...) and xvimage*
  The return value is 1 on success and 0 otherwise. To export this function
  you should include a function call in pypink.cpp of the form

  allocdef( const char * function name, Fn function_pointer, arg_t arguments' description, const char * documentation )

  Example:
  allocdef( "ptisolated",
  lptisolated,
  ( arg("image"), arg("connexity") ),
  doc__ptisolated__c__
  );

  You can use the following functions (in the same order as the function declarations above):

  functiondef = calculate the result in the first parameter
  allocdef    = allocates the result image and returns a pointer
  resultdef   = calculates the result in the final argument (which is preallocated for the operator before the call )
  exportdef   = does not create any image result. This means it does not allocate any image, and does not alter any of it's parameters
                this type of function is used by the exporting functions ( writeimage, etc...)
  * in C++

  You should write your function in format WRITE ME!!!!
*/

// declarations
// void deep_xvimage_object_export();void shallow_xvimage_object_export();
// void xvimage_object_export();
// void writeimage_export();
void vint_object_export(); void vector_int_object_export();void wshedtopo_export();
/*void erosball_export()*/;void medianfilter_export();

// skeleton function
void skeleton2_export();void zoom_export();

// Fast morpho operators
void fdilaterect_export();
void fdilatepoly_export();
void feroderect_export();
void ferodepoly_export();
void fcloserect_export();
void fclosepoly_export();
void fopenrect_export();
void fopenpoly_export();
// opening by unions of lines
void openbun_export();
void closebin_export();

void read_raw_image_export(); void seuil_export(); void plane3d_export(); void draw_plane_export();
void project_plane_export(); void border_export(); void identifyline_export(); void surimp_export();
void generate_rgb_image_export(); /*void closeball_export();*/ void minmax_export();
/*void dilatball_export();*/ void asfbin_export();
void skelcurv_export(); void distc_export();
void readimage_export(); void skelsurf_export(); void toposhrink_export(); void htkern_export();
/*void openball_export();*/ void gradmorph_export(); void mcube_export(); void minima_export();
void complex_export(); void fft_export(); void mult_export(); void pymaxflow();

//!!!!!! juliette
// int short2long(xvimage *image, xvimage *result);
// int long2short(xvimage *image, xvimage *result);
// int imferode_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
// int imfdilat_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
// int imfopen_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);
// int imfclose_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

// skeleton
void pyskeleton();

// graphs
void pygraph();

// this part is not yet ready for windows
#ifdef UNIXIO
// not sure this is the correct thing to do here HT
// This is the prototype of the automatically generated export function
void Pimview_export(); void Pimviewlogin_export(); void Pimviewputimage_export();
void PLiarEnableDebug_export() ; void PLiarDisableDebug_export(); void Pimview_force_socket_export();
void Pimview_sendcommand_export();
#endif /* UNIXIO */

void pyio();
void pystat();
void pyutil();
void pygeom();
void pytopo();
void pystat();
void pygraph();
void pyhisto();
void pywshed();
void pysignal();
void pymorpho();
void pycomplex();
void pyarithmetic();
void pyprimitives();
void pysegmentation();

BOOST_PYTHON_MODULE(libcpp_pink)
// the final module will be named pink, but that will also contain
// some python functions, like readimage, so it will be created as
// a tree hierarchy
{
# ifdef UJIMAGE_DEBUG
  std::cerr << "Pink was compiled in debug level " << UJIMAGE_DEBUG << ". You will see various debug messages.\n" <<  std::endl;
# endif /* UJIMAGE_DEBUG */

  boost::python::def("greet", greet, "Gently greets the user.");
  
//   // submodules
//   // motivation:
//   // http://bfroehle.com/2011/07/boost-python-submodules/
//   // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
//   boost::python::object package = boost::python::scope();
//   package.attr("__path__") = "libcpp_pink";

  
// //   wshedtopo_export(); /* erosball_export();*/  medianfilter_export();

// //   // the skeleton function
// //   skeleton2_export();  zoom_export();

// //   dilation_export(); erosion_export();  geodilat_export();  geoeros_export();
// //   opening_export();  drawcurve2D_export();
// //   // liar operator functions
// //   fdilaterect_export();
// //   fdilatepoly_export();
// //   feroderect_export();
// //   ferodepoly_export();
// //   fcloserect_export();
// //   fclosepoly_export();
// //   fopenrect_export();
// //   fopenpoly_export();
  
// //   openbun_export();
// //   closebin_export();

// //   read_raw_image_export();  seuil_export();  plane3d_export();
// // //  draw_plane_export();
// // //  project_plane_export();
// //   border_export();  identifyline_export();  surimp_export();  generate_rgb_image_export();

//minmax_export();  /* dilatball_export();*/
// //   asfbin_export();  skelcurv_export();
// //   readimage_export();  distc_export();  skelsurf_export();  toposhrink_export();
// //   htkern_export();  /*openball_export();*/    gradmorph_export(); mcube_export(); minima_export();
// //   complex_export(); fft_export();  mult_export();  numpy_export();


//   // this part is not yet ready for windows
// # ifdef UNIXIO
//   // is this enough ? HT
//   // this calls the definition in the right place

//   // Pimview_export();
//   // Pimviewlogin_export();
//   // Pimviewputimage_export();
//   // PLiarEnableDebug_export();
//   // Pimview_force_socket_export();
//   // Pimview_sendcommand_export();

// # endif /* UNIXIO */

  pyio();
  pygeom();
  pyutil();  
  pytopo();
  pystat();  
  pygraph();
  pyhisto();
  pywshed();  
  pysignal();  
  pymorpho();
  pycomplex();  
  pyarithmetic();
  pyprimitives();
  pysegmentation();
  
  import_array();  // numpy initialization

} /* BOOST_PYTHON_MODULE */

// LuM end of file
