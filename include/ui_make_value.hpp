/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_MAKE_VALUE_HPP
#define UI_MAKE_VALUE_HPP

#include <boost/preprocessor.hpp>
#include "ui_convert_if.hpp"
#include "uiFibreTypes.h"

using namespace pink::python;


#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ui_make_value.hpp: " << msg; call_error(fullmessage.str());}

//#define MAX_PARAMETERS 3

#ifndef MAX_PARAMETERS
#  define MAX_PARAMETERS 15
#  define CLEAN_UP_MAX_PARAMETERS
#endif /* MAX_PARAMETERS */


#define                                         \
  PARAM(z, n, text)                             \
  T##n t##n

#define                           \
  CONVERT_IF(z, n, text)          \
  typename convert_if<T##n>::type

//BOOST_PP_ENUM_PARAMS( MAX_PARAMETERS, class param_type)

#define                                                                 \
  MAKE_VALUE(z, n, text)                                                \
  template < class RT,                                                  \
  BOOST_PP_ENUM_PARAMS(n, class T ) BOOST_PP_COMMA_IF(n)                \
  RT (*mcfunction) (                                                    \
    BOOST_PP_ENUM(n, CONVERT_IF, ~)                                     \
    )                                                                   \
  >                                                                     \
  RT make_value( BOOST_PP_ENUM(n, PARAM, ~)                             \
    )                                                                   \
  {                                                                     \
  RT result;                                                            \
                                                                        \
  result = mcfunction(BOOST_PP_ENUM_PARAMS(n, t));                      \
                                                                        \
  return result;                                                        \
  };                                                                    \
  


namespace pink
{
  namespace python
  {

    BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_VALUE, ~)

  } /* namespace python */
} /* namespace pink */

    
// cleaning up after us
#undef CONVERT_IF
#undef PARAM
#undef MAKE_VALUE
#undef error
#ifdef CLEAN_UP_MAX_PARAMETERS
#  undef MAX_PARAMETERS
#endif /* CLEAN_UP_MAX_PARAMETERS */

// the generated code is:

// namespace pink
// {
//   namespace python
//   {
//     template < class RT, RT (*mcfunction) ( ) > RT make_value( )
//     {
//       RT result;
//       result = mcfunction();
//       return result;
//     };
//     template < class RT, class T0 , RT (*mcfunction) ( typename convert_if<T0>::type ) > RT make_value( T0 t0 )
//     {
//       RT result;
//       result = mcfunction( t0);
//       return result;
//     };
//     template < class RT, class T0 , class T1 , RT (*mcfunction) ( typename convert_if<T0>::type , typename convert_if<T1>::type ) > RT make_value( T0 t0 , T1 t1 )
//     {
//       RT result;
//       result = mcfunction( t0 , t1);
//       return result;
//     };
//   }
// }








#endif /* UI_MAKE_VALUE_HPP */
// LuM end of file
