/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include "lsym.h"
#include "lgeodesic.h"
#include "python_doc.h"
#include "ldilateros.h"
#include "ldilateros3d.h"


namespace pink {
  namespace python {
    using boost::python::object;    
    
    object
    opening( const object & image, const object & elem, const index_t & x, const index_t & y )
    {
      if ((2 != cxvimage(image).size().size()) or (2 != cxvimage(elem).size().size()))
        pink_error("The dimensions of the structuring element, the image and the center must be the same (2).");
      
      cxvimage result = cxvimage(image).clone();
      cxvimage celem  = cxvimage(elem).clone();

      uint8_t * data  = result.pdata<uint8_t>();
            
      index_t rs = celem.rows();
      index_t cs = celem.cols();        

      if (! ldilateros_leros( result, celem, x, y)) pink_error("function leros failed");
      if (! lsym( celem, 'c')) pink_error("function lsym failed");
      if (! ldilateros_ldilat( result, celem, rs - 1 - x, cs - 1 - y)) pink_error("function ldilat failed");

      return result.steel();      
    } // opening

    object
    opening3d( const object & image, const object & celem, const index_t & x, const index_t & y, const index_t & z )
    {
      if ((3 != cxvimage(image).size().size()) or (3 != cxvimage(celem).size().size()))
        pink_error("The dimensions of the structuring element, the image and the center must be the same (3).");
      
      cxvimage result = cxvimage(image).clone();
      cxvimage elem  = cxvimage(celem).clone();

      index_t rs = elem.rows();      
      index_t cs = elem.cols();      
      index_t ds = elem.depth();      

      if (! leros3d( result, elem, x, y, z)) pink_error("function leros3d failed");
      if (! lsym(elem, 'c')) pink_error("function lsym failed");      
      if (! ldilat3d( result, elem, rs - 1 - x, cs - 1 - y, ds - 1 - z)) pink_error("function leros3d failed");
        
      return result.steel();
    } /* opening */


  } /* namespace python */
} /* namespace pink */


void
opening_export() {
  using boost::python::arg;  
  using boost::python::def;

  def( "opening", pink::python::opening,   ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y") ), doc__opening__c__  );
  def( "opening", pink::python::opening3d, ( arg("image"), arg("structuring element"), arg("center_x"), arg("center_y"), arg("center_z") ), doc__opening__c__  );

  return;  
} // opening export









// LuM end of file
