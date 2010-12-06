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
      char_image tmp;      
      if (connex==6)
      {
        tmp = inverse(image);        
      }
      else /* NOT connex== 6 */
      {
        if (connex==26)
        {
          tmp = image;          
        }
        else /* NOT connex== 26 */
        {
          error("bad connexity");
        } /* NOT connex== 26 */
      } /* NOT connex== 6 */

      InitLUT();
      InitMesh(1000); /* automatically reallocated if needed */

      stringstream result;
            
      if (seuil == 255)
      { // with fix-point preservation
        if (! lmarchingcubes2(tmp.get_output(), nregul, obj_id, reinterpret_cast<FILE*>(&result), T_VTK_PYTHON))
        {
          error("function lmarchingcubes2 failed");
        }
      }
      else
      {
        if (! lmarchingcubes(tmp.get_output(), seuil, nregul, obj_id, reinterpret_cast<FILE*>(&result), T_VTK_PYTHON))
        {
          error("function lmarchingcubes failed");
        }
      }
      
      TermineMesh();

      return result.str();      
    } /* mcube */
    
    

  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  mcube,
  pink::python::mcube,
  (arg("image"), arg("seuil"), arg("nregul"), arg("obj_id"), arg("connex")=26),
  "brief topologically correct 'marching cubes'-like algorithm\n"
  "\n"
  "<B>Usage:</B> mcube in.pgm threshold nregul obj_id format [connex] out\n"
  "\n"
  "<B>Description:</B>\n"
  "Generates a 3d mesh from the binary or grayscale image b in.pgm .\n"
  "\n"
  "The original image is first thresholded (parameter b threshold, values 0 and 1\n"
  "both fit for a binary image). Then the method described in [Lac96] is applied to\n"
  "generate the 3d mesh. The resulting mesh is then smoothed by b nregul steps of \n"
  "laplacian smoothing. The parameter b obj_id is used to tag the generated mesh.\n"
  "\n"
  "The parameter b format indicate the format of the output file \n"
  "(choices are POV, POVB, COL, MCM, AC, GL, VTK, RAW).\n"
  "The keyword POVB corresponds to a bare Povray mesh: \n"
  "a header and a footer must be catenated in order to make a full Povray scene. \n"
  "The keyword RAW is the exchange format for the 'mesh' software: see http://mesh.berlios.de/\n"
  "\n"
  "The optional parameter b connex indicates the connexity used for the object.\n"
  "Possible values are 6 and 26 (default).\n"
  "\n"
  "[Lac96] J.-O. Lachaud, 'Topologically defined iso-surfaces', DGCI'96, LNCS 1176 (245--256), Springer Verlag, 1996.\n"
  "\n"
  "<B>Types supported:</B> byte 3d\n"
  "\n"
  "<B>Category:</B> mesh3d\n"
  "ingroup  mesh3d\n"
  "\n"
  "author Michel Couprie\n"
// end of documentation
  );

















































// LuM end of file
