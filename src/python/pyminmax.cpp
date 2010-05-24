// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pymaxflow.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



void py_minmax_export(){

  def( "cpp_maxflow_float", &maxflow_float,
       args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
       "maximal flows, returns the image (potential field) \n"
       " \n"
// end of the documenation
    );

  def( "cpp_maxflow_cami_float", &maxflow_cami,
       args("source and sink", "boundaries", "iteration", "tau", "the number of threads"),
       "maximal flows, returns the flow (float array of size pixels * dimension ) \n"
       " \n"
// end of the documenation
    );
  

};
