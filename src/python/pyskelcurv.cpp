/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "ujimage.hpp"
#include "pyexport.hpp"
#include "lskeletons.h"
#include "python_doc.h"

// you should not define N as a macro
#undef N


namespace pink {
  namespace python {

    using pink::pick;    
    using boost::python::object;
    using boost::python::extract;    

    object
    skelcurv(
      object image,
      object cprio,
      const index_t & connex,
      object inhibit ) {

      cxvimage prioimage;
      cxvimage inhibimage;
      int32_t  priovalue = -1;
      int32_t  priocode;      
      
      if (PyArray_Check(cprio.ptr())) { // prio is an image
        prioimage = cxvimage(cprio);
      }      
      else
        priovalue = extract<int32_t>(cprio);

      if (PyArray_Check(inhibit.ptr()))
        inhibimage = cxvimage(inhibit);

      cxvimage result = cxvimage(image).clone();
      
      if ((prioimage.isnull()) && (priovalue != 5))
      {
        int32_t i, N;
        uint8_t *F;
        prioimage = cxvimage( VFF_TYP_4_BYTE, result.size() );

        N = result.rows() * result.cols() * result.depth();        
        F = result.pdata<uint8_t>();
        for (i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

        bool is2d = (result.depth() == 1);
        
        if (priovalue == 0)
          pick("ldisteuc(3d)", is2d, ldisteuc, ldisteuc3d, result, prioimage );
        else if (priovalue == 1)
          pick("ldistquad(3d)", is2d, ldistquad, ldistquad3d, result, prioimage );        
        else if (priovalue == 2)
        { if (! lchamfrein(result, prioimage)) pink_error("lchamfrein failed"); }
        else if (priovalue == 3)
        { if (! lsedt_meijster(result, prioimage)) pink_error("lsedt_meijster failed"); }
        else
        { if (! ldist(result, priovalue, prioimage)) pink_error("ldist failed"); }

        for (i = 0; i < N; i++) // re-inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
      }
      
      if (result.depth() == 1)
      {
        if (! lskelcurv(result, prioimage, inhibimage, connex))
          pink_error("lskelcurv failed");        
      }
      else // NOT result.depth == 1
      {
        if (priovalue == 5)
        {
          if (! lskeldir3d_1(result, inhibimage, connex, -1))
            pink_error("lskeldir3d failed");          
        }
        else // NOT priovalue == 5
        {
          if (! lskelcurv3d(result, prioimage, inhibimage, connex))
            pink_error("lskelcurv3d failed");          
        } // NOT priovalue == 5
      } // NOT result.depth == 1

      return result.steel();    
    } // skelcurv

    
  } /* namespace python */
} /* namespace pink */


void
skelcurv_export()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;  

  def( "skelcurv", pink::python::skelcurv, (arg("image"), arg("prio"), arg("connex"), arg("inhibit") = object(-1) ), doc__skelcurv__c__ );

  import_array();  // numpy initialization

  return;  
} /* skelcurv_export */

















































// LuM end of file
