/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink_python.h>

#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in pygeoeros.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image geoeros(
      const char_image & F, 
      const char_image & G, 
      int connex, 
      int numb_iter=-1
      )
    {

      if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
      {
        error("error: bad connexity - use one of the following:\n"
              "4, 8 (in 2d), 6, 18, 26 (in 3d)\n"
          );
      } /* ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26)) */


      char_image result;
      result.copy(F);
      char_image G_const_away;
      G_const_away.copy(G);


      if (result.get_size().size()!=result.get_size().size()){
        error("error: the dimensions of F and G must be equal");
      } 


      if (result.get_size().size()==2)
      {
        lgeoeros( result.get_output(), G_const_away.get_output(), connex, numb_iter ); 
      } 
      else 
      { /* NOT result.get_size().size()==2 */
        if (result.get_size().size()==3)
        {      
          lgeoeros3d( result.get_output(), G_const_away.get_output(), connex, numb_iter );
        } 
        else 
        { /* NOT result.get_size().size()==3 */
          error("error: only 2D and 3D images are supported");
        }; /* NOT result.get_size().size()==3 */
      }; /* NOT result.get_size().size()==2 */

   
      return result;    
    } /* py_geoeros */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION (
  geoeros,
  pink::python::geoeros,
  ( arg("G image"), arg("F image"), arg("connexity"), arg("number of iterations")=-1 ),
  doc__geoeros__c__
  // end of the documenation
  );

  




















































// LuM end of file
