/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_development.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymaxflow.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



void py_maxflow_export(){

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
  

} /* py_maxflow_export */

// LuM end of file
