/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymcube.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    string mcube(
      char_image image,
      int seuil,
      int nregul,
      int obj_id,
      int connex=26
      )
    {
      if (connex==6)
      {
        
      }
      else /* NOT connex== 6 */
      {
        if (connex==26)
        {
          
        }
        else /* NOT connex== 26 */
        {
          error("bad connexity");
        } /* NOT connex== 26 */
      } /* NOT connex== 6 */
      
      
    } /* mcube */
    


  } /* namespace python */
} /* namespace pink */


















































// LuM end of file
