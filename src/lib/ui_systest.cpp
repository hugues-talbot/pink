/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#define BOOST_DISABLE_ASSERTS
#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api

#include <numa.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "pink.h"
#include "ui_simd.hpp"

namespace pink
{
  namespace benchmark
  {

    const index_t test_size = 200 * 1024 * 1024 / 4;  // 200 MiB of data
    const index_t simd_size = 200 * 1024 * 1024 / 16; // 200 MiB of data
    const index_t repeat    = 10;
    

    
    double now()
    {
      struct timeval tp;
      double sec, usec, result;
// Time stamp before the computations
      gettimeofday( &tp, NULL );
      sec    = static_cast<double>( tp.tv_sec );
      usec   = static_cast<double>( tp.tv_usec )/1E6;
      result = sec + usec;
      return result;
    }


    double float_speed()
    {
      double float_speed = 0.;
      boost::scoped_array<float> from (new float[test_size]);
      boost::scoped_array<float> to   (new float[test_size]);

//      std::cout << "measuring float-copy speed" << std::endl;
      
      FOR(q, repeat)
      {
        srand(now());
        // filling up the memory with pseudo-random numbers
        for (index_t q=0; q<test_size; q++)
        {
          from[q]=rand();
        }
        // copying the data
        double start = now();
        std::copy( &(from[0]), &(from[test_size]), &(to[0]) );
        double end = now();
        float_speed += static_cast<double>(test_size * 4) / (1024. * 1024.) / (end - start) / static_cast<double>(repeat);
      } /* for q in repeat */
      
      // std::cout << "estimated speed of float copy = " << float_speed << " MiBps" << std::endl;
      return float_speed;
    } /* float_speed */
      


    double simd_speed()
    {
      double simd_speed = 0.;
      boost::scoped_array<simd::vec> from (new simd::vec[simd_size]);
      boost::scoped_array<simd::vec> to   (new simd::vec[simd_size]);

      //std::cout << "measuring simd-copy speed" << std::endl;
      
      FOR(q, repeat)
      {
        srand(now());
        // filling up the memory with pseudo-random numbers
        for (index_t q=0; q<simd_size; q++)
        {
          from[q].i[0]=rand();
          from[q].i[1]=rand();
          from[q].i[2]=rand();
          from[q].i[3]=rand();
        }
        // copying the data
        double start = now();
        std::copy( &(from[0].v), &(from[simd_size].v), &(to[0].v) );
        double end = now();
        simd_speed += static_cast<double>(simd_size * 16) / (1024. * 1024.) / (end - start) / static_cast<double>(repeat);
      } /* for q in repeat */
      
      //std::cout << "estimated speed of simd copy = " << simd_speed << " MiBps" << std::endl;
      return simd_speed;
    } /* simd_speed */

    
    
    void memspeed()
    {
      std::cout << "measuring float-copy speed" << std::endl;      
      std::cout << "estimated speed of float copy = " << simd_speed() << " MiBps" << std::endl;

      std::cout << "measuring simd-copy speed" << std::endl;      
      std::cout << "estimated speed of simd copy  = " << simd_speed() << " MiBps" << std::endl;
      return;
    } /* memspeed */    

    

#   ifdef PINK_HAVE_NUMA
    double numa_worker( index_t node )
    {
      // restricting myself to this node
      // for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node);
      numa_bind(&mask);

      double float_speed = 0.;
      float * from = reinterpret_cast<float*>( numa_alloc_onnode(test_size * sizeof(float), node ) );
      float * to   = reinterpret_cast<float*>( numa_alloc_onnode(test_size * sizeof(float), node ) );

      FOR(q, repeat)
      {
        srand(now());
        // filling up the memory with pseudo-random numbers
        for (index_t q=0; q<test_size; q++)
        {
          from[q]=rand();
        }
        // copying the data
        double start = now();
        std::copy( &(from[0]), &(from[test_size]), &(to[0]) );
        double end = now();
        float_speed += static_cast<double>(test_size * 4) / (1024. * 1024.) / (end - start) / static_cast<double>(repeat);
      } /* for q in repeat */

      numa_free( reinterpret_cast<void*>(from), test_size * sizeof(float));
      numa_free( reinterpret_cast<void*>(to  ), test_size * sizeof(float));
      
      return float_speed;
    } /* numa_worker () */



    template <BOOST_PP_ENUM_PARAMS(3, class T)>
    void numa_thread( T0 node, T1 barrier_start, T2 barrier_end )
    {
      // restricting myself to this node
      // for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node);
      numa_bind(&mask);

      double float_speed = 0.;
      float * from = reinterpret_cast<float*>( numa_alloc_onnode(test_size * sizeof(float), node ) );
      float * to   = reinterpret_cast<float*>( numa_alloc_onnode(test_size * sizeof(float), node ) );
      
      srand(now());
      // filling up the memory with pseudo-random numbers
      for (index_t q=0; q<test_size; q++)
      {
        from[q]=rand();
      }

      barrier_start->wait();
      FOR(q, repeat)
      {
        std::copy( &(to[0]), &(to[test_size]), &(from[0])  );
        std::copy( &(from[0]), &(from[test_size]), &(to[0]));
      }
      barrier_end->wait();

      numa_free( reinterpret_cast<void*>(from), test_size * sizeof(float));
      numa_free( reinterpret_cast<void*>(to  ), test_size * sizeof(float));

      return;
    } /* numa_worker () */



    void numaspeed()
    {
      index_t number_of_nodes  = numa_max_node() + 1;
      std::vector< boost::shared_ptr<boost::thread> > threads(number_of_nodes);

      std::cout << "sequential numa read-write test" << std::endl;
      
      FOR(q, number_of_nodes)
      {
        std::cout << "the speed of node " << q << " is " << numa_worker(q) << " MiBps" << std::endl;
      } /* q in number_of_nodes */

      std::cout << "parallel numa read-write test" << std::endl;
      typedef boost::shared_ptr<boost::barrier> pbarrier_t;
      pbarrier_t barrier_start( new boost::barrier(number_of_nodes + 1) );
      pbarrier_t barrier_end(   new boost::barrier(number_of_nodes + 1) );
       
      FOR( q, number_of_nodes )
      {
        threads[q].reset( new boost::thread( numa_thread<index_t, pbarrier_t, pbarrier_t>, q, barrier_start, barrier_end ) );
      }

      double start = now();
      barrier_start->wait();
      barrier_end->wait();
      double end = now();

      double numa_speed = static_cast<double>( repeat * number_of_nodes * 2 * test_size * 4) / (1024. * 1024.) / static_cast<double>(end - start);
            
      std::cout << "numa parallel speed = " << numa_speed << " MiBps" << std::endl;
            
      FOR( q, number_of_nodes )
      {
        threads[q]->join();
      }
      
      return;
    } /* numaspeed */    



    void numa_dancer( index_t node, index_t nbt )
    {
      index_t number_of_nodes  = numa_max_node() + 1;
      std::vector< boost::shared_ptr<boost::thread> > threads(nbt);

      std::cout << "parallel numa read-write test" << std::endl;
      typedef boost::shared_ptr<boost::barrier> pbarrier_t;
      pbarrier_t barrier_start( new boost::barrier(nbt + 1) );
      pbarrier_t barrier_end(   new boost::barrier(nbt + 1) );
       
      FOR( q, nbt )
      {
        threads[q].reset( new boost::thread( numa_thread<index_t, pbarrier_t, pbarrier_t>, q, barrier_start, barrier_end ) );
      }

      double start = now();
      barrier_start->wait();
      barrier_end->wait();
      double end = now();

      double numa_speed = static_cast<double>( repeat * nbt * 2 * test_size * 4) / (1024. * 1024.) / static_cast<double>(end - start);
            
      std::cout << "numa parallel speed = " << numa_speed << " MiBps" << std::endl;
            
      FOR( q, nbt )
      {
        threads[q]->join();
      }
      
      return;      
    } /* numa_dancer */
    

#   endif /* PINK_HAVE_NUMA */

    
  } /* namespace benchmark */
} /* namespace pink */


// LuM end of file
