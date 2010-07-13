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
#ifndef UIINSERT_HPP_
#define UIINSERT_HPP_

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiInsert.hpp: " << msg; call_error(fullmessage.str());}


namespace pink {


  template <class image_type >
  PTR<image_type> insert_image( 
    const image_type & big_image,
    const image_type & small_image,
    const boost::python::list & shift
    )
  {
    PTR<image_type> result( new image_type(big_image) );
    int d = big_image.get_size().size();
    vint shift_vec(shift);

    vint curr(d);
    vint new_pos(d);

    DEBUG(shift_vec.repr());
    DEBUG(big_image.get_size().repr());
    DEBUG(result->get_size().repr());
    DEBUG(small_image.get_size().repr());
    

    FOR(q, small_image.get_size().prod())
    {
      small_image.get_size().nextStep( q, curr );
      
      FOR(w, d)
      {
	new_pos[w]=curr[w]+shift_vec[w];
      } /* FOR */
      
      if (big_image.get_size().inside(new_pos))
      {
	(*result)[new_pos]=small_image[curr];
      } /* if */

    } /* FOR */
   

    return result;
  } /* insert_image */

  template <class image_type >
  PTR<image_type> merge_max_image( 
    const image_type & big_image,
    const image_type & small_image,
    const boost::python::list & shift
    )
  {
    PTR<image_type> result( new image_type(big_image) );
    int d = big_image.get_size().size();
    vint shift_vec(shift);

    vint curr(d);
    vint new_pos(d);

    DEBUG(shift_vec.repr());
    DEBUG(big_image.get_size().repr());
    DEBUG(result->get_size().repr());
    DEBUG(small_image.get_size().repr());
    

    FOR(q, small_image.get_size().prod())
    {
      small_image.get_size().nextStep( q, curr );
      
      FOR(w, d)
      {
	new_pos[w]=curr[w]+shift_vec[w];
      } /* FOR */
      
      if (big_image.get_size().inside(new_pos))
      {
	(*result)[new_pos]=
	  (*result)[new_pos]>small_image[curr]?
	  (*result)[new_pos]:small_image[curr];
      } /* if */

    } /* FOR */
   

    return result;
  } /* merge_max_image */






} /* namespace pink */
#endif /* UIINSERT_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */

