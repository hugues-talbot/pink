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

#include "lminima.h"
#include "python_doc.h"

namespace pink {
  namespace python {


    template<class image_type>
    image_type
    minima(
      const image_type & image,
      const std::string & mode
      )
    {
      image_type result = image.clone();
      
      lminima( result, mode.c_str());

      return result;      
    } /* minima */

   
    

  } /* namespace python */
} /* namespace pink */

using boost::python::arg;
using boost::python::def;
using pink::python::minima;

/** Note: this is a special function, as we use implicit std::copy-conversion
    
 */
void minima_export()
{
  def("minima", minima<pink::char_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", minima<pink::short_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", minima<pink::int_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", minima<pink::float_image>,( arg("image"), arg("mode")), doc__minima__c__);
  def("minima", minima<pink::double_image>,( arg("image"), arg("mode")), doc__minima__c__);  
} /* minima_export */


















































// LuM end of file
