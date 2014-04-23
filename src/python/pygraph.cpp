/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// this file contains the wrapper for Pink's skeleton class


#include "pyexport.hpp"

#include "ljones.h"
#include "ujimage.hpp"
#include "ui_graph.hpp"
#include "python_doc.h"
#include "ui_skeleton.hpp"


void
pygraph()
{

  using pink::functiondef;
  using boost::python::arg;
  using boost::python::def;
  using boost::python::args;

        
  boost::python::class_< pink::graph_t<index_t> > (
    "graph_t",
    "This is the class for manipulating graphs.",
    boost::python::init<>(
      args("self"), "Creates an empty graph object." )
    )   

    .def( boost::python::init< std::string > (
            args("self","filename"), "Reads a skeleton from a file." )
      )
        
    .def( "writegraph", &pink::graph_t<index_t>::writegraph,
          ( arg("filename") ),
          "WRITE ME!!!"
      )

    .def( "writeps", &pink::graph_t<index_t>::writeps,
          ( arg("filename"), arg("scale")=20 ),
          "WRITE ME!!!"
      )

    .def( "clone", &pink::graph_t<index_t>::clone,
          "WRITE ME!!!"
      )

    .def( "nb_edges", &pink::graph_t<index_t>::nb_edges,
          "WRITE ME!!!"
      )

    .def( "nb_maxedges", &pink::graph_t<index_t>::nb_maxedges,
          "WRITE ME!!!"
      )

    .def( "nb_nodes", &pink::graph_t<index_t>::nb_nodes,
          "WRITE ME!!!"
      )
    ; // pink::skel_t (skeleton)
    
  def( "skel2graph", pink::skel2graph<index_t>, ( arg("skeleton"), arg("mode"), arg("param") ), doc__skel2graph__c__ );

  def( "skel2graph", pink::skel2graph_short<index_t>, ( arg("skeleton"), arg("mode")=0 ), doc__skel2graph__c__ );


  functiondef( "jones", ljones, (arg("image"), arg("connex")), doc__jones__c__ );

  import_array();  // numpy initialization

  return;
  
} // pygraph
        


//LuM end of file
