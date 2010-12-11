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

#include <pink_python.h>

using namespace boost::python;
using namespace pink;

// maybe I'll get away with it
#include <pyujimage.hpp>
#include <pyexport.hpp>
#include <python_doc.h>

// declarations
// void deep_xvimage_object_export();void shallow_xvimage_object_export();
// void xvimage_object_export();
// void writeimage_export();
void vint_object_export(); void vector_int_object_export();void wshedtopo_export();
void erosball_export();void medianfilter_export();

// skeleton function
void skeleton2_export();void zoom_export();

void dilation_export(); void erosion_export(); void geodilat_export();
void geoeros_export(); void opening_export(); void drawcurve2D_export(); void maxflow_export();
void uiSqhool_object_export(); void gradient_export();

void read_raw_image_export(); void seuil_export(); void plane3d_export(); void draw_plane_export();
void project_plane_export(); void border_export(); void identifyline_export(); void surimp_export();
void generate_rgb_image_export(); void closing_export(); void closeball_export(); void minmax_export();
void dilatball_export(); void asfbin_export(); void ptcurve_export();
void skelcurv_export(); void ptend_export(); void distc_export();
void readimage_export(); void skelsurf_export(); void toposhrink_export(); void htkern_export();
void openball_export(); void gradmorph_export(); void mcube_export(); void minima_export();


// not sure this is the correct thing to do here HT
void imview_export();

// for wrapping every type with one function
// template <class image_type>
// void ujoi_class_export( const char* object_name, const char* object_description );


BOOST_PYTHON_MODULE(libcpp_pink) // the final modul will be named pink, but that will also contain
                              // some python functions, like readimage, so it will be created as
                              // a tree hierarchy
{

  def("greet", greet, "Gently greets the user.");
  
  vector_int_object_export();
  
  // obsolete 
  // xvimage_object_export(); // self explaining
  // deep_xvimage_object_export(); // self explaining
  // shallow_xvimage_object_export(); // self explaining

  // created as a method of the object
  //  writeimage_export(); // self explaining

  vint_object_export(); // self explaining

  wshedtopo_export();  erosball_export();  medianfilter_export();

  // the skeleton function
  skeleton2_export();  zoom_export();

  dilation_export(); erosion_export();  geodilat_export();  geoeros_export();
  opening_export();  drawcurve2D_export();
  
//  maxflow_export();

//  specialize_export();

  uiSqhool_object_export();
//  gradient_export();

  read_raw_image_export();  seuil_export();  plane3d_export();
//  draw_plane_export();
//  project_plane_export();
  border_export();  identifyline_export();  surimp_export();  generate_rgb_image_export();
  closing_export();  closeball_export();  minmax_export();  dilatball_export();
  asfbin_export();  ptcurve_export();  skelcurv_export();
  ptend_export();  readimage_export();  distc_export();  skelsurf_export();  toposhrink_export();
  htkern_export();  openball_export();    gradmorph_export(); mcube_export(); minima_export();
  


  UI_DEFINE_FUNCTION(
    inverse,
    pink::inverse,
    ( arg("image") ),
    doc__inverse__c__
// end of the documenation
  );    

  
  // is this enough HT ?  
  imview_export();



  
  UI_DEFINE_FUNCTION(
    print_image,
    print_image,
    (arg("image")),
    "Prints the values in a table. Usefull for debugging small 2D images."
    );
  

  
  UI_DEFINE_FUNCTION(
    insert_image, 
    insert_image, 
    ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
    "this function inserts an image to another one. The "
    "shift vector indicates the position of the lower corner "
    "of the new image. If the image doesn't fit into the big one, "
    "it is cropped."
    );
  

  
  UI_DEFINE_FUNCTION(
    merge_max_image, 
    merge_max_image, 
    ( arg("big_image"), arg("small_image"), arg("shift_vector") ),
    "this function inserts an image to another one. The "
    "shift vector indicates the position of the lower corner "
    "of the new image. If the image doesn't fit into the big one, "
    "it is cropped. During the insertion the pixels are compared "
    "and the bigger value is conserved"
    );



  UI_DEFINE_FUNCTION(
    frame,
    frame,
    ( arg("image"), arg("with value") ),
    "This function takes an image, and in the result it set's it most outer rectangle to the given value."
    );
  
  UI_DEFINE_FUNCTION(
    frame_around,
    frame_around,
    ( arg("image"), arg("with value") ),
    "This function takes an image, and in the result and adds a frame around it, with"
    " a given value. THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
    );


    UI_DEFINE_FUNCTION(
    frame_remove,
    frame_remove,
    ( arg("image") ),
    "This function takes an image, and in the result and removes the most outer rectangle from the result. "
    "THIS FUNCTION CHANGES THE RESULTING IMAGE'S SIZE."
    );

    UI_DEFINE_FUNCTION(
      max,
      immap_max,
      ( arg("image1"), arg("image2") ),      
    "Generates an image result[i]:=max(image1[i],image2[i])"
    );

    UI_DEFINE_FUNCTION(
      min,
      immap_min,
      ( arg("image1"), arg("image2") ),      
      "Generates an image result[i]:=min(image1[i],image2[i])"
    );

    def( "ptisolated",
         &make_function<char_image, int, &lptisolated>,
         ( arg("image"), arg("connexity") ),
         doc__ptisolated__c__
      );

    def( "ptjunction",
         &make_function<char_image, int, &lptjunction>,
         ( arg("image"), arg("connexity") ),
         doc__ptjunction__c__
      );

    def( "selectcomp",
         &make_function<char_image, int, int, int, int, &lselectcomp>,
         ( arg("image"), arg("connexity"), arg("x"), arg("y"), arg("z") ),
         doc__selectcomp__c__
      );

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

    UI_DEFINE_ONE_FUNCTION(
      long2byte,
      pink::long2byte, 
      ( arg("image"),arg("mode"),arg("nb_new_val") ),
      doc__long2byte__c__
      // end of the documenation
      );


    def(
      "holeclosing",
      &make_function<char_image, int, int, &lfermetrous3dbin>,
      ( arg("image"), arg("connexity"), arg("holesize") ),
      doc__holeclosing__c__
      );

    def(
      "holeclosing",
      &make_function<char_image, char_image, int, int, &lfermetrous3dbin2>,
      ( arg("image"), arg("guiding image") ,arg("connexity"), arg("holesize") ),
      doc__holeclosing__c__
      );

    def(
      "lambdaskel",
      &make_function< char_image, char_image, int, int, &llambdakern >,
      ( arg("image"), arg("imcond"), arg("connexity"), arg("lambda") ),
      doc__lambdaskel__c__
      );

    def(
      "lambdaskel",
      &make_function< char_image, int, int, &llambdakern_short >,
      ( arg("image"), arg("connexity"), arg("lambda") ),
      doc__lambdaskel__c__
      );

    
    def(
      "rankfilter",
      &make_function< char_image, char_image, int, int, double, &lfiltreordre >,
      ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("r") ),
      doc__rankfilter__c__
      );

    def(
      "rankfilter",
      &make_function< char_image, char_image, int, int, int, double, &lfiltreordre3d >,
      ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z"), arg("r") ),
      doc__rankfilter__c__
      );


    def(
      "tuf",
      &make_function< char_image, int, int, &ltuf >,
      ( arg("image"), arg("min connex"), arg("radius") ),
      doc__tuf__c__
      );

    def(
      "taf",
      &make_function< char_image, int, int, int, int, &ltaflambda >,
      ( arg("image"), arg("min connex"), arg("radius"), arg("lambdapics"), arg("lambdapuits") ),
      doc__taf__c__
      );

    def(
      "watershed",
      &make_function< char_image, char_image, int, &lwshedtopobin >,
      ( arg("image"), arg("mark"), arg("connexity") ),
      doc__watershed__c__
      );

    def(
      "heightmaxima",
      &make_function< char_image, int, int, &lheightmaxima >,
      ( arg("image"), arg("connexity"), arg("height") ),
      doc__heightmaxima__c__
      );
    
    def(
      "heightminima",
      &make_function< char_image, int, int, &lheightminima >,
      ( arg("image"), arg("connexity"), arg("height") ),
      doc__heightminima__c__
      );
    
    UI_DEFINE_ONE_FUNCTION(
      genball,
      pink::genball,
      ( arg("radius"), arg("dimension")=2 ),
      "Generates a discreet eucledian ball with given radius and dimensions."
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


    UI_DEFINE_FUNCTION(
      extractplane,
      pink::extractplane,
      ( arg("image"), arg("number of the plane"), arg("mode") ),
      doc__extractplane__c__
      );
    
    
//   def( "cpp_circle_tangent", &pink::gsl::circle_tangent,
//        (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"), boost::python::arg("point of derivation")),
//        "This function estimates the derivativ of the function given by points. It "
//        " models a circle and returns its tangent at the given point."
//     );

//   def( "cpp_fit_circle", &pink::fit_circle,
//        (boost::python::arg("x coordinates"), boost::python::arg("y coordinates"),boost::python::arg("filename")=""),
//        "This function fits a circle to the given points. It returns a vector for the following formula:\n"
//        "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
//        "If the argument filename is given, than the function will generate an example Mathematica\n"
//        "code with the circle and the points."
//        "The algorithm is a modified version of that in:\n"
//        "Andrew Fitzgibbon, Maurizio Pilu, Robert B. Fisher, 'Direct Least Square Fitting of Ellipses', IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 21, no. 5, pp. 476-480, May 1999, doi:10.1109/34.765658\n"
//     );

//   def( "cpp_circle_equation_to_coordinates", &pink::circle_equation_to_coordinates,
//        (boost::python::arg("equation")),
//        "This function converts the vector of equation coordinates to a vector of geometric coordinates:\n"
//        "the input vector is [a,b,c,d], where\n"
//        "  a*x^2 + a*y^2 + b*x + c*y + d == 0\n"
//        "the output vector is [center_x, center_y, r], where\n"
//        "  (x-center_x)^2 + (y-center_y)^2 == r^2 "
//     );

  
  // here are the image objects ( char_image, short_image, int_image, float_image, double_image )
  // there is a template in pyujimage.hpp to help them export without rewritting all the declarations

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

};



































// LuM end of file
