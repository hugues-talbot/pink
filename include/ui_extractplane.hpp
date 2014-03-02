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
#ifndef UI_EXTRACTPLANE_HPP_
#define UI_EXTRACTPLANE_HPP_

#include "ui_pink_types.hpp"

namespace pink {

  template<class image_type>
  image_type
  extractplane(
    const image_type & image,
    const index_t & n, /*the number of the plane*/
    const std::string & mode
    )
  {
    index_t i, j, k, t, offset, rs, cs, ds;

    if (! (((mode[0] == 'x') && (mode[1] == 'y')) ||
           ((mode[0] == 'y') && (mode[1] == 'x')) ||
           ((mode[0] == 'x') && (mode[1] == 'z')) ||
           ((mode[0] == 'z') && (mode[1] == 'x')) ||
           ((mode[0] == 'y') && (mode[1] == 'z')) ||
           ((mode[0] == 'z') && (mode[1] == 'y'))))
    {
        // fprintf(stderr, "usage: filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)");
      pink_error("extractplane called with incorrect mode " << mode );
    }

    rs = image.rows(); //rowsize(image);
    cs = image.cols(); //colsize(image);
    ds = image.depth(); //depth(image);

    /* ---------------------------------------------------------- */
    /* extraction d'un plan */
    /* ---------------------------------------------------------- */

    if ((mode[0] == 'x') && (mode[1] == 'y'))
    {
      image_type result(rs,cs);
      if ((n < 0) || (n >= ds)) pink_error("bad plane number " << n);

      t = rs * cs;
      offset = n * t;
      for (i = 0; i < t; i++) result(i) = image(i + offset);

      return result;      
    } 
    else if ((mode[0] == 'y') && (mode[1] == 'x'))
    {
      image_type result(cs, rs);
      if ((n < 0) || (n >= ds)) pink_error("bad plane number " << n);
      t = rs * cs;
      for (j = 0; j < cs ; j++)
        for (i = 0; i < rs ; i++)
          result(i*cs + j) = image(n*t + j*rs + i);

      return result;      
    }
    else if ((mode[0] == 'x') && (mode[1] == 'z'))
    {
      image_type result(rs, ds);

      if ((n < 0) || (n >= cs)) pink_error("bad plane number " << n);
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (i = 0; i < rs ; i++)
          result(k*rs + i) = image(k*t + n*rs + i);

      return result;
    }
    else if ((mode[0] == 'z') && (mode[1] == 'x'))
    {
      image_type result(ds, rs);
      
      if ((n < 0) || (n >= cs)) pink_error("bad plane number " << n ); //,n
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (i = 0; i < rs ; i++)
          result(i*ds + k) = image(k*t + n*rs + i);
      
      return result;      
    }
    else if ((mode[0] == 'y') && (mode[1] == 'z'))
    {  
      image_type result(cs, ds);
     
      if ((n < 0) || (n >= rs)) pink_error("bad plane number " << n);
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (j = 0; j < cs ; j++)
          result(k*cs + j) = image(k*t + j*rs + n);

      return result;      
    }
    else if ((mode[0] == 'z') && (mode[1] == 'y'))
    {
      image_type result( ds, cs);
      
      if ((n < 0) || (n >= rs))
      {
        pink_error("bad plane number " << n);
      }
      t = rs * cs;
      for (k = 0; k < ds ; k++)
        for (j = 0; j < cs ; j++)
          result(j*ds + k) = image(k*t + j*rs + n);

      return result;      
    }
    else
    {
      pink_error("Valid modes are (mode=xy|yx|xz|zx|yz|zy) " << mode);
    }
 
  } /* extractplane */
  

  




} /* namespace pink */
#endif /* UI_EXTRACTPLANE_HPP_ */
#endif /*__cplusplus*/
/* LuM end of file */
