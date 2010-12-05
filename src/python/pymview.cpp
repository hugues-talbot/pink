/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) Hugues Talbot, 2009-2010 Université Paris-Est, Laboratoire
  d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy
  le Grand CEDEX

  hugues.talbot@gmail.com

*/

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymview.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
    namespace python {

        template <class image_type> void imview (const image_type & image)
        {
            std::cerr << "This will show an image eventually" << std::endl;
        }

    } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
    imview,
    pink::python::imview,
    (arg("src")
        ),
    "Displays an image"
    );
