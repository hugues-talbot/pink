/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UICUTPLANE_HPP_
#define UICUTPLANE_HPP_

#include <boost/python/list.hpp>

#include "ujimage.hpp"


namespace pink {

  pink::image<uint8_t>
  draw_plane( const pink::image<uint8_t> & original, float a, float b, float c, float d );

# ifdef PINK_HAVE_PYTHON
  char_image project_plane( const char_image & original,
                            const boost::python::list & A,
                            const boost::python::list & B,
//			     const boost::python::list & shift,
                            double alpha
    );  
# endif /* PINK_HAVE_PYTHON */



} /* namespace pink */
#endif /* UICUTPLANE_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
