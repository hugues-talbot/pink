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
#ifndef UI_GENBALL_HPP_
#define UI_GENBALL_HPP_

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ui_genball.hpp: " << msg; call_error(fullmessage.str());}


namespace pink {


  char_image genball( int radius, int dim );
  


} /* namespace pink */
#endif /* UI_GENBALL_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */


