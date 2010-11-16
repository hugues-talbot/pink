/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pygeodilat.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



char_image py_geodilat(
  const char_image & F, 
  const char_image & G, 
  int connex, 
  int numb_iter
  )
{
  
  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    error("error: bad connexity - use one of the following:\n"
	  "4, 8 (in 2d), 6, 18, 26 (in 3d)\n"
      );
  } /* ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26)) */


  char_image result;
  result = F;
  char_image G_const_away;
  G_const_away = G;

  if (result.get_size().size()!=result.get_size().size()){
    error("error: the dimensions of F and G must be equal");
  }


  if (result.get_size().size()==2)
  {
    lgeodilat( result.get_output(), G_const_away.get_output(), connex, numb_iter ); 
  } 
  else /* NOT result.get_size().size()==2 */
  { 
    if (result.get_size().size()==3)
    {      
      lgeodilat3d( result.get_output(), G_const_away.get_output(), connex, numb_iter );
    } 
    else 
    { /* NOT result.get_size().size()==3 */
      error("error: only 2D and 3D images are supported");
    }; /* NOT result.get_size().size()==3 */
  }; /* NOT result.get_size().size()==2 */
  
  return result;    
};



UI_EXPORT_ONE_FUNCTION(
  cpp_geodilat,
  py_geodilat,
  ( arg("G image"), arg("F image"), arg("connexity"), arg("number of iterations")),
  "geodesic (grayscale or binary) dilation \n"
  " \n"
  "Usage: geodilat g.pgm f.pgm connex niter out.pgm \n"
  " \n"
  "Description: \n"
  "Geodesic erosion of g.pgm over f.pgm \n"
  "Let G and F be the two input images. If G is not over F, then  \n"
  "G is replaced initially by min(G,F). \n"
  "The structuring element is specified by the value of the parameter connex, \n"
  "which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d. \n"
  "The parameter \b niter sets the number of iterations. If \b niter = -1, \n"
  "then the iterations continue until stability. \n"
  " \n"
  "Types supported: byte 2d, byte 3d. \n"
  " \n"
  "Category: connect, morpho \n"
  "group  connect, morpho \n"
  " \n"
  "author Michel Couprie - juillet 1996 \n"
  " \n"
// end of the documenation
    );
  



















































// LuM end of file
