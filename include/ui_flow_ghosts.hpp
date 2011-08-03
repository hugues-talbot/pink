/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// parts of this code are based on http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
// -O3 -DNDEBUG -ipo -no-prec-div -fp-model fast=2 -msse4.2

#ifndef UI_FLOW_GHOSTS__HPP__
#define UI_FLOW_GHOSTS__HPP__


//!! note this is a hack. Pink's using depth as a macro for accessing
//!! the z size of the image
#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api
#include <set>
#include <queue>
#include <numa.h>
#include <sched.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "uiFibreTypes.h"
#include "ui_polythread.hpp"

// system call for the thread binding
inline
long int
gettid()
{
  return static_cast<long int>(syscall(__NR_gettid));  
} /* gettid */

//_syscall0(pid_t,gettid)
//pid_t gettid(void);


namespace pink {
  namespace numa {

    
    
    typedef boost::mutex mutex_t;
    typedef std::pair<index_t, index_t> range_t;
    typedef boost::shared_ptr<mutex_t> pmutex_t;    
    typedef boost::lock_guard<mutex_t> scoped_lock;


    
    template<class T0>
    struct cmpkernel
    {
      inline bool operator()( const T0& a, const T0& b)
        {
          return a.first < b.first;  
        }
    }; /* class cmpfirst */



    template<class T0>
    struct cmptime
    {
      inline bool operator()( const T0& a, const T0& b)
        {
          return a.second > b.second;  
        }
    }; /* class cmpsecond */


    
    template <class T0 = index_t>
    class partitioner_t
    {
    public:

      T0      tag;      
      range_t size;
      
    private:
      
      mutex_t guard;
      index_t grain_size;
      

  
    public:

      partitioner_t( range_t size, index_t grain_size = 1 )
        : size(size), grain_size(grain_size), guard() { }



      partitioner_t( index_t grain_size = 1 )
        : size(-1,-1), grain_size(grain_size), guard() { }

  

      void reset( range_t size )
        {          
          this->size = size;
          return;      
        }
  

      range_t pop()
        {
          range_t result;
          scoped_lock scp(guard);          
          if ((size.second - size.first) <= grain_size)
          {
            result = size;
            size.first = -1;
            size.second = -1;
          }
          else
          {
            result.first  = size.first;
            result.second = size.first + (size.second - size.first) / 2;
            size.first    = result.second;
          }
      
          return (result);
        } /* pop */
  

  
      index_t occupancy() const
        {          
          return (size.second - size.first);      
        }
  
  
  
      range_t steal()
        {
          range_t result;
          scoped_lock scp(guard);
          if ((size.second - size.first) <= grain_size)
          {
            result      = size;
            size.first  = -1;
            size.second = -1;
          }
          else
          {
            result.second = size.second;
            result.first  = size.first + (size.second - size.first) / 2;
            size.second   = result.first;
          }

          return (result);
        } /* steal */ 


  
      bool empty()
        {
          return (size.first == size.second);
        }
  
    }; /* partitioner_t */


    
    class kernel_update_t
    {
    public:

      kernel_update_t() { }

      virtual ~kernel_update_t() { }

      virtual void execute( const range_t& ) = 0;
      
    }; /* kernel_update_t */
    typedef boost::shared_ptr<kernel_update_t> pkernel_update_t;    
    // typedef kernel_update_t* pkernel_update_t;


    
    template <class task_t>
    class mutable_priority_queue_t
    {
    public:
      typedef std::pair<task_t, index_t> value_type;
      typedef std::set<value_type, cmpkernel<value_type> >    kernel_t;
      typedef std::multiset<value_type, cmptime<value_type> > time_t;
    
    private:

      time_t   times;
      pmutex_t guard;
      kernel_t kernels;  

      
  
    public:

      mutable_priority_queue_t()
        : guard(new mutex_t) { }


      
      bool empty()
        {
          // scoped_lock scl(guard);
          return kernels.empty();      
        } /* mpq::empty */
  

      
      void push( value_type val )
        {
          scoped_lock scl(guard);
          kernels.insert(val);
          times.insert(val);      
        } /* mpq::push */



      value_type pop()
        {
          value_type result(-1, -1);
          scoped_lock scl(guard);
          if (not times.empty())
          {
            result = (*times.begin());
            kernels.erase( kernels.find( *times.begin() ));
            times  .erase( times.begin() );
          }
          
          return(result);
        } /* mpq::pop */
  
  
  
      void try_erase( value_type val )
        {
          scoped_lock scl(guard);
          times  .erase(val);
          kernels.erase(val);      
        } /* mpq::try_erase */

    }; /* mutable_priority_queue_t */



    template < class T0 = index_t >
    struct thread_private_t
    {
      typedef boost::shared_ptr<partitioner_t<> > ppartitioner_t;

      T0      tag;      
      index_t node;
      index_t core;
      index_t part; // the assigned partitioner      
      ppartitioner_t partitioner;
      
      thread_private_t( index_t core, index_t node, index_t part, ppartitioner_t partitioner )
        : node(node), core(core), part(part), partitioner(partitioner) { }
      
    }; /* struct thread_private_t */
    typedef boost::shared_ptr<thread_private_t<> > pthread_private_t;    
    
    

    template < class T0 = index_t >
    struct thread_common_t
    {
      typedef typename thread_private_t<>::ppartitioner_t ppartitioner_t;      
      typedef std::vector<ppartitioner_t>                 partitioners_t;

      T0               tag;      
      bool             quit;
      pkernel_update_t kernel;
      boost::barrier   work_end;
      boost::barrier   work_begin;
      partitioners_t   partitioners;
      mutable_priority_queue_t<index_t> grand_queue;      
      std::vector< mutable_priority_queue_t<index_t> > queues;
      

      
      thread_common_t( index_t nodes, index_t number_of_threads, index_t grain_size )
        : quit(false), queues(nodes),
          // note that the + 1 is for the scheduler
          work_begin( number_of_threads + 1 ), work_end( number_of_threads + 1 ),
          partitioners(number_of_threads)
        {
          FOR( q, number_of_threads )
          {
            partitioners[q].reset( new partitioner_t<>(grain_size) );            
          } /* FOR q in number_of_threads */
        } /* thread_common_t */

    }; /* struct thread_common */
    typedef boost::shared_ptr<thread_common_t<> > pthread_common_t;    

    
    
    template <BOOST_PP_ENUM_PARAMS(3, class T)>
    void laborer( T0 thread_common, T1 thread_private, T2 iterations )
    {
      index_t core  = thread_private->core;
      index_t node  = thread_private->node;
      index_t part  = thread_private->part;      

      // restricting all memory operations and affinity to the
      // selected numa node of this thread for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node);
      numa_bind(&mask);
            
      // restricting myself to a particular core and nothing else
      cpu_set_t cpu_set;
      CPU_ZERO(&cpu_set);
      CPU_SET( core, &cpu_set );
      sched_setaffinity( gettid(), sizeof(cpu_set), &cpu_set );

      range_t range, advertise;

      // eager to work
      thread_common->work_begin.wait();
      while ( not thread_common->quit )
      //FOR(q, iterations)
      {
        thread_common->kernel->execute( thread_private->partitioner->size );        
        // // in the first part of the computation I extinguish my calculcations (pop)
        // while ( not thread_private->partitioner->empty() )
        // {
        //   // fetch a part for myself
        //   range = thread_private->partitioner->pop();

        //   // advertise the rest
        //   advertise = range_t( part, thread_private->partitioner->occupancy() ); 
        //   thread_common->queues[node].push(advertise); // advertise to the node
        //   thread_common->grand_queue.push(advertise); // advertize to all system
          
        //   // calculate
        //   thread_common->kernel->execute(range);

        //   // revoke advertising
        //   thread_common->queues[node].try_erase(advertise);
        //   thread_common->grand_queue.try_erase(advertise);
        // } /* while I have work to do on my own */
        
        // // now I look around if there are some threads still working on this node (steal)
        // while ( not thread_common->queues[node].empty() )
        // {
        //   advertise = thread_common->queues[node].pop();
        //   if ( advertise.second >= 0 )
        //   {
        //     range = thread_common->partitioners[advertise.first]->steal();
        //     thread_common->kernel->execute(range);
        //   } /* advertise.second >= 0 */
          
        // } /* while there are tasks left on the node */

        // // now I look around if there are some threads still working on this node (steal)
        // while ( not thread_common->grand_queue.empty() )
        // {
        //   advertise = thread_common->grand_queue.pop();
        //   if ( advertise.second >= 0 )
        //   {
        //     range = thread_common->partitioners[advertise.first]->steal();
        //     thread_common->kernel->execute(range);
        //   } /* advertise.second >= 0 */
          
        // } /* while there are tasks left on the node */


        thread_common->work_end.wait();
        thread_common->work_begin.wait();
      } /* while not thread_common->quit */
      
      return;      
    } /* void laborer */
        


    template < class T0 = index_t >
    class scheduler_t
    {
    public:
      T0 tag;      
      typedef boost::shared_ptr<boost::thread>          pthread_t;
      typedef std::vector<pthread_t>                    threads_t;


      
    private:
      index_t grain_size;
      index_t number_of_cores;      
      index_t number_of_nodes;
      index_t threads_per_node;
      index_t number_of_threads;
      
      threads_t        threads;
      pthread_common_t thread_common;      


      
    public:
      scheduler_t( index_t number_of_threads, index_t resolution, index_t iterations )
        : number_of_threads(number_of_threads), grain_size(resolution)
        {
          // Getting the numa characteristics from the system
          if (numa_available() < 0)
          {
            pink_error("This scheduler wants to be conscious, but your system does not support the NUMA API");
          } /* if numa_available */
          
          number_of_nodes  = numa_max_node() + 1;
          number_of_cores  = boost::thread::hardware_concurrency();
          threads_per_node = number_of_threads / number_of_nodes;
          thread_common.reset( new thread_common_t<>( number_of_nodes, number_of_threads, grain_size ) );
          
          // creating the worker threads, the threads will be bound to
          // the core for the cache scheduling
          threads.resize(number_of_cores);
          
          FOR( node, number_of_nodes )
          {
            FOR( q, threads_per_node )
            {
              threads[ node * threads_per_node + q ].reset(
                new boost::thread(
                  laborer<pthread_common_t, pthread_private_t, index_t>,
                  thread_common,
                  pthread_private_t(
                    new thread_private_t<>(
                      /* core */ node * (number_of_cores / number_of_nodes) + q,
                      /* node */ node,
                      /* part */ node * threads_per_node + q,
                      thread_common->partitioners[ node * threads_per_node + q ]
                      )),
                  iterations
                  ));
            } /* FOR q in threads_per_node */
          } /* FOR node in number_of_nodes */
          
          return;          
        } /* scheduler_t */



      ~ scheduler_t()
        {
          thread_common->quit = true;
          thread_common->work_begin.wait();          
          
          FOR(q, number_of_threads)
          {
            threads[q]->join();            
          } /* FOR q in number_of_cores */          
        } /* ~scheduler_t */
      


      void schedule_work( pkernel_update_t kernel, range_t range )
        {          
          thread_common->kernel = kernel;
          // distributing the range equally among the laborers
          numa::distributor_t distributor( range.second - range.first, number_of_threads );
          FOR(q, number_of_threads)
          {
            thread_common->partitioners[q]->reset( range_t( range.first + distributor.low(q), range.first + distributor.high(q)));
          } /* for q in number_of_threads */
          
          // // launch the threads
          thread_common->work_begin.wait();
          // wait for the threads to finish the calculation
          thread_common->work_end.wait();
          return;          
        } /* schedule_work */
      
    }; /* class scheduler_t */
    
      
    
    
  } /* end namespace numa */
} /* end namespace pink */
#endif /* UI_FLOW_GHOSTS__HPP__ */
/* LuM end of file */
