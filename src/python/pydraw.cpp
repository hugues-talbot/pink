// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pydraw.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {

    char_image drawcurve2D(
      const vint & size,
      const vint & coordX,
      const vint & coordY
      )
    {
      if (size.size()!=2)
      {
        error("only 2D images are supported. Nag the developer.");
      } /* size.size()!=2 */

      if (coordX.size() != coordY.size())
      {
        error("error: The number of X and Y coordinates must be equal.");
      } /* coordX.size() != coordy.size() */
  

      char_image result(size);

      FOR( q, coordX.size()-1 )
      {
        ldrawline( result.get_output(),
                   coordX[q],
                   coordY[q],
                   coordX[q+1],
                   coordY[q+1]
          ); 
      } /* FOR coordX.size()-1 */

      return result;
    } /* drawcurve2D */

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

UI_EXPORT_ONE_FUNCTION(
  drawcurve2D,
  pink::python::drawcurve2D,
  ( arg("image size"), arg("X coordinates"), arg("Y coordinates")),
  "the help is in 'help(pink.drawcurve)'"
  );










































// LuM end of file
