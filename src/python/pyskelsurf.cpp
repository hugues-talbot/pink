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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyskelsurf.cpp: " << msg; call_error(fullmessage.str());}

// you should not use one-letter macro names!
#undef N
#undef D

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    template <class image_type>
    char_image skelsurf(
      const char_image & image,
      image_type priority_image,
      char_image inhibit,
      int connexity
      )
    {
      char_image result;
      result.copy(image);
     
      if (! lskelsurf3d(result.get_output(), priority_image.get_output(), /*can_be_null(inhibit)*/ NULL, connexity))
      {
        error("lskelsurf3d failed");        
      } /* if */


      return result;      
    } /* skelsurf */

    template <class image_type>
    char_image skelsurf_short(
      const char_image & image,
      image_type priority_image,
      int connexity
      )
    {     
      char_image result = skelsurf(image, priority_image, char_image(), connexity);
      return result;
    } /* skelsurf */


    
  } /* namespace python */
} /* namespace pink */

// const string skel_help =
//   "  brief surfacic binary skeleton guided by a priority image"
//   ""
//   "<B>Usage:</B> skelsurf in.pgm prio connex [inhibit] out.pgm"
//   ""
//   "<B>Description:</B>"
//   "Surfacic binary skeleton guided by a priority image."
//   "The lowest values of the priority image correspond to the highest priority."
//   ""
//   "The parameter b prio is either an image (byte or int32_t), or a numerical code"
//   "indicating that a distance map will be used as a priority image; "
//   "the possible choices are:"
//   "li 0: approximate euclidean distance"
//   "li 1: approximate quadratic euclidean distance"
//   "li 2: chamfer distance"
//   "li 3: exact quadratic euclidean distance"
//   "li 6: 6-distance in 3d"
//   "li 18: 18-distance in 3d"
//   "li 26: 26-distance in 3d"
//   ""
//   "The parameter b connex indicates the connectivity of the binary object."
//   "Possible choices are 6, 26."
//   ""
//   "If the parameter b inhibit is given and is a binary image name,"
//   "then the points of this image will be left unchanged. "
//   ""
//   "Let X be the set corresponding to the input image b in.pgm."
//   "Let P be the function corresponding to the priority image."
//   "Let I be the set corresponding to the inhibit image, if given, or the empty"
//   "set otherwise."
//   "The algorithm is the following:"
//   ""
//   "verbatim"
//   "C = {y in F | Tb(y) > 1}"
//   "Repeat until stability"
//   "  choose a point x in X, simple for X, such that C[x] == 0 "
//   "    and such that P[x] is minimal"
//   "  X = X - {x}"
//   "  for all y in gamma(x)"
//   "    if Tb(y) > 1 then C[y] = 1"
//   "Result: X"
//   "endverbatim"
//   ""
//   "where Tb(y) refers to the second connectivity number of y, that is, "
//   "(informally) the number of connected components of gamma(y) inter complement(X)."
//   ""
//   "References:<BR> "
//   "[BC07] G. Bertrand and M. Couprie: http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf Transformations topologiques discretes"
//   ""
//   "<B>Types supported:</B> byte 3d"
//   ""
//   "<B>Category:</B> topobin"
//   "ingroup  topobin"
//   ""
//   "author Michel Couprie"
                                                                                      
//                                                                                       ;


                                                                                      
void skelsurf_export()
{

UI_DEFINE_FUNCTION(
  skelsurf,
  pink::python::skelsurf,
  ( arg("image"), arg("priority_image"), arg("inhibit"), arg("connexity")),
  "WRITE ME!! skelhelp"
  // end of the documentation
   );

UI_DEFINE_FUNCTION(
  skelsurf,
  pink::python::skelsurf_short,
  ( arg("image"), arg("priority_image"), arg("connexity")),
    "WRITE ME!! skelhelp"
  // end of the documentation
   );


} /* skelsurf_export */










// LuM end of file
