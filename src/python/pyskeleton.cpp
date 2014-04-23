/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "ldist.h"
#include "lskeletons.h"
#include "lsquelbin.h"
#include "python_doc.h"

// based on skeleton.c

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {

    using pink::pick;    
    using boost::python::object;
    using boost::python::extract;
        
    object
    skeleton(
      const object  image,
      const object  cprio,
      const int32_t connex,
      const object  inhibit ) {

      cxvimage prioimage;      
      int32_t  priocode = -1;
      cxvimage inhibimage;
      int32_t  inhibvalue = -1;

      cxvimage result = cxvimage(image).clone();

      if (PyArray_Check(cprio.ptr())) // prio is an image
        prioimage = cxvimage(cprio);            
      else
        priocode = extract<index_t>(cprio);

      if (PyArray_Check(inhibit.ptr()))
        inhibimage = cxvimage(inhibit);
      else
        inhibvalue = extract<index_t>(inhibit);

      // if the priority is some kind of distance
      if (prioimage.isnull()) {
        int32_t i, N;
        uint8_t *F;
        prioimage = cxvimage( VFF_TYP_4_BYTE, result.size() );
        
        N = result.rows() * result.cols() * result.depth();
        F = result.pdata<uint8_t>();        
        for (i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

        bool is2d = (result.depth()==1);        
        
        if (priocode == 0)
          pick( "ldisteuc(3d)", is2d, ldisteuc, ldisteuc3d, result, prioimage );
        else if (priocode == 1)
          pick( "ldistquad(3d)", is2d, ldistquad, ldistquad3d, result, prioimage );        
        else if (priocode == 2) {
          if (!lchamfrein(result, prioimage)) pink_error("lchamfrein failed");
        }
        else if (priocode == 3)
        {
          if (! lsedt_meijster(result, prioimage)) pink_error("lsedt_meijster failed");          
        }
        else {
          if (! ldist(result, priocode, prioimage)) pink_error("ldist failed");          
        }

        for (i = 0; i < N; i++) // re-inverse the image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
      }

      if (result.depth() == 1)
      {
        if (inhibimage.isnull() || (inhibvalue == -1)) {
          if (! lskelubp2(result, prioimage, connex, inhibimage)) pink_error("lskelubp2 failed");
        }
        else {
          if (! lskelubp(result, prioimage, connex, inhibvalue)) pink_error("lskelubp failed");
        }
      }
      else
      {
        if (inhibimage.isnull() || (inhibvalue == -1))
        {
          if (! lskelubp3d2(result, prioimage, connex, inhibimage)) pink_error("lskelubp3d2 failed");
        }
        else
        {
          if (! lskelubp3d(result, prioimage, connex, inhibvalue)) pink_error("lskelubp3d failed");
        }
      }

      return result.steel();      
    } // skeleton
    

  } /* namespace python */
} /* namespace pink */


/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************

exported functions

***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
*/

void
skeleton_export() 
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;
    
  def( "skeleton", pink::python::skeleton, (arg("image"), arg("prio"), arg("connex"), arg("inhibit") = object(-1) ), doc__skeleton__c__ );

  import_array();  // numpy initialization
  return;  
} /* py_skeleton_export */









































// LuM end of file
