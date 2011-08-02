/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2007-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/** \file uiFlow.cpp
 * \brief The heart function of the software.
 * 
 * It is the implementation of Hugues Talbot's maximal flow algorithm. Everything else is just a foreplay.
 * \author UjoImro, 2007.
 */
// not my includes
/* Line 12+1 */



// my includes
#include "uiFlow.hpp"
#include "pink.h"

namespace pink { 

  float_image maxflow_cami(
    char_image  SS,       /* image of the source and of the sink (not the original image) */
    float_image gg,     /* Boundaries */
    index_t     iteration,        /* number of iterations */
    float       glob_tau,	  /* timestep */
    index_t     number_of_threads /* the number of threads to execute if in parallel mode */
    )
  {

    pink_error("not compiled, uncomment the code.");
    
//     boost::shared_ptr<maxflow<float_image> > maxflow_obj;
    
//     maxflow_obj.reset( new maxflow<float_image>(SS, gg, iteration, glob_tau, number_of_threads ) );
    
//     float_image result_image = maxflow_obj->start();
    
    
//     // creating the n+1D flow_image
    
//     vint dim(3,0);
//     dim[0]=SS.get_size()[0];
//     dim[1]=SS.get_size()[1];
//     dim[2]=SS.get_size().size();
    
//     float_image result_flow(maxflow_obj->get_flow(), "result_flow");
    
// //    ARRAY<float> flow = maxflow_obj->get_flow();
    
//     return result_flow;
    
  }/* maxflow_cami */
  
  
  
  float_image maxflow_float( 
    char_image  SS,         /* image of the source and of the sink (not the original image) */
    float_image gg,        /* Boundaries */
    index_t     iteration,         /* number of iterations */
    float       glob_tau,	   /* timestep */
    index_t     number_of_threads,  /* the number of threads to execute if in parallel mode */
    index_t     packet_size
    )
  {
    
//    cout << "DEBUG VERSION 16\n";
    
    boost::shared_ptr< maxflow<float_image> > maxflow_obj;
    
    //cout << "maxflow object\n";
    
    maxflow_obj.reset( new maxflow<float_image>(
                         frame_around(SS, -1),
                         frame_around(gg, 0.),
                         iteration,
                         glob_tau,
                         number_of_threads,
                         packet_size
                         )
      );
    
    //cout << "start()ing\n";
    
    float_image result = frame_remove(maxflow_obj->start());
    
    //cout << "returning result\n";
    
    return result;
    
  } /* maxflow_float */
  
  
  











} /* namespace pink */


/* LuM end of file */
