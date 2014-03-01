/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pink_python.h"

#include "lfmm.h"
#include "pde_toolbox.h"

using boost::python::arg;
using boost::python::def;

boost::python::list
pylfmm(
  pink::int_image seeds,
  pink::float_image speeds,
  index_t stop,
  float threshold )
{

  index_t  rs, cs, ss, ndim;
  int32_t *SeedIn,  *SeedOut, dim[4];
  float   *SpeedIn, *SpeedOut;
  
  SeedIn  = seeds.get(); //SLONGDATA(seeds);
  SpeedIn = speeds.get(); // FLOATDATA(speed);
    
  pink::int_image voronoi;
  voronoi = seeds.clone();  
  
  pink::float_image distance;
  distance = speeds.clone();
    
  SeedOut  = voronoi.get();
  SpeedOut = distance.get();

  /* careful: rowsize is the size of a row => nx */
  rs = seeds.rows();  
  cs = seeds.cols();
  ss = seeds.depth();

#if UJIMAGE_DEBUG > 1
  fprintf(stderr, "dimensions: [%d x %d x %d]\n", rs, cs, ss);
#endif

  ndim = seeds.size().size();  
  // ndim = 2;
  // if (ss > 1)
  //   ++ndim;
    
  dim[0] = rs ; dim[1] = cs ; dim[2] = ss;

  index_t error;  
  if (error = lfmmdist(SeedIn, SeedOut, dim, ndim, SpeedIn, stop, threshold, SpeedOut) != 0)
  {
    std::cout << "function lfmmdist failed with error code " << error << std::endl;
    pink_error("function lfmmdist failed"); 
  }

  boost::python::list result;
  result.append(voronoi);
  result.append(distance);
  
  return result;  
} /*py lfmm */



void fmm_export()
{
  def(
    "fmm",
    pylfmm,
    (arg("seeds"), arg("speeds"), arg("stop"), arg("threshold")),
    doc__fmm__c__
    );
  
} /* fmm_export */




// LuM end of file
