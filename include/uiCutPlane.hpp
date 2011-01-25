/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UICUTPLANE_HPP_
#define UICUTPLANE_HPP_

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiCutPlane.hpp: " << msg; call_error(fullmessage.str());}


namespace pink {

  char_image draw_plane( const char_image & original, float a, float b, float c, float d );


  char_image project_plane( const char_image & original,
                            const boost::python::list & A,
                            const boost::python::list & B,
//			     const boost::python::list & shift,
                            double alpha
    );  



} /* namespace pink */
#endif /* UICUTPLANE_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
