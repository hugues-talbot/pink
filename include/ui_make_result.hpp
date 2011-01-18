/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_MAKE_RESULT_HPP
#define UI_MAKE_RESULT_HPP

#include <boost/preprocessor.hpp>
#include "ui_convert_if.hpp"
#include "uiFibreTypes.h"

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ui_make_result.hpp: " << msg; call_error(fullmessage.str());}


#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */

// this header defines a function wrapper make_function
/*! \file ui_make_function.hpp
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
*/



#define                                         \
  PARAM(z, n, text)                             \
  param_type##n param##n

#define                                         \
  CONVERT_IF(z, n, text)                        \
  typename convert_if<param_type##n>::type

//BOOST_PP_ENUM_PARAMS( MAX_PARAMETERS, class param_type)


#define                                                                 \
  MAKE_RESULT(z, n, text)                                               \
  template < class image_type,                                          \
  BOOST_PP_ENUM_PARAMS(n, class param_type ) BOOST_PP_COMMA_IF(n)       \
  int (*mcfunction) (                                                   \
    typename convert_if<image_type>::type,                              \
    BOOST_PP_ENUM(n, CONVERT_IF, ~) BOOST_PP_COMMA_IF(n)                \
    typename convert_if<image_type>::type                               \
    )                                                                   \
  >                                                                     \
  image_type make_result( image_type image BOOST_PP_COMMA_IF(n)         \
                          BOOST_PP_ENUM(n, PARAM, ~)                    \
    )                                                                   \
  {                                                                     \
  image_type result;                                                    \
  result.copy(image);                                                   \
                                                                        \
  if (!mcfunction(image, BOOST_PP_ENUM_PARAMS(n, param) BOOST_PP_COMMA_IF(n) result)) \
  {                                                                     \
  error("mcfunction failed");                                           \
  }                                                                     \
                                                                        \
  return result;                                                        \
  }

namespace pink
{
  namespace python
  {
    BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_RESULT, ~)
  } /* namespace python */
} /* namespace pink */


// cleaning up after us
#undef CONVERT_IF
#undef PARAM
#undef MAKE_FUNCTION
#undef error
#ifdef CLEAN_UP_MAX_PARAMETERS
#  undef MAX_PARAMETERS
#endif /* CLEAN_UP_MAX_PARAMETERS */

// the generated code is:

// template <
//   class image_type,
//   int (*mcfunction) (
//     typename convert_if<image_type>::type,
//     typename convert_if<image_type>::type ) >
// image_type make_result( image_type image )
// {
//   image_type result;
//   result.copy(image);
//   if (!mcfunction(image, result)) {
//     error("mcfunction failed");
//   }
//   return result;
// }

// template <
//   class image_type,
//   class param_type0 ,
//   int (*mcfunction) (
//     typename convert_if<image_type>::type,
//     typename convert_if<param_type0>::type ,
//     typename convert_if<image_type>::type ) >
// image_type make_result(
//   image_type image ,
//   param_type0 param0
//   )
// {
//   image_type result;
//   result.copy(image);
//   if (!mcfunction(image, param0 , result)) {
//     error("mcfunction failed");
//   }
//   return result;
// }









#endif /* UI_MAKE_RESULT_HPP */
// LuM end of file
