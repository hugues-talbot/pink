/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// This macro exports a template function to all possible image types
#ifndef PYEXPORT_HPP
#define PYEXPORT_HPP

// Defines the longest pink function to wrap. Can be increased
#define MAX_PARAMETERS 15

#include <boost/preprocessor.hpp>
#include "ui_wrap_function.hpp"




#define UI_EXPORT_FUNCTION(FNAME, FN, ARGS, DOC)                        \
                                                                        \
  void FNAME##_export()                                                 \
  {									\
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<char_image>,   \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<short_image>,  \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<int_image>,    \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<float_image>,  \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN)<double_image>, \
         ARGS,                                                          \
         DOC                                                            \
      );                                                                \
  }									


#define UI_DEFINE_FUNCTION( FNAME, FN, ARGS, DOC )                      \
  {									\
    def( FNAME, BOOST_PP_EXPAND(FN)<char_image>,                        \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<short_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<int_image>,                         \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<float_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<double_image>,                      \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
                                                                        \
  }									 

#define UI_WRAP_CPP( FNAME, FN, ARGS, DOC )                             \
  void BOOST_PP_CAT(export_function, BOOST_PP_COUNTER)()                \
  {		     							\
    def( FNAME, BOOST_PP_EXPAND(FN)<char_image>,                        \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<short_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<int_image>,                         \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<float_image>,                       \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
    def( FNAME, BOOST_PP_EXPAND(FN)<double_image>,                      \
         ARGS,								\
         DOC                                                            \
      );                                                                \
                                                                        \
                                                                        \
  }									 


#define UI_EXPORT_ONE_FUNCTION( FNAME, FN, ARGS, DOC )                  \
                                                                        \
  void FNAME##_export()                                                 \
  {									\
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN),               \
         ARGS,                                                          \
         DOC                                                            \
      );								\
  }									

#define UI_DEFINE_ONE_FUNCTION( FNAME, FN, ARGS, DOC )                  \
                                                                        \
  {									\
    def( BOOST_PP_STRINGIZE(FNAME), &BOOST_PP_EXPAND(FN),               \
	 ARGS,								\
	 DOC                                                            \
      );								\
    									\
  }									


  
// // void py_maxflow_export(){

// //   def( "cpp_maxflow_float", &maxflow_float,
// //        args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
// //        "maximal flows, returns the image (potential field) \n"
// //        " \n"
// // // end of the documenation
// //     );

// //   def( "cpp_maxflow_cami_float", &maxflow_cami,
// //        args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
// //        "maximal flows, returns the flow (float array of size pixels * dimension ) \n"
// //        " \n"
// // // end of the documenation
// //     );
  

// // } /* py_maxflow_export */

//   template < class image_type >
//   class convert_if
//   {
//   public:
//     typedef typename boost::mpl::if_< boost::is_base_of<pink::pink_image, image_type>,
//                                       xvimage*,
//                                       image_type >::type type;
//   }; /* convert_if */

// // **********************************************************
// // **********************************************************
// // **********************************************************
// // ***** THIS IS REPLACED WITH THE PREPROCESSING MACRO ******
// // **********************************************************
// // **********************************************************
// // **********************************************************




// // this macro is for classical pink functions
// // like
// // int mcfunction( image, param1, ... )
// BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_FUNCTION, ~)


// // this function is for pink functions which supply the result in a different
// // image like
// // int mcfunction( image, param1, ..., paramn, result )
// BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_RESULT, ~)




template <class image_type>
xvimage * can_be_null( image_type & image )
{
  if (image.isnull())
  {
    return NULL;        
  }
  else /* NOT image.isnull() */
  {
    return image.get_output();        
  }      
} /* can_be_null */

#define EXPORT_FUNCTION(z,n,text)               \
  export_function##n();                         
  
#define CALL_EXPORTED_FUNCTIONS(n)              \
  BOOST_PP_REPEAT(n, EXPORT_FUNCTION, ~)


// // #define UI_EXPORT_PINK_CHANGE_IMAGE(fname, funcname, doc)               \
// //   template <class Fn>                                                   \
// //   void export_function##BOOST_PP_COUNTER()(char * fname, Fn fn, char * doc) \
// //   {                                                                     \
  
// //   }                                                                     
  







#endif /* PYEXPORT_HPP */
// LuM end of file
