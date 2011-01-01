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

#define UI_EXPORT_FUNCTION( function_name, function, args, comment )	\
                                                                        \
  void function_name##_export()                                         \
  {									\
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<char_image>, \
         args,                                                          \
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<short_image>, \
         args,                                                          \
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<int_image>, \
         args,                                                          \
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<float_image>, \
         args,                                                          \
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<double_image>, \
         args,                                                          \
         comment                                                        \
      );                                                                \
  }									


#define UI_DEFINE_FUNCTION( function_name, function, args, comment )	\
  {									\
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<char_image>, \
         args,								\
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<short_image>, \
         args,								\
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<int_image>, \
         args,								\
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<float_image>, \
         args,								\
         comment                                                        \
      );                                                                \
                                                                        \
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<double_image>, \
         args,								\
         comment                                                        \
      );                                                                \
                                                                        \
                                                                        \
  }									


#define UI_EXPORT_ONE_FUNCTION( function_name, function, args, comment ) \
                                                                        \
  void function_name##_export()                                         \
  {									\
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function), \
         args,                                                          \
         comment							\
      );								\
  }									

#define UI_DEFINE_ONE_FUNCTION( function_name, function, args, comment ) \
                                                                        \
  {									\
    def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function), \
	 args,								\
	 comment							\
      );								\
    									\
  }									


  
// void py_maxflow_export(){

//   def( "cpp_maxflow_float", &maxflow_float,
//        args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
//        "maximal flows, returns the image (potential field) \n"
//        " \n"
// // end of the documenation
//     );

//   def( "cpp_maxflow_cami_float", &maxflow_cami,
//        args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
//        "maximal flows, returns the flow (float array of size pixels * dimension ) \n"
//        " \n"
// // end of the documenation
//     );
  

// } /* py_maxflow_export */

  template < class image_type >
  class convert_if
  {
  public:
    typedef typename boost::mpl::if_< boost::is_base_of<pink::pink_image, image_type>,
                                      xvimage*,
                                      image_type >::type type;
  }; /* convert_if */

// **********************************************************
// **********************************************************
// **********************************************************
// ***** THIS IS REPLACED WITH THE PREPROCESSING MACRO ******
// **********************************************************
// **********************************************************
// **********************************************************


// template < class image_type,
//            class param1_type,
//            int (*mcfunction)( typename convert_if<image_type>::type,
//                               typename convert_if<param1_type>::type )
//            >
// image_type make_function( const image_type & image,
//                           param1_type param1 )
// {
//   image_type result;
//   result.copy(image);
  
//   if (!mcfunction(result, param1))
//   {
//     error("mcfunction failed");    
//   }
  
//   return result;  
// } /* make_function */


// Defines the longest pink function to wrap. Can be increased
#define MAX_PARAMETERS 10

#define                                         \
  PARAM(z, n, text)                             \
  param_type##n param##n

#define                                         \
  CONVERT_IF(z, n, text)                        \
  typename convert_if<param_type##n>::type

//BOOST_PP_ENUM_PARAMS( MAX_PARAMETERS, class param_type)

#define                                                                 \
  MAKE_FUNCTION(z, n, text)                                             \
  template < class image_type,                                          \
  BOOST_PP_ENUM_PARAMS(n, class param_type ) BOOST_PP_COMMA_IF(n)       \
  int (*mcfunction) (                                                   \
    typename convert_if<image_type>::type BOOST_PP_COMMA_IF(n)          \
    BOOST_PP_ENUM(n, CONVERT_IF, ~)                                     \
    )                                                                   \
  >                                                                     \
  image_type make_function( const image_type & image BOOST_PP_COMMA_IF(n) \
                            BOOST_PP_ENUM(n, PARAM, ~)                  \
    )                                                                   \
  {                                                                     \
  image_type result;                                                    \
  result.copy(image);                                                   \
                                                                        \
  if (!mcfunction(result BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, param))) \
  {                                                                     \
  error("mcfunction failed");                                           \
  }                                                                     \
                                                                        \
  return result;                                                        \
  }

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

// this macro is for classical pink functions
// like
// int mcfunction( image, param1, ... )
BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_FUNCTION, ~)


// this function is for pink functions which supply the result in a different
// image like
// int mcfunction( image, param1, ..., paramn, result )
BOOST_PP_REPEAT(MAX_PARAMETERS, MAKE_RESULT, ~)




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


template <class image_type>
class CAN_BE_NULL : public image_type
{
public:

  virtual operator xvimage* ()
    {
      return this->get_output();      
    } /* cast operator xvimage* */
  
  virtual xvimage* get_output()
  {
    if (this->isnull())
    {
      return NULL;      
    }
    else /* NOT this->isnull() */
    {
      return this->image_type::get_output();      
    } /* NOT this->isnull() */    
    
  } /* get_output() */
  
}; /* class can_be_null */

template <class image_type>
class only_2D : public image_type
{
public:  
  virtual operator xvimage* ()
    {
      return this->get_output();      
    } /* cast operator xvimage* */
  
  virtual xvimage* get_output()
  {
    if (this->get_size().size()!=2)
    {
      error("This function or parametrization  only accepts 2D images");      
    }
    else /* NOT this->isnull() */
    {
      return this->image_type::get_output();      
    } /* NOT this->isnull() */    
    
  } /* get_output() */  
  
}; /* class only_2D */


template <class image_type>
class only_3D : public image_type
{
public:  
  virtual operator xvimage* ()
    {
      return this->get_output();      
    } /* cast operator xvimage* */
  
  virtual xvimage* get_output()
  {
    if (this->get_size().size()!=3)
    {
      error("This function or parametrization only accepts 3D images");      
    }
    else /* NOT this->isnull() */
    {
      return this->image_type::get_output();      
    } /* NOT this->isnull() */    
    
  } /* get_output() */  
  
}; /* class only_2D */



#endif /* PYEXPORT_HPP */
// LuM end of file
