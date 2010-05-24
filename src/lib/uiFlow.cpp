/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2007-2010
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
#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiFlow.cpp: " << msg; call_error(fullmessage.str());}

namespace pink { 




  PTR<float_image> maxflow_cami(
    char_image & SS,       /* image of the source and of the sink (not the original image) */
    float_image & gg,     /* Boundaries */
    int iteration,        /* number of iterations */
    float glob_tau,	  /* timestep */
    int number_of_threads /* the number of threads to execute if in parallel mode */
    ){
    
    PTR<maxflow<float_image> > maxflow_obj;
    
    maxflow_obj.reset( new maxflow<float_image>(SS, gg, iteration, glob_tau, number_of_threads ) );
    
    PTR<float_image> result_image = maxflow_obj->start();
    
    
    // creating the n+1D flow_image
    
    vint dim(3,0);
    dim[0]=SS.get_size()[0];
    dim[1]=SS.get_size()[1];
    dim[2]=SS.get_size().size();
    
    PTR<float_image> result_flow(new float_image(dim, maxflow_obj->get_flow(), "result_flow"));
    
//    ARRAY<float> flow = maxflow_obj->get_flow();
    
    return result_flow;
    
  }/* maxflow_cami */
  
  
  
  PTR<float_image> maxflow_float( 
    char_image & SS,         /* image of the source and of the sink (not the original image) */
    float_image & gg,       /* Boundaries */
    int iteration,          /* number of iterations */
    float glob_tau,	    /* timestep */
    int number_of_threads=0 /* the number of threads to execute if in parallel mode */
    )
  {
    
//    cout << "DEBUG VERSION 16\n";
    
    PTR<maxflow<float_image> > maxflow_obj;
    
    //cout << "maxflow object\n";
    
    maxflow_obj.reset( new maxflow<float_image>(SS, gg, iteration, glob_tau, number_of_threads ) );
    
    //cout << "start()ing\n";
    
    PTR<float_image> result = maxflow_obj->start();
    
    //cout << "returning result\n";
    
    return result;
    
  } /* maxflow_float */
  
  
  











  } /* namespace pink */


/* LuM end of file */
