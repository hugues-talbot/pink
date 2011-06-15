// UjoImro, 2011

// This is a research implementation of the NUMA polymorphic
// array. This array is allocated in several threads. The array can be
// accessed directly (by a pointer) and abstractly (by an operator). The
// direct access is fast, if the part of the memory is in the same
// thread which accesses it. The operator access is straightforward
// for the synchronization of the array between threads.

// 2011-04-26 in the first version the array is split up equally so if
// you have an array of size 100 and 7 threads, you will have the
// following limits (99 is the highest index):

// 0 .. 14 .. 28 .. 42 .. 57 .. 71 .. 85 .. 100

// Note, that the upper indices are not included. 
// for q in range(7): print int((100/7.)*q)

#ifndef UI__POLYTHREAD__HPP__
#define UI__POLYTHREAD__HPP__

#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api
#include <numa.h>
#include <vector>
#include <iostream>
#include <boost/timer.hpp>
#include <boost/thread.hpp>
//#include <boost/random.hpp>
//#include <boost/date_time.hpp>
#include <boost/smart_ptr.hpp>

//#include <glib/gchecksum.h>
#include "uiFibreTypes.h"
#include "ui_numa_types.hpp"

// uncomment the following line to activate some additional limit
// checking and debug messages
//#define UJIMAGE_DEBUG 5

namespace pink
{
  
  namespace numa
  {


    class numa_deleter_t;
    
    template <class array_t>
    class slow_iterator_t
    {
    public:
      typedef typename array_t::value_type value_type;

    private:
      value_type * value;
      index_t nextm1; // m1 stands for minus 1
      index_t pos;
      array_t array;
      
    public:      

      slow_iterator_t( array_t & array, index_t pos )
        : array(array), pos(pos), nextm1( array.segment_high(array.node(pos)) - 1 )
        {
          value = &(array[pos]);
        } /* slow_iterator_t */

      slow_iterator_t(): pos(-1), array() { }

      void reset( array_t & array, index_t pos )
        {
          this->array=array; // boost shared array is copied only if they are different
                    
          this->pos    = pos;
          this->value  = &(this->array[this->pos]);
          this->nextm1 = array.segment_high( array.node(pos)) - 1;          
        }
          
            
      value_type & operator*()
        {
          return *value;        
        }
      
      void operator++(int)
        {
          if (pos<nextm1)
          {            
            value++; pos++;            
          }
          else
          {
            pos++;
            value = &(array[pos]);
            nextm1 = array.segment_high( array.node(pos)) - 1;            
          }
        }    
      
    }; /* class slow_iterator_t */
   
    
    
    template <class array_t>
    void
    fast_iterator( typename array_t::value_type * & pointer, array_t & array, index_t pos )
    {
      pointer = &(array[pos]);
      return;
    } /* get_fast_iterator */
    

    
    template <class array_t>
    void    
    slow_iterator( slow_iterator_t<array_t> & slow_it, array_t & array, index_t pos )
    {
      slow_it.reset(array, pos);
      return;      
    } /* slow_iterator */



    template <class T0>
    class poly_array
    {
    public:
      typedef T0 value_type;
      typedef boost::shared_array<value_type> segment_t;
      index_t number_of_nodes;
      
    private:
      index_t size_;

      // boost::shared_array<value_type> segments;
      boost::shared_array<segment_t> segments;   
      
            
    public:
      
      poly_array(): size_(0) { }
      
      poly_array( index_t size )
        : size_(size),
          number_of_nodes(numa_max_node() + 1)
        {
          if ((size < number_of_nodes) or (size == 0))
          {
            pink_error("Distributed polymorhic array size is too small. "
                       "Either 0 or less then the number of threads.");            
          }

          segments.reset(new segment_t[number_of_nodes]);
                    
          // Getting the numa characteristics from the system
          if (numa_available() < 0)
          {
            pink_error("poly_array called in numa mode, but your system does not support the NUMA API");
          } /* if numa_available */



          FOR(q, number_of_nodes)
          {
            init(q);
          }
         
        } /* poly_array::poly_array */


      
      value_type & operator[](index_t pos)
        {
          index_t segment = node(pos);
          return segments[segment][pos - segment_low(segment)];
        }


      
      virtual ~poly_array()
        { }


      
      index_t size() const
        {
          return size_;          
        }


      
      /**
         \brief 
         description returns the length of the 'q'-th segment.
         
         \param q the ID of the segment
         \return the length corresponding with the segment
      */    
      index_t segment_size(index_t q)
        {
#         ifdef UJIMAGE_DEBUG
          if ((q < 0) or (q >= number_of_nodes)) 
          {
            pink_error("trying to reach outside of the thread range!");          
          } /* if element in 0..size_-1*/
#         endif /* UJIMAGE_DEBUG */
          
          return segment_high(q) - segment_low(q);        

        } /* poly_array::segment_size */

      

      index_t segment_low(index_t q)
        {
#         ifdef UJIMAGE_DEBUG
          if ((q < 0) or (q >= number_of_nodes)) 
          {
            pink_error("trying to reach outside of the thread range!");          
          } /* if element in 0..size_-1*/
#         endif /* UJIMAGE_DEBUG */

          return size_ * q / number_of_nodes; // note that this is an integer operation

        } /* poly_array::segment_low */
      
    
      index_t segment_high(index_t q)
        {
#         ifdef UJIMAGE_DEBUG
          if ((q < 0) or (q >= number_of_nodes)) 
          {
            pink_error("trying to reach outside of the thread range!");          
          } /* if element in 0..size_-1*/
#         endif /* UJIMAGE_DEBUG */

          return size_ * (q+1) / number_of_nodes;  // note that this is an integer operation

        } /* poly_array::segment_high */
      

     
      /**
         \brief This function returns the ID of the segment, which
         corresponds with the given pixels.
         
         description The polymorphic array is broken up to segments. For
         a given pixel in the abstract array segment_ID returns the id of
         the 'thread' in which holds the segment of the pixel 'element'.

         \brief Returns the node corresponding with the current segment.
         
         description If the segment is allocated with the numa policy in
         mind, then each segment belongs to a specific numa node. The
         segments are distributed equally between the nodes. This
         function returns the number of the node in which the segment in
         question is/will be allocated.
         
         note: this function is using the same algorithm as segment_ID
         
         \param segment The segment in question. 
         \return The number of the node.
         
         \param element The pixel in question
         
         \return The ID of the segment. The ID is between 0 and number_of_nodes-1
      */      
      index_t node(index_t element)
        {
#         ifdef UJIMAGE_DEBUG
          if ((element < 0) or (element >= size_)) 
          {
            pink_error("trying to reach an element outside of the array");          
          } /* if element in 0..size_-1*/        
#         endif /* UJIMAGE_DEBUG */

          if ( number_of_nodes * (element+1) % size_ == 0 )
          {
            return number_of_nodes * (element) / size_;
          }
          else /* NOT number_of_nodes * (element+1) % size_ == 0 */
          {      
            return number_of_nodes * (element+1) / size_;
          }/* NOT number_of_nodes * (element+1) % size_ == 0 */

          return -1; // some compilers complain

        } /* poly_array::node */



      /**
         \brief allocates the i-th segment of the array
         
         \description If the segment is already allocated, than it
         throws an error. This function can only be used for
         'SEMI_LOCAL' allocation.
         
         \param q the ID of the segment
         
         \return Returns a 'boost::shared_array' with the allocated
         segment.
      */
      segment_t init(index_t q) // q represents the number of the thread
        {

          
          value_type * current_segment = NULL;
          
          current_segment = reinterpret_cast<value_type*>(
            numa_alloc_onnode( segment_size(q) * sizeof(value_type),
                               node(q)
              )
          );
          
          // testing if the allocation had succeeded. Numa allocation
          // is much more risky then malloc or new, because the system
          // might need to reallocate some pages, so the testing of
          // the success of the allocation is crucial!
          if (current_segment==NULL)
          {
            pink_error("the allocation of the memory segment did not succeed.");            
          }
          
          // the following object will take care of the deletion of the object
          numa_deleter_t cleaner(segment_size(q));          

          // after the attribution to this smart shared array, the deletion of the
          // memory should happen automaticly
          segments[q].reset(current_segment, cleaner);

          return segments[q];        

        } /* poly_array::init(q)*/

      /**
         \brief This function returns a pointer to the first segment
         data.
         
         description It allows the comparison between the
         poly_array-s.
         
      \return A pointer to the data
      */      
      value_type * get()
        {
          if (size_>0)
          {            
            return segments[0].get();            
          } 
          else  /* NOT size>0 (the poly_array is not yet initialized)*/
          {
            return NULL;            
          }          
        }
      










      
    }; /* class poly_array */
    
      
    
    
    /**
       \brief This is a special deleter function for numa_arrays.
       
       description Instead of delete, this function will call the
       'numa_free' function.
    */    
    class numa_deleter_t
    {
    private:
      index_t size;

    public:
      numa_deleter_t(index_t size): size(size)
        {}
            
      template <class T0>       
      void operator()(T0 ptr)
        {
          numa_free( reinterpret_cast<void*>(ptr), size );
          //delete[] ptr;          
        } /* operator() */
      
    }; /* class numa_deleter_t */

    
    
    class distributor_t
    {
    private:
      index_t number_of_threads;
      index_t size_;
      
    public:
      distributor_t(index_t size, index_t number_of_threads):
        size_(size),
        number_of_threads(number_of_threads)
        {}

      index_t size( index_t ID )
        {
          return high(ID) - low(ID);
        } /* size */
      
      index_t low( index_t ID )
        {
          return size_ * ID / number_of_threads; // note that this is an integer operation
    
        } /* low */

      index_t high( index_t ID )
        {
          return size_ * ( ID + 1 ) / number_of_threads;  // note that this is an integer operation          
        } /* high */

      index_t node( index_t thread )
        {
          if ( number_of_threads * (thread+1) % size_ == 0 )
          {
            return number_of_threads * (thread) / size_;
          }
          else /* NOT number_of_threads * (thread+1) % size_ == 0 */
          {      
            return number_of_threads * (thread+1) / size_;
          }/* NOT number_of_threads * (thread+1) % size_ == 0 */
          
          return -1; // some compilers complain
        }
      
    }; /* class distributor_t */
    
  } /* namespace numa */
  
} /* namespace pink */

#endif /* UI__POLYTHREAD__HPP__ */
// LuM end of file
