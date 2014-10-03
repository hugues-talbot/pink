/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The Pink Segmentation

#include "pyexport.hpp"

#include "uiFlow.hpp"
#include "lattribvol.h"
#include "python_doc.h"
#include "lattribarea.h"
#include "lattribheight.h"
#include "lsegreconsheight.h"

void
pysegmentation ()
{
  using pink::allocdef;
  using pink::resultdef;
  using pink::functiondef;
  
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;

  // make "from mypackage.segmentation import <whatever>" work
  object segmentation_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.segmentation"))));
  
  // make "from mypackage import segmentation" work
  scope().attr("segmentation") = segmentation_module;
  
  // set the current scope to the new sub-module
  scope segmentation_scope = segmentation_module;

  functiondef( "segmentheight", lsegmentheight, ( arg("image"), arg("connexity"), arg("height"), arg("mode")=0 ), doc__segmentheight__c__ );

  functiondef( "reconsheight", lsegreconsheight, ( arg("image"), arg("connexity"), arg("height") ), doc__segmentheight__c__ );
  
  functiondef( "segmentarea", lsegmentarea, ( arg("image"), arg("connexity"), arg("area"), arg("mode") ), doc__segmentarea__c__ );
    
  functiondef( "segmentvol", lsegmentvol, ( arg("image"), arg("connexity"), arg("vol"), arg("mode") ), doc__segmentvol__c__ );

  def( "maxflow", pink::pymaxflow<pink::float_image>, ( arg("Source and Sink"), arg("Measure (Gradient Image)"), arg("number of iterations"), arg("tau")=0.132, arg("number of threads")=1, arg("packet_size")=100, arg("verbose")=false, arg("debug")=false ), doc__maxflow__cpp__ );
 
  import_array();  // numpy initialization
  
  return;  
} // pysegmentation 

