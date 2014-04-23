/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <boost/python.hpp>

#include "lsym.h"
#include "ujimage.hpp"
#include "python_doc.h"
#include "ldilateros.h"
#include "ldilateros3d.h"


namespace pink {
  namespace python {
    using boost::python::object;
        
    object
    closing ( const object & image,  const object & elem, const index_t & x, const index_t & y )
    {
      if ((2 != cxvimage(image).size().size()) or (2 != cxvimage(elem).size().size()))
        pink_error("The dimensions of the structuring element, the image and the center must be the same (d=2).");
      
      const cxvimage cimage(image);
      cxvimage result = cimage.clone();
      cxvimage celem  = cxvimage(elem).clone();
      
      index_t rs = celem.rows();
      index_t cs = celem.cols();

      if (! ldilateros_ldilat(result, celem, rs - 1 - x, cs - 1 - y)) pink_error("function ldilat failed");
      if (! lsym(celem, 'c')) pink_error("function lsym failed");      
      if (!ldilateros_leros(result, celem, x, y)) pink_error("function leros failed");

      return result.steel();
      
    } // closing


    object
    closing3d ( const object & image,  const object & elem, const index_t & x, const index_t & y, const index_t & z )
    {
      if ((3 != cxvimage(image).size().size()) or (3 != cxvimage(elem).size().size()))
        pink_error("The dimensions of the structuring element, the image and the center must be the same (d=3).");

      cxvimage celem = cxvimage(elem).clone();
      cxvimage result = cxvimage(image).clone();
      
      index_t rs = celem.rows();
      index_t cs = celem.cols();
      index_t ds = celem.depth();
      
      if (! ldilat3d(result, celem, x, y, z)) pink_error("function leros3d failed");
      if (! lsym(celem, 'c')) pink_error("function lsym failed");
      if (! leros3d(result, celem, rs - 1 - x, cs - 1 - y, ds - 1 - z)) pink_error("function leros3d failed");
  
      return result.steel();
    } /* closing3d */

  } /* namespace python */
} /* namespace pink */


void
closing_export() {
  using boost::python::arg;
  using boost::python::def;
  
  def( "closing", &pink::python::closing, ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y") ), doc__closing__c__ );
  def( "closing", &pink::python::closing3d, ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z") ), doc__closing__c__ );

  return;  
} // closing export




// LuM end of file
