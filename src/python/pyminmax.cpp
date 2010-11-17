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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyminmax.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    boost::python::list
    minmax( const image_type & image )
    {
      typedef typename image_type::pixel_type pixel_type;
      boost::python::list result;
  
      pair<pixel_type, pixel_type> tmp = lminmaxval(image);

      result.append(tmp.first);
      result.append(tmp.second);
  
      return result;
  
    } /* minmax */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION( minmax, 
		    pink::python::minmax, 
		    (arg("image")),
                    "Returns the minimum and maximum values of the image"
  );

































// LuM end of file
