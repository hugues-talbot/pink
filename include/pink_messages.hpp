/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) HT 2018
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  hugues.talbot@univ-paris-est.fr
*/


#ifdef __cplusplus
#  ifndef PINK_MESSAGES_H_
#  define PINK_MESSAGES_H_

// Boost is explicitly forbidden here

#include <iostream>

// does not throw !OB
#define pink_warning(message) \
    {                                                  \
        std::cerr << "Pink warning: " << message << std::endl;  \
    }

// This throws an exception
#define pink_error(message)                                             \
    {                                                                   \
      std::cerr << std::endl << "Pink error!" << std::endl << "in '" << __FILE__<< "' (line " << __LINE__ << "): " << message << std::endl << std::endl; \
      throw "Pink error!\n";                                            \
    }

#  endif // PINK_MESSAGES_H_
#endif // __cplusplus
