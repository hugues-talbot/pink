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


namespace pink {
  namespace python {

    char_image geodilat(
      const char_image & G, 
      const char_image & F, 
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
      result.copy(G);

      char_image fc;
      fc.copy(F);
      
      if (result.get_size().size()!=fc.get_size().size()){
        error("error: the dimensions of F and G must be equal");
      }


      if (result.get_size().size()==2)
      {
        lgeodilat( result.get_output(), fc.get_output(), connex, numb_iter ); 
      } 
      else /* NOT result.get_size().size()==2 */
      { 
        if (result.get_size().size()==3)
        {      
          lgeodilat3d( result.get_output(), fc.get_output(), connex, numb_iter );
        } 
        else 
        { /* NOT result.get_size().size()==3 */
          error("error: only 2D and 3D images are supported");
        } /* NOT result.get_size().size()==3 */
      } /* NOT result.get_size().size()==2 */

      return result;    
    } /* geodilat */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION(
  geodilat,
  pink::python::geodilat,
  ( arg("G image"), arg("F image"), arg("connexity"), arg("number of iterations")=-1),
  doc__geodilat__c__
  // end of the documenation
  );
  



















































// LuM end of file
