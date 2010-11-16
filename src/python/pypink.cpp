// UjoImro, 2009
// This software is meant to be free
// The pink python wrapper core file

#include <pink_python.h>

using namespace boost::python;
using namespace pink;

// maybe I'll get away with it
#include <pyujimage.hpp>
#include <pyexport.hpp>

// declarations
void py_hello_export(); 
void deep_xvimage_object_export();
void shallow_xvimage_object_export();
void xvimage_object_export();
void py_readimage_export(); 
// void py_writeimage_export();
void vint_object_export(); 
void vector_int_object_export();
void py_wshedtopo_export();
void py_erosball_export();
void py_medianfilter_export();

// skeleton function
void py_skeleton_export();

void py_zoom_export();
void py_long2byte_export();


void py_dilat_export();
void py_eros_export();
void py_geodilat_export();
void py_geoeros_export();
void py_geoeros_export();
void py_opening_export();
void py_drawcurve_export();

void py_maxflow_export();
void uiSqhool_object_export();
void py_gradient_export();
void py_float2byte_export();

void py_read_raw_image_export();
void py_seuil_export();
void insert_image_export();
void merge_max_image_export();
void py_3dplane_export();
void draw_plane_export();
void project_plane_export();
void py_border_export();
void py_identifyline_export();
void py_surimp_export();
void py_generate_rgb_image_export();
void py_closing_export();
void py_closeball_export();
void py_minmax_export();


// for wrapping every type with one function
template <class object_type, class pixel_type>
void ujoi_object_export( const char* object_name, const char* object_description );


BOOST_PYTHON_MODULE(libcpp_pink) // the final modul will be named pink, but that will also contain
                              // some python functions, like readimage, so it will be created as
                              // a tree hierarchy
{

  py_hello_export(); // the test function 'greet()'
  vector_int_object_export();
  xvimage_object_export(); // self explaining
  deep_xvimage_object_export(); // self explaining
  shallow_xvimage_object_export(); // self explaining
  py_readimage_export(); // self explaining
//  py_writeimage_export(); // self explaining
  vint_object_export(); // self explaining
  py_wshedtopo_export();
  py_erosball_export();
  py_medianfilter_export();

  // the skeleton function
  py_skeleton_export();
  
  py_zoom_export();
  py_long2byte_export();

  py_dilat_export();
  py_eros_export();
  py_geodilat_export();
  py_geoeros_export();
  py_geoeros_export();
  py_opening_export();
  py_drawcurve_export();
  
//  py_maxflow_export();

//  py_specialize_export();

  uiSqhool_object_export();
//  py_gradient_export();

  py_float2byte_export();

  py_read_raw_image_export();
  py_seuil_export();
  insert_image_export();
  merge_max_image_export();
  py_3dplane_export();
//  draw_plane_export();
//  project_plane_export();
  py_border_export();
  py_identifyline_export();
  py_surimp_export();
  py_generate_rgb_image_export();
  py_closing_export();
  py_closeball_export();
  py_minmax_export();

  UI_DEFINE_FUNCTION(
    print_image,
    print_image,
    (arg("image")),
    "Prints the values in a table. Usefull for debugging small 2D images."
    )
  
  
//   def( "cpp_circle_tangent", &pink::gsl::py_circle_tangent,
//        (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"), boost::python::arg("point of derivation")),
//        "This function estimates the derivativ of the function given by points. It "
//        " models a circle and returns its tangent at the given point."
//     );

//   def( "cpp_fit_circle", &pink::py_fit_circle,
//        (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"),boost::python::arg("filename")=""),
//        "This function fits a circle to the given points. It returns a vector for the following formula:\n"
//        "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
//        "If the argument filename is given, than the function will generate an example Mathematica\n"
//        "code with the circle and the points."
//        "The algorithm is a modified version of that in:\n"
//        "Andrew Fitzgibbon, Maurizio Pilu, Robert B. Fisher, 'Direct Least Square Fitting of Ellipses', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 21, no. 5, pp. 476-480, May 1999, doi:10.1109/34.765658\n"
//     );

//   def( "cpp_circle_equation_to_coordinates", &pink::py_circle_equation_to_coordinates,
//        (boost::python::arg("equation")),
//        "This function converts the vector of equation coordinates to a vector of geometric coordinates:\n"
//        "the input vector is [a,b,c,d], where\n"
//        "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
//        "the output vector is [center_x, center_y, r], where\n"
//        "  (x-center_x)^2 + (y-center_y)^2 == r^2 "
//     );

  
  // here are the image objects ( char_image, short_image, int_image, float_image, double_image )
  // there is a template in pyujimage.hpp to help them export without rewritting all the declarations

  ujoi_object_export < char_image, unsigned char > (
    "char_image",
    "This is the image object for 1 byte 'char' images"
    );


  ujoi_object_export < short_image, unsigned short int > (
    "short_image",
    "This is the image object for 2 byte 'short int' images"
    );


  ujoi_object_export < int_image, unsigned /*long*/ int > ( // LONG INT is 8 byte's on 64 bit system
    "int_image",
    "This is the image object for 4 byte 'long int' images. "
    "Note that on 32bit systems it's equivalent with c++ 'int'. "
    "However while 'int' can change on 64bit systems, 'long int' "
    "should remain 32bit." 
    );

  ujoi_object_export < float_image, float > (
    "float_image",
    "This is the image object for 4 byte 'float' images"
    );

  ujoi_object_export < double_image, double > (
    "double_image",
    "This is the image object for 8 byte 'double' images"
    );

};



































// LuM end of file
