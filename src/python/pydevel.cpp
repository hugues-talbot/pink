/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#ifndef PINK_DEVELOPMENT
#  include "pink_python.h"
#else /* PINK_DEVELOPMENT */
#  include "pink_development.h"
#endif /* PINK_DEVELOPMENT */

using boost::python::arg;
using boost::python::def;

#ifdef PINK_DEVELOPMENT
# include "uiCircle.hpp"
# include "uiGradient.hpp"
# include "ui_measure.hpp"
# include "ui_polythread.hpp"
# include "ui_fit_circle.hpp"
#endif /* PINK_DEVELOPMENT */

void pydevel()
{
# ifdef PINK_DEVELOPMENT



# endif /* PINK_DEVELOPMENT */

  
}


// LuM end of file
