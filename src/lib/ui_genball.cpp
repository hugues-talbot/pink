/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"
#include "ujimage.hpp"

namespace {

  using pink::char_image;
  using boost::python::object;

  object 
  genball( double radius, index_t dim )
  {
    char_image result;
    if (dim==2) 
    {
      result = char_image( 2*radius+1, 2*radius+1 );
      FOR(w, result.rows())
        FOR(q, result.cols())
        if ( sqrt( (q-radius)*(q-radius) + (w-radius)*(w-radius)) <= radius )
          result(q, w) = 255;
    }
    else if (dim==3)
    {
      result = char_image( 2*radius+1, 2*radius+1, 2*radius+1 );
      FOR(e, result.depth())
        FOR(w, result.cols())
        FOR(q, result.rows())
        if ( sqrt( (q-radius)*(q-radius) + (w-radius)*(w-radius) + (e-radius)*(e-radius) )<=radius)
          result(q, w, e)=255;
    }
    else /* NOT dim == 3 */
      pink_error("only 2D and 3D supported in this moment.");           

    return result.steel();  
  } /* genball */
  
} // unnamed namespace

void 
genball_export() {

  using boost::python::arg;
  using boost::python::def;
  
  def( "genball", ::genball, ( arg("radius"), arg("dim") ), "Generates a [dim] dimensional binary ball, with [radius] radius." );
    
  import_array();  // numpy initialization
  return;    
}
  



  







/* LuM end of file */
