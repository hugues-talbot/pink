/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_CONVERT_IF_HPP
#define UI_CONVERT_IF_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

#include "pyujimage.hpp"

namespace pink
{
  namespace python
  {
    

    template < class T >
    struct convert_if
    {
      typedef typename boost::mpl::if_<
        boost::is_base_of<pink::pink_image, typename boost::remove_reference<T>::type >,
        xvimage*,
        T
        >::type type;
    }; /* convert_if */

    
    template < class T >
    struct xv2pink
    {
      typedef typename boost::mpl::if_<
        boost::is_same<xvimage*, T>,
        pink::pink_image&,
        T
        >::type type;      
    }; /* xv2pink */
    

    template <class RT, class T, RT (*mcfunction)(T)>
    RT ui_cheat(T t)
    {
      return mcfunction(t);
    }; /* ui_cheat */
   

  } /* namespace python */
} /* namespace pink */






















#endif /* UI_CONVERT_IF_HPP */
// LuM end of file
