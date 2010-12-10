/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pytoposhrink.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;




namespace pink {
  namespace python {

    char_image toposhrink(
      const char_image & image,
      int_image priority_image,
      int connexity,
      int tmin,
      int tmax,
      int tbmin,
      int tbmax,
      char_image inhibit
      )
    {
      char_image result;
      result.copy(image);

      char_image loc_inhibit;
      
      if (inhibit!=0)
      {
        loc_inhibit = inhibit;        
      }
      
      
      if (image.get_size().size()==2)
      {
        if (! ltoposhrink(result, priority_image, connexity, tmin, tmax, tbmin, tbmax, can_be_null(inhibit)))
        {
          error("ltoposhrink failed");
        }
      }
      else /* NOT image.get_size().size()==2 */
      {
        if (! ltoposhrink3d(result, priority_image, connexity, tmin, tmax, tbmin, tbmax, can_be_null(inhibit)))
        {
          error("ltoposhrink3d failed");
        }
      }  /* NOT image.get_size().size()==2 */

      return result;      
    } /* toposhrink */
    

    char_image toposhrink_small(
      const char_image & image,
      int_image priority_image,
      int connexity,
      int tmin,
      int tmax,
      int tbmin,
      int tbmax
      )
    {
      char_image result = toposhrink( image, priority_image, connexity, tmin, tmax, tbmin, tbmax, char_image() );
      return result;      
    } /* toposhrink_small */
    

    
  } /* namespace python */
} /* namespace pink */


void toposhrink_export()
{

  UI_DEFINE_ONE_FUNCTION(
    toposhrink,
    pink::python::toposhrink,
    ( arg("image"),
      arg("priority_image"),
      arg("connexity"),
      arg("tmin"),
      arg("tmax"),
      arg("tbmin"),
      arg("tbmax"),
      arg("inhibit")
      ),
    "  brief topologically controled binary shrinking guided by a priority image\n"
    "\n"
    "<B>Usage:</B> toposhrink in.pgm prio.pgm connex tmin tmax tbmin tbmax [inhibit] out.pgm\n"
    "\n"
    "<B>Description:</B>\n"
    "Topologically controled binary shrinking guided by a priority image.\n"
    "The lowest values of the priority image correspond to the highest priority.\n"
    "\n"
    "The parameter b prio is an image (byte or int32_t).\n"
    "\n"
    "The parameter b connex indicates the connectivity of the binary object.\n"
    "Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.\n"
    "\n"
    "If the parameter b inhibit is given and is a binary image name,\n"
    "then the points of this image (set Y) will be left unchanged. \n"
    "\n"
    "Let X be the set of points of the binary image b in.pgm .\n"
    "\n"
    "verbatim\n"
    "Repeat until stability: \n"
    "    select a point p of X  Y such that\n"
    "        tmin <= T(p) <= tmax and tbmin <= Tb(p) <= tbmax \n"
    "        and with the lowest possible priority value\n"
    "    X := X - { p }\n"
    "endverbatim\n"
    "\n"
    "<B>Types supported:</B> byte 2d, byte 3d\n"
    "\n"
    "<B>Category:</B> topobin\n"
    "ingroup  topobin\n"
    "\n"
    "author Michel Couprie\n"
    //end of documentation
    );

  UI_DEFINE_ONE_FUNCTION(
    toposhrink,
    pink::python::toposhrink_small,
    ( arg("image"),
      arg("priority_image"),
      arg("connexity"),
      arg("tmin"),
      arg("tmax"),
      arg("tbmin"),
      arg("tbmax")
      ),
    "WRITE ME!!"
    // end of documentation
    );
  

}







// LuM end of file
