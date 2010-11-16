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

#define UI_EXPORT_FUNCTION( function_name, function, args, comment )	\
									\
  void function##_export()						\
  {									\
  def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<char_image>, \
       args,								\
       comment								\
    );									\
									\
  def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<short_image>, \
       args,								\
       comment								\
    );									\
  									\
  def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<int_image>, \
       args,								\
       comment								\
    );									\
									\
  def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<float_image>, \
       args,								\
       comment								\
    );									\
									\
  def( BOOST_PP_STRINGIZE(function_name), &BOOST_PP_EXPAND(function)<double_image>, \
       args,								\
       comment								\
    );									\
									\
									\
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
  void function##_export()						\
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










// LuM end of file
