/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_DISTRIBUTED_HPP_
#define UIFLOW_DISTRIBUTED_HPP_

#define BOOST_DISABLE_ASSERTS

#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api
#include <numa.h>
#include <vector>
#include <utility>
#include <sched.h>
#include <boost/mpl/at.hpp>
#include <boost/thread.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

//#include "uiFlow.hpp"
#include "uiFrame.hpp"
#include "pyujimage.hpp"
#include "ui_compactor.hpp"
#include "ui_polythread.hpp"





namespace pink {
  
  namespace numa {

#   define REPORT_INTERVAL 10

    class semaphores_t
    {
      typedef boost::shared_mutex shared_mutex_t;
      typedef boost::shared_ptr<shared_mutex_t> pshared_mutex_t;
      typedef boost::shared_ptr<bool> pbool_t;      

    public:

      pbool_t keep_up;      
      
      pshared_mutex_t iteration_begin;
      pshared_mutex_t potencial_begin;
      pshared_mutex_t flow_begin;
      pshared_mutex_t const_begin;
      pshared_mutex_t srcsink_begin;
      
      pshared_mutex_t iteration_end;
      pshared_mutex_t potencial_end;
      pshared_mutex_t flow_end;
      pshared_mutex_t const_end;
      pshared_mutex_t srcsink_end;

      pshared_mutex_t loop_begin;
      pshared_mutex_t loop_end;
      pshared_mutex_t loop_restart;
      pshared_mutex_t loop_restart_lockable;
      

      semaphores_t():
        keep_up( new bool ),
        
        iteration_begin( new shared_mutex_t ),
        potencial_begin( new shared_mutex_t ),
        flow_begin     ( new shared_mutex_t ),
        const_begin    ( new shared_mutex_t ),
        srcsink_begin  ( new shared_mutex_t ),
        
        iteration_end  ( new shared_mutex_t ),
        potencial_end  ( new shared_mutex_t ),
        flow_end       ( new shared_mutex_t ),       
        const_end      ( new shared_mutex_t ),
        srcsink_end    ( new shared_mutex_t ),

        loop_begin     ( new shared_mutex_t ),
        loop_end       ( new shared_mutex_t ),
        loop_restart   ( new shared_mutex_t ),
        loop_restart_lockable ( new shared_mutex_t )
        
        { } /* semaphores_t constructor */
      
      
    }; /* semaphores_t */
    
    template <class CT0>
    class dealers_t
    {
    public:
      typedef CT0 dealer_t;
      typedef typename dealer_t::array_t array_t;

      std::vector<dealer_t> potencial;
      vector2D<dealer_t>      flow;
      std::vector<dealer_t> constrain;
      index_t number_of_nodes;
      index_t d;      

      template <class T0, class T1, class T2, class T3, class T4>
      dealers_t( T0 potencials, T1 flows, T2 constrains, T3 number_of_nodes, T4 d )
        : number_of_nodes(number_of_nodes), d(d)
        {
          potencial.resize(number_of_nodes);
          FOR(q, number_of_nodes)
          {
            potencial[q] = dealer_t(potencials[q]);
          } /* FOR q, number_of_nodes */
                    
          flow.resize(d);
          FOR(q, d)
          {
            flow[q].resize(number_of_nodes);            
            FOR(w, number_of_nodes)
            {
              flow[q][w] = dealer_t(flows[q][w]);              
            } /* FOR q, number_of_nodes */
            
          } /* q, d */
 
          constrain.resize(number_of_nodes);
          FOR(q, number_of_nodes)
          {
            constrain[q] = dealer_t(constrains[q]);
          } /* q, number_of_nodes */
         
          
        } /* dealers_t */
      
      void reset()
        {
          FOR(q, number_of_nodes)
          {
            potencial[q].reset();            
          } /* FOR q, number_of_nodes */
                    
          FOR(q, d)
          {
            FOR(w, number_of_nodes)
            {
              flow[q][w].reset();              
            } /* FOR q, number_of_nodes */
          } /* q, d */
 
          FOR(q, number_of_nodes)
          {
            constrain[q].reset();            
          } /* q, number_of_nodes */
          
        } /* dealers_t reset */
      
      
    }; /* class dealers_t */
    
    
      
    template<class mutex_t>
    void checkpoint( mutex_t & mutex )
    {
      mutex->lock();
      mutex->unlock();   
    } /* checkpoint */

    template<class mutex_t>
    void shared_checkpoint( mutex_t & mutex )
    {
      mutex->lock_shared();
      mutex->unlock_shared();   
    } /* shared_checkpoint */


    template <class T>
    inline T isqr(const T & x)
    {
      return x * x;
    }
    
    
    
    /**
       \brief Charges the mutex for signaling the end of the
       calculation.
       
       description This function takes a boost shared_mutex and
       lock_shared 's it N times.  This way each thread can
       unlock_shared it after the thread has finished its part of the
       calculation. When the last thread shared_unlock -ed the mutex,
       the mutex becomes checkpointable.

       \param mutex The shared mutex to charge \param
       number_of_threads The number of the threads which are
       calculating.

       \return void
    */    
    template<class mutex_t, class nbt_t >
    void charge_mutex( mutex_t & mutex, nbt_t number_of_threads )
    {
      FOR(q, number_of_threads)
      {
        mutex->lock_shared();        
      } /* for number_of_threads */
      
    } /* charge_mutex */
    
    

    // /**
    //    \brief This class manages a shared pointer array
       
    // description The class behaves is a 2D array of smart pointers, but
    // its interface is that of a simple 2D array. It can be allocated by:
    
    // ptr_array<array_t> array(length)

    // initialized as:

    // array.reset(q, new array_t(array_t_length) )

    // and accessed as:

    // array[q][w]
    // */    
    // template<class data_t>
    // class ptr_array
    // {
    // private:
    //   boost::shared_array< boost::shared_ptr<data_t> > data;
      
    // public:
    //   ptr_array(index_t size): data(new boost::shared_ptr<data_t>[size]) {}

    //   data_t & operator[](index_t q)
    //     {
    //       return (*data[q]);      
    //     }
      
    //   void reset(index_t q, data_t * newptr )
    //     {
    //       data[q].reset(newptr);
    //     }
    // }; // ptr_array

    template <class image_type>
    class distributed_flow;    
    
    // this is the thread function, which will be used for iteration
    template <class image_type>
    void maxflow_iterator(
      pink::numa::distributed_flow<image_type> & obj,
      index_t ID,
      index_t node_of_this_thread
      );    

    
    template <class image_type>
    class distributed_flow 
    {
      
    private:

      typedef typename image_type::pixel_type   pixel_type;
      //typedef typename pink::numa::poly_array<pixel_type>     poly_array_t;
      //typedef std::vector<pixel_type> poly_array_t;
      typedef poly_array<pixel_type>            poly_array_t;      

      typedef boost::shared_mutex               shared_mutex_t;
      typedef boost::shared_ptr<shared_mutex_t> pshared_mutex_t;

      typedef dealer_t<vector2D<dibble_t> >     mdealer_t;      
      typedef dealers_t<mdealer_t>              mdealers_t;
      typedef mdealer_t::packet_t               packet_t;      
      

      friend void checkpoint       <pshared_mutex_t> ( pshared_mutex_t& );
      friend void shared_checkpoint<pshared_mutex_t> ( pshared_mutex_t& );
      friend void charge_mutex     <pshared_mutex_t> ( pshared_mutex_t&, index_t );
      friend void maxflow_iterator <image_type> (
        pink::numa::distributed_flow<image_type> & obj,
        index_t ID,
        index_t node_of_this_thread
        );
      

      
      // the result will be put in this image
      image_type gg;
      char_image src_sink;
      image_type potencial;      

//    boost::shared_array<pixel_type> pot_glob;

      // these distributed arrays contain the fields during the iterations
      /* changed from std::vector*/ std::vector<poly_array_t> flow_glob; // flow is allocated in each direction, so we have as many flows as directions
      poly_array_t  g_glob;
      poly_array_t  pot_glob;

      index_t starttime;
      index_t number_of_threads;
      index_t number_of_nodes;      
      index_t resolution;
      
      bool    flow_calculated;  // this variable hold if start function has already been called, 
      // if it hasn't than pot_glob and flow_flob are not yet allocated
   
      // functions for threading    
      index_t   packet_size; // this variable holds the number of consecutive
      // dibbles to be processed by the same thread. If
      // packet_size==n then each thread processes n
      // dibbles for on one charge. n dibbles is called
      // a packet.

      // this structure holds the list of the dibbles' purity. If the
      // value of the vector is a nonnegative value between 0 and the
      // number_of_threads-1, then the given dibble is pure to that
      // node. If the value is negative then the dibble is overlapping
      // in at least one calculation. If the value is positive and
      // greater then the number of threads, then the particular
      // dibble has not yet been considered.
      vint         pure_pot;
      /* changed from std::vector*/ std::vector<vint> pure_flow;
      vint         pure_cons;
      semaphores_t semaphores;
      boost::shared_ptr<mdealers_t>   dealers;      

      // this structure holds the compactified pure dibbles as well as
      // the collected tainted dibbles for the synchronizer

      vector2D<dibble_t> compact_srcsink;      
      vector2D<dibble_t> compact_pot;
      vector3D<dibble_t> compact_flow;      
      vector2D<dibble_t> compact_cons;

      vector3D<dibble_t> resolut_pot;
      vector4D<dibble_t> resolut_flow;
      vector3D<dibble_t> resolut_cons;
      
      
      vector2D<dibble_t> synchro_src;
      vector2D<dibble_t> synchro_sink;
      vector2D<dibble_t> synchro_pot;
      vector3D<dibble_t> synchro_flow;      
      vector2D<dibble_t> synchro_cons;
      

      
      // these structures hold the pure dibbles partitioned by the
      // nodes. If there are 4 nodes in the system, then
      // thread_pot.size() is 4 and thread_pot[0] holds the list of
      // the pure dibbles on node 0.
      // /* changed to shared_vector */ shared_vector<vint> thread_pot;
      // /* changed to shared_vector */ shared_vector< /* changed to shared_vector */ shared_vector<vint> > thread_flow;
      // /* changed to shared_vector */ shared_vector<vint> thread_cons;
            
    protected:

      index_t d;
      index_t length_glob;
      vint    dim;
      float   tau;
      
      progressBar sentinel;

      // the number of desired iterations
      index_t       iteration;

      /* changed from std::vector*/ std::vector<dibble_t> dib_constrain;
      /* changed from std::vector*/ std::vector<dibble_t> dib_potencial;

      // this way when the vector destroys the array it will
      // destroy all the elements as well
      vector2D<dibble_t> dib_flow; 


      
      // functions for calculation
      template <class array_t, class fiterator>
      void update_constrain( const array_t & array, fiterator iterator )        
      //void update_constrain(index_t startDibble, index_t endDibble)
        {

          //typedef typename boost::function_types::result_type<fiterator>::type iterator_t;

          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;
          
          //     //local copies
          pixel_type dFabs[d];
          iterator_t locInFlow[d];
          iterator_t locOutFlow[d];
          iterator_t locG;
          pixel_type v;
	
          //locals
          vint fm1s(d), fm1_vec(d);
          index_t start, end, length;
	
          // calculating differences between the flow and the point
          FOR( w, d )
          {
            fm1_vec.reset();
            fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
            fm1s[w]=dim.position(fm1_vec);
          }

          FOR(e, array.size())
          {
	
            // //for (/*int*/ e=0; e<=dib_constrain->length-1; e++){
            // for (index_t e=start_dibble; e < end_dibble; e++)
            // {
            start  = array[e].first;
            end    = array[e].second;
            length = end - start;
              
            iterator(locG, g_glob, start);
            FOR(w,d)
            {
              iterator(locOutFlow[w], flow_glob[w], start );
              iterator(locInFlow[w],  flow_glob[w], start - fm1s[w] );
            }
            FOR(r, length)
            {
              v=0.;
              FOR(w, d)
              {
                // dFabs=max(-Fin, 0, Fout)
                dFabs[w] = -(*locInFlow[w]) > (*locOutFlow[w]) ? -(*locInFlow[w]) : (*locOutFlow[w]);
                if (0. > dFabs[w]) dFabs[w] = 0.;
                // v = sum(dFabs^2)
                //v += static_cast<pixel_type>(dFabs[w] * dFabs[w]);
                v += isqr(dFabs[w]);                  
              }

              FOR(w, d)
              {
                // multiplying
                if ( isqr(*locG) < v ) dFabs[w] *= (*locG) / static_cast<pixel_type>(sqrt(v));
                // testing the final condition
                if ( (*locInFlow[w])  < -dFabs[w]) (*locInFlow[w])  = -dFabs[w];
                if ( (*locOutFlow[w]) > dFabs[w] ) (*locOutFlow[w]) = dFabs[w];
              }
              // stepping;
              locG++;

              FOR(w, d)
              {
                locInFlow[w]++;
                locOutFlow[w]++;
              } /* FOR w, d */
                

            } /* for r in length */
          } /* FOR e in array.size() */
          
        } /* update_constrain */



      /**
         \brief Updates the potencial.
         
      description 

      \param array

      \param iterator fiterator is a function pointer. It can be
      fast_iterator and slow_iterator. Fast iterator simply returns a
      pointer, while slow iterator does bond checking a shift, so it
      also works across the numa nodes. 
      
      \return 
      */      
      template<class array_t, class fiterator>
      void update_potencial(const array_t & array, fiterator iterator )
      //void upDatePotencial(index_t startDibble, index_t endDibble)      
        {
          //typedef typename boost::function_types::result_type<fiterator>::type iterator_t;
          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;

          iterator_t p_c, f_out, f_in;          
          index_t fm1 /*fm1_vec[d]*/, start, end;            
          vint fm1_vec(d);
            
          FOR(w, d)
          {
            fm1_vec.reset();
            fm1_vec[w]=1; //we are calculating the distance the opposite direction but it should be the same
            fm1=dim.position(fm1_vec);
            
            FOR( e, array.size() )
            {
              //std::cout << " q= " << q << " w= " << w << " e= " << e
              //<< " dib_potencial.size()= " << dib_potencial.size() << std::endl;
                
              start = array[e].first;
              end   = array[e].second;
                
              iterator(p_c,   pot_glob,     start);
              iterator(f_out, flow_glob[w], start);
              iterator(f_in,  flow_glob[w], start-fm1);

              //the hyper-super ultra fast loop
              for ( index_t r=start; r<end; r++, p_c++, f_out++, f_in++ )
              {
                *p_c -= tau * ( *f_out - *f_in );
              }
            } /* for e in array_size */
          } /* FORR d */

               
        } /* update_potencial */


      
      template <class array_t, class fiterator>
      void update_flow(const array_t & array, fiterator iterator, int w /*direction*/)
      //void update_flow(index_t startDibble, index_t endDibble, int w /*direction*/)
        {
          //typedef typename boost::function_types::result_type<fiterator>::type iterator_t;
          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;

          iterator_t p, pp1, f;
            
          index_t start, end, pp1_pos;
          vint pp1_vec(d);

          pp1_vec.reset();
          pp1_vec[w]=1;
          pp1_pos=dim.position(pp1_vec);

          FOR(q, array.size())
          {
            start  = array[q].first;            
            end    = array[q].second;

            iterator(f,   flow_glob[w], start );
            iterator(p,   pot_glob,     start );
            iterator(pp1, pot_glob,     start + pp1_pos );
              
            for (index_t r=start; r<end; r++, f++, p++, pp1++)
            {
              *f -= tau*( *pp1 - *p );
              //flow_glob[w][r] -= tau*(pot_glob[pp1_pos+r]-pot_glob[r]);                  
            }
          } /* FOR q in array.size */
      
        } /* update_flow */

      

      template <class array_t>
      void update_srcsink( const array_t & array, pixel_type val )
        {
          FOR(q, array.size())
          {            
            for( index_t e = array[q].first; e < array[q].second; e++ )
            {
              pot_glob[e]=val;              
            } /* FOR e in first..second */                        
          } /* FOR q in array.size() */
          
        } /* update_src */
      

      
      int create_dibbles()        
        {
          // it is demanded that all the fragments on the border of the image are sink
          // for every dibble, the higher end is NOT included

// following we will parallelize the creation of the dibbles
// as there are three parts we only create 3 threads
          //#pragma omp parallel num_threads(3) 
          { 

            //#pragma omp sections
            {

              //  -------- potencial -------
              // all the fragments different from source and sink are to be exclusively among the dibbles
              //#pragma omp section
              {
                bool started = false;
                index_t start=0, end=0;
                index_t currlength=0;
                boost::shared_array<unsigned char> curr;
                curr = src_sink.get_pixels();
                FOR(q, length_glob) {
                  if (started) {
                    currlength++;
                    if (curr[q]!=0) {
                      end=q;
                      dib_potencial.push_back(dibble_t(start, end));
                      ///!!! std::cout << "dib_potencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
                      start=0;
                      end=0;
                      started=false;
                    } else /* NOT (curr[q]!=0) */
                      if (currlength>=MaxDibble) {
                        end=q+1;
                        dib_potencial.push_back(dibble_t(start, end));
                        ///!!! std::cout << "maxdibble dib_potencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
                        start=0;
                        end=0;
                        started=false;
                      } /* (currlength>=MaxDibble) */
                  } 
                  else  /* NOT (started) */
                  {
                    if (curr[q]==0){
                      started=true;
                      start=q;
                      currlength=1;
                    } /* (curr[q]==0) */
                  } /* NOT (started) */
                } /* FOR(q, length_glob) */
              } /* end pragma omp section */
  
  
              //  -------- flow ------------
              // all the arrows with at least one end not in src or sink are to be exclusively among the dibbles
              //#pragma omp section
              {
                bool started=false;
                index_t start=0, end=0;
                unsigned char *pp1[d];
                boost::shared_array<unsigned char> p;
                vint pp1_vec(d);
                index_t currlength=0;
                vint pp1_pos(d);
                // Calculating the shift of the neighbourh elements
                FOR(w, d) {
                  pp1_vec.reset();
                  pp1_vec[w]=1;
                  index_t currpos = dim.position(pp1_vec);
///!!!	std::cout << "currpos=" << currpos << std::endl;
                  pp1_pos[w]=currpos;
                  pp1[w]=&(src_sink(currpos));
                } /* FOR(w, d) */
                p=src_sink.get_pixels();
                FOR(w, d) {
                  FOR(q, length_glob - pp1_pos[w] /* - 1 */ ) { /////////
                    if (started){
                      currlength++;
                      if ((p[q]!=0) and (pp1[w][q]!=0)){
                        end=q;
                        dib_flow[w].push_back(dibble_t(start, end));
                        ///!!! std::cout << "dibFlow[" << w << "]->addElement(" << start << "," << end << ")" << std::endl;
                        start=0;
                        end=0;
                        started=false;
                      } else /* NOT ((p[q]!=0) and (pp1[w][q]!=0)) */
                        if (currlength>=MaxDibble){
                          end=q+1;/////!!!!!!!!!!!!!!!
                          dib_flow[w].push_back(dibble_t(start, end));
                          start=0;
                          end=0;
                          started=false;
                        } /* (currlength>=MaxDibble) */
                    } else { /* NOT (started) */
                      if ((p[q]==0) or (pp1[w][q]==0)){
                        started=true;
                        start=q;
                        currlength=1;
                      }
                    } /* NOT (started) */
                  } /* FOR(q, length_glob-pp1_pos[w]-1) */
                } /* FOR(w, d) */
              } /* pragma omp section */
  
              //  -------- constrain -------
              // all the regular points and sources with at least one regular neighbour
              //#pragma omp section
              {
                bool started=false;
                index_t start=0, end=0;
                unsigned char *pp1[d], *pm1[d];
                boost::shared_array<unsigned char> p;
                vint pp1_vec(d);
                index_t currlength=0;
                index_t max=0;
                FOR(w, d) {
                  pp1_vec.reset();
                  pp1_vec[w]=1;
                  index_t currpos = dim.position(pp1_vec);
                  max = max > currpos ? max : currpos;
                  pp1[w]=&( src_sink(currpos) );
                  pm1[w]=&( src_sink(0) ) - currpos;// risky, we always read far enough from the beginning
                } /* FOR(w, d) */
                p=src_sink.get_pixels();
                for ( index_t q=max; q < length_glob - max; q++ )
                {
                  bool i_am_a_regular_point = (p[q]==0.);
                  bool i_am_a_source = (p[q]==1.);
                  bool there_is_a_regular_point_near = false;
                  FOR(w, d) {
                    if ((pp1[w][q]==0.) or (pm1[w][q]==0.)) there_is_a_regular_point_near=true;
                  }
                  bool i_want_to_be_in_a_dibble = (i_am_a_regular_point or (i_am_a_source and there_is_a_regular_point_near));
                  //bool i_want_to_be_in_a_dibble = (i_am_a_regular_point or i_am_a_source);
                  if (started){
                    currlength++;
                    bool too_long = (currlength>=MaxDibble);
                    if (not i_want_to_be_in_a_dibble){
                      end=q;
                      dib_constrain.push_back(dibble_t(start, end));
                      ///!!! std::cout << "dib_constrain->addElement(" << start << ", " << end << ")" << std::endl;
                      start=0;
                      end=0;
                      started=false;
                    } else if (too_long){
                      //printf("uiNotice: breaking because of the MaxDibble" << std::endl;);
                      end=q+1;///////!!!!!!
                      dib_constrain.push_back(dibble_t(start, end));
                      start=0;
                      end=0;
                      started=false;
                    }
                  } else { /* NOT started */
                    if (i_want_to_be_in_a_dibble){
                      started=true;
                      start=q;
                      currlength=1;
                    } /* if (i_want_to_be_in_a_dibble) */
                  } /* NOT started */
                } /* FOR ( q, length_glob-max-1 ) */
              } /* pragma omp section*/
            } /* pragma omp sections */
          } /* pragma omp parallel */
          return 0; 

        } /* uiCreateDibbles */

      /**
         \brief This function detects the pure and tainted segments
         
      description The segments of the flow and the potencial are
      distributed between nodes of the system memory. If a thread
      iterates over a dibble, then the segments of this dibble has to
      be in fully the thread's node. This function therefore detects
      the overlapping dibbles. If a dibble is in a sole thread with
      all its segments, it is attributed the thread's number. If it is
      overlapping, it is marked tainted. In the end the function
      populates the pure thread list of the object.

      */      
      void tainted_dibbles()
        {
          // for a dibble to be to be pure, all the segments it's
          // using has to be in the same node. These segments include
          // the flow and the potencial points used during the
          // iteration.
          
          // resetting pure_potencial dibbles. Using the value
          // nbt+10 means, that the dibble is neither associated
          // with any thread nor rejected yet.
          pure_pot.resize(dib_potencial.size(), number_of_nodes + 10);
          pure_flow.resize(d);
          FOR(q, d)
          {
            pure_flow[q].resize(dib_flow[q].size(), number_of_nodes + 10);
          } /* FOR q, d */
          pure_cons.resize(dib_constrain.size(), number_of_nodes + 10);            

          
          // potencial --------------------------------------
          {
            // the potencial points used during the calculation of the
            // potencial are:
            // - the potencial points between first and second of the dibble
            // - the flow points from first-fm1 to second
            //   here fm1 is calculated as follow:
            //     fm1_vec.reset();
            //     fm1_vec[w]=1;
            //     fm1=dim.position(fm1_vec);

            // in this test we use pot_glob as the reference poly_array,
            // but the limits of the other array should be the same
            
            index_t reference_node = -1;
            index_t pure_dibbles   =  0;
            index_t pures_per_node[number_of_nodes];
            FOR(q, number_of_nodes)
            {
              pures_per_node[q]=0;
            }
            
            FOR(q, dib_potencial.size())
            {
              bool pot_is_in_good_node = false;
              bool flow_is_in_good_node = true; // here we use the optimistic approach
              
              { // first we check the potencial
                index_t pot_first  = dib_potencial[q].first;
                index_t pot_second = dib_potencial[q].second;
                reference_node     = pot_glob.node(pot_first);                
                
                if ( pot_glob.node(pot_second) == reference_node )
                {
                  pot_is_in_good_node = true;                    
                }
                
              } // check the potencial
              
              {// now we check the pixels of the flow
                
                index_t flow_first;
                index_t flow_second;
                vint fm1_vec(d);
                
                FOR(e, d)
                {
                  fm1_vec.reset();
                  fm1_vec[e]=1;//we are calculating the distance the opposite direction but it should be the same
                  index_t fm1 = dim.position(fm1_vec);
                  flow_first  = dib_potencial[q].first;
                  flow_second = dib_potencial[q].second - fm1;
                  
                  if (
                    (flow_glob[e].node(flow_first)  != reference_node)
                    or
                    (flow_glob[e].node(flow_second) != reference_node)
                    )
                  {                      
                    flow_is_in_good_node = false;                      
                  }
                } /* for e,d */
              } // check the pixels of the flow
              
              if ((pot_is_in_good_node) and (flow_is_in_good_node))
              {
                // std::cout << "dibble " << q << " is pure to " << nodes_of_this_thread << std::endl;                  
                pure_pot[q]=reference_node;
                pure_dibbles++;
                pures_per_node[reference_node]++;                
              }
              else /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
              {
              } /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
              
              
            } /* FOR q, dib_potencial->size */                            
            
            std::cout << "in the POTENCIAL there are " << pure_dibbles << " pure dibbles from " << dib_potencial.size()
                      << "; that makes " << 100 * pure_dibbles / dib_potencial.size() << "%" << std::endl;

            // std::cout << "they are divided in [";
            // FOR(q, number_of_nodes - 1 )
            // {
            //   std::cout << 100 * pures_per_nodes[q] / dib_potencial.size() << "%, ";            
            // }
            // std::cout << 100 * pures_per_nodes[ number_of_nodes - 1 ] / dib_potencial.size() << "%]" << std::endl;            
            
          } // potencial
      
          // flow -------------------------------------------
          { // note that here we still use pot_glob, but all the distributed arrays
            // should be the same size, so its ok.
            
            FOR(w, d)
            {
              index_t reference_node = -1;
              index_t pure_dibbles   =  0;
              index_t pures_per_node[number_of_nodes];            
              FOR(q, number_of_nodes)
              {
                pures_per_node[q]=0;              
              }

              FOR(q, dib_flow[w].size())
              {

                vint pp1_vec(d);
                index_t pp1_pos;              
                bool pot_is_in_good_node  = true; // here we use the optimistic approach
                bool flow_is_in_good_node = true; // here we use the optimistic approach
                reference_node = pot_glob.node(dib_flow[w][q].first);
                
                index_t flow_first  = dib_flow[w][q].first;
                index_t flow_second = dib_flow[w][q].second;
                
                pp1_vec.reset();
                pp1_vec[w] = 1;
                pp1_pos    = dim.position(pp1_vec);
                
                if (
                  (pot_glob.node(flow_first)  != reference_node )
                  or
                  (pot_glob.node(flow_second) != reference_node )
                  )
                {                  
                  flow_is_in_good_node = false;
                }
                
                if (
                  ( pot_glob.node( flow_first + pp1_pos )  != reference_node )
                  or
                  ( pot_glob.node( flow_second + pp1_pos ) != reference_node )
                  )
                {
                  pot_is_in_good_node = false;                  
                }
                
                if ((pot_is_in_good_node) and (flow_is_in_good_node))
                {
                  // std::cout << "dibble " << q << " is pure to " << nodes_of_this_thread << std::endl;                  
                  pure_flow[w][q] = reference_node;
                  pure_dibbles++;
                  pures_per_node[reference_node]++;                
                }
                else /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
                {
                  // std::cout << "dibble " << q << " is NOT pure" << std::endl;                  
                } /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */

                
              } /* FOR(q, dib_flow->size()) */

              std::cout << "in the FLOW there are " << pure_dibbles << " pure dibbles from " << dib_flow[w].size()
                        << "; that makes " << 100 * pure_dibbles / dib_flow[w].size() << "%" << std::endl;
              
              // std::cout << "they are divided in [";
              // FOR(q, number_of_nodes - 1 )
              // {
              //   std::cout << 100 * pures_per_nodes[q] / dib_flow[w].size() << "%, ";
              // }
              // std::cout << 100 * pures_per_nodes[ number_of_nodes - 1 ] / dib_flow[w].size() << "%]" << std::endl;            
              
            } /* FOR(w, d) */           
           
          } // flow
          
          // constrain --------------------------------------
          {
            index_t reference_node = -1;
            index_t pure_dibbles   =  0;
            index_t pures_per_node[number_of_nodes];            
            FOR(q, number_of_nodes)
            {
              pures_per_node[q]=0;              
            }

            vint fm1s(d), fm1_vec(d);                          
            FOR( w, d )
            {
              fm1_vec.reset();
              fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
              fm1s[w]=dim.position(fm1_vec);
            }

            FOR(q, dib_constrain.size())
            {
              bool the_dibble_is_pure = true; // here we use the optimistic approach
              
              index_t first, second;
              first  = dib_constrain[q].first;
              second = dib_constrain[q].second;
              reference_node = pot_glob.node(first);

              if (reference_node != pot_glob.node(second))
              {
                the_dibble_is_pure = false;                
              }
              
              FOR(w, d)
              {
                if (
                  ( reference_node != pot_glob.node(first - fm1s[w]))
                  or
                  ( reference_node != pot_glob.node(second - fm1s[w]))
                  )
                {
                  the_dibble_is_pure = false;                  
                }
                
              } /* FOR w, d */
              
              if (the_dibble_is_pure)
              {
                pure_cons[q]=reference_node;
                pure_dibbles++;
                pures_per_node[reference_node]++;                
              } /* the_dibble_is_pure */
                            
            } /* FOR q dibContrain->size() */

            std::cout << "in the CONSTRAIN there are " << pure_dibbles << " pure dibbles from " << dib_constrain.size()
                      << "; that makes " << 100 * pure_dibbles / dib_constrain.size() << "%" << std::endl;
            
            // std::cout << "they are divided in [";
            // FOR(q, number_of_nodes - 1 )
            // {
            //   std::cout << 100 * pures_per_nodes[q] / dib_constrain.size() << "%, ";
            // }
            // std::cout << 100 * pures_per_nodes[ number_of_nodes - 1 ] / dib_constrain.size() << "%]" << std::endl;

          } // constrain
          
        } /* uiTaintedDibbles */


      /**
         \brief This method distributes the dibbles between the threads.
         
      description The pure dibbles are collected by 'uiTaintedDibbles' 

      \param 
      \return 
      */      
      void distribute_dibbles()
        {

          std::cout << "compacting the source and the sink" << std::endl;

          // note here:
          //   - 0 will be the regular cells (neither src nor sink)
          //   - 1 will be the src
          //   - 2 will be the sink
          compact_srcsink = collector( src_sink, src_sink.get_size().prod(), 2/*number_of_nodes 2, the src and the sink*/);

          synchro_src  = partition( compact_srcsink[1], number_of_threads );
          synchro_sink = partition( compact_srcsink[2], number_of_threads );
          
          std::cout << "compacting the potencial" << std::endl;          

          compact_pot = pure_sort( pure_pot, dib_potencial, number_of_nodes );
          
          // compact_pot = pink::numa::collector(
          //   pure_pot,
          //   pure_pot.size(),
          //   number_of_nodes
          //   );

          // Breaking up the dibbles to packets.
          // These packets will then be distributed dynamically.
          resolut_pot.resize(number_of_nodes);

          FOR(q, number_of_nodes)            
          {
            resolut_pot[q] = partition( compact_pot[q], resolution );
          } /* FOR q, number_of_nodes */

          synchro_pot = partition( compact_pot[number_of_nodes], number_of_threads );
                    
          std::cout << "compacting the flow" << std::endl;

          resolut_flow.resize(d);          
          FOR(q, d)
          {
            compact_flow[q] =
              pure_sort( pure_flow[q], dib_flow[q], number_of_nodes );
            
            synchro_flow[q] = partition( compact_flow[q][number_of_nodes], number_of_threads );

            // Breaking up the dibbles to packets.
            // These packets will then be distributed dynamically.
            resolut_flow[q].resize(number_of_nodes);          
            FOR(w, number_of_nodes)            
            {
              resolut_flow[q][w]=partition( compact_flow[q][w], resolution );            
            } /* FOR w, number_of_nodes */
            
          } /* FOR q in d */

          std::cout << "compacting the constrain" << std::endl;

          compact_cons =
            pure_sort( pure_cons, dib_constrain, number_of_nodes );          

          // Breaking up the dibbles to packets.
          // These packets will then be distributed dynamically.
          resolut_cons.resize(number_of_nodes);
          
          FOR(q, number_of_nodes)            
          {
            resolut_cons[q] = partition( compact_cons[q], resolution );
          } /* FOR q, number_of_nodes */

          
          synchro_cons = partition( compact_cons[number_of_nodes], number_of_threads );

          std::cout << "compacted everything" << std::endl;
        } /* distflow::uiDistributeDibbles */
      

      

    public:

      /**
         \brief returns the calculated flow in raw format
         
         description the length of the array is dimension * pixels, and
         the vectors are grouped by direction

         \param 
         \return 
      */      
      boost::shared_array<pixel_type> get_flow() 
        {
          if (not flow_calculated){
            pink_error("The flow has not yet been calculated. You can only call get_flow after at least 1 iteration."
                       " For the courious souls after the 0th iteration the flow is zero everywhere");
          }
          else /* (flow_calculated) */
          {
            return flow_glob;
          }/* (flow_calculated) */
          return flow_glob;
        }
      
        
      /**
       * The iterator function. Copyes everything to local variables, so the compiler can optimize, then manages the iterations, mesures the time generates messages and saves intermediat results.
       * @param D input measure picture
       * @param S image representing source and sink (source = 1; sink = -1)
       * @param g the boundari measure field
       * @param iteration number of all iterations demanded. The number of the iterations can be estimated from the parameters. It needn't to be tested dinamically.
       * @param tau the iterating constant
       * @param uiMovie whether we want a movie or not
       * @return returns the final (hopefully yet convergent image)
       */
      distributed_flow(
        const char_image & SS,     /* image of the source and of the sink (not the original image) */
        const image_type & gg,     /* Boundaries */
        index_t iteration,         /* number of iterations */
        float   tau,		   /* timestep */
        index_t number_of_threads = 0, /* the number of threads to execute if in parallel mode */
        index_t resolution = 1     /* the size of the packet to process */
        ) :
        tau(tau),
        d( gg.get_size().size() ),
        dim( gg.get_size() ),
        iteration(iteration),
        flow_calculated(false),
        number_of_threads(number_of_threads),
        resolution(resolution),
        packet_size(packet_size),
        length_glob( gg.get_size().prod() ),
        //g_glob( number_of_threads, gg.get_size().prod(), NUMA ),
        //pot_glob( number_of_threads, gg.get_size().prod(), NUMA ),
        //flow_glob( gg.get_size().size(), poly_array_t(number_of_threads, gg.get_size().prod(), NUMA) ), // number_of_threads, d*length_glob, NUMA )
        g_glob( gg.get_size().prod() ),
        pot_glob( gg.get_size().prod() ),
        flow_glob( gg.get_size().size() ), // number_of_threads, d*length_glob, NUMA )

        dib_potencial(),
        dib_constrain(),
        dib_flow( gg.get_size().size() + 3 ), // we are adding here 3 because of the parallelization later
        //pure_flow( gg.get_size().size() ), // the size is d
        semaphores(),
        potencial(gg.get_size()), // we will put the result in this image
        // compact_pot(0),
        compact_flow(gg.get_size().size()), // the proper size
        // compact_cons(0),
        // compact_srcsink(0),
        // synchro_src(0),
        // synchro_sink(0),
        // synchro_pot(0),
        synchro_flow(gg.get_size().size()) //,
        // synchro_cons(0)
        {
#         ifdef UJIMAGE_DEBUG
          std::cout << "creating the distflow object (" << static_cast<void*>(this) << ")" << std::endl;	
#         endif /* UJIMAGE_DEBUG */        

          // these lines are no longer valid!
          // potencial.copy(gg); // "potencial";
          // potencial.fill(0.);

          // creating a local copy of image, srcsink potencial and flows ---------------------------
          std::cout << "dimension = " << d << "D" << std::endl;
          std::cout << "length_glob=" << this->length_glob << std::endl;

          // nodes
          this->number_of_nodes = pot_glob.number_of_nodes;
          this->resolution = number_of_threads/number_of_nodes;
          _DEBUG(this->resolution);
          _DEBUG(this->number_of_nodes);
          
          
          if (number_of_threads<=0)
          {
            pink_error("Wrong number of threads!");
          }
          else if (number_of_threads==0)
          {
            // getting the number of cpu-s on the system
            this->number_of_threads = boost::thread::hardware_concurrency();
            this->number_of_threads = std::max<index_t>( 1, this->number_of_threads );            
          } /* number_of_threads == 0 */

          std::cout << "Using " << this->number_of_threads << " threads from " << boost::thread::hardware_concurrency() << std::endl;
          
          // Now we copy the pointers to global variables, so the threads can see them.
          // boost::shared_ptr is boost's 'shared_array' smart pointer.
          this->gg.copy(gg);

          std::cout << "initializing the distributed array" << std::endl;

          FOR(q, d)
          {
            poly_array_t   flow_line( gg.get_size().prod() );            
            flow_glob[q] = flow_line;            
          }
          
          
          // FLOW IS ALREADY GOOD
          // setting up the flow
          // FOR(q, d)
          // {
          //   flow_glob.reset( q, new poly_array_t( number_of_threads, length_glob, NUMA ));        
          // } /* for d */ 

          //cleaning the flow
          //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
          FOR(q, d)
          {            
            FOR(w, length_glob)
            {
              flow_glob[q][w]=0.;
            } /* end of parallel FOR */
          } /* for d */

          FOR(q, length_glob)
          {
            pot_glob[q]=0.;            
          }
          
          
          
          //!!! std::cout << "debug: copying the source" << std::endl;
          // making 
          ///!!!! src_sink.reset(new char_image(SS, "distflow::src_sink"));
          src_sink.copy(SS);

          std::cout << "tau=" << this->tau << std::endl;
          std::cout << "iteration=" << this->iteration << std::endl;
	
          //// --------------------- breaking the fields into dibbles ----------------------------
	
          std::cout << "breaking up the field into dibbles" << std::endl;
        
          create_dibbles();

          std::cout << "looking for the tainted dibbles" << std::endl;

          tainted_dibbles();

          std::cout << "distributing the dibbles between threads" << std::endl;

          distribute_dibbles();          

          std::cout << "copying g_glob" << std::endl;
          FOR(q, gg.get_size().prod())
          {
            g_glob[q]=gg(q);            
          }          
          
          std::cout << "setting up source" << std::endl;
    
          // FOR(q, gg.get_size().prod())
          // {
          //   if ( src_sink(q) == 1 ) 
          //   {
          //     pot_glob[q]=1.; 
          //   } /* ( *ss == 1. ) */
          //   else /* NOT ( *ss == 1. ) */
          //   {
          //     if ( src_sink(q)==255 )
          //     { 
          //       pot_glob[q]=0.;	
          //     } /* if ( *ss==-1. ) */
          //   } /* NOT ( *ss == 1. ) */
          // } /* FOR q */
          
          
          //!!! std::cout << "debug: initialization of the distflow object has finished" << std::endl;

          
        } /* constructor distflow */
      

      virtual image_type start()
        {
          //// --------------------- initializing the time measure -------------------------------
          sentinel.maxPos(iteration);
          sentinel.minPos(0);
          sentinel << 0;

          // Posix threading
          typedef typename boost::shared_ptr<boost::thread> pthread_t;
          
          boost::shared_array<pthread_t>
            threads( new pthread_t[number_of_threads] );


          // initializing the mutexes and setting them to locked/charged state          
          semaphores.iteration_begin->lock();
          semaphores.potencial_begin->lock();
          semaphores.flow_begin     ->lock();
          semaphores.const_begin    ->lock();
          semaphores.srcsink_begin  ->lock();
          semaphores.loop_begin     ->lock();
          
          
          charge_mutex( semaphores.potencial_end,         number_of_threads );
          charge_mutex( semaphores.flow_end,              number_of_threads );
          charge_mutex( semaphores.const_end,             number_of_threads );
          charge_mutex( semaphores.srcsink_end,           number_of_threads );
          charge_mutex( semaphores.loop_end,              number_of_threads );
          charge_mutex( semaphores.loop_restart_lockable, number_of_threads );
          
          (*semaphores.keep_up)=true;

          //// --------- initializing the dealers ----------
          dealers.reset(
            new mdealers_t(resolut_pot, resolut_flow, resolut_cons, number_of_nodes, d)
            );

          // this distributor will distribute the threads between the
          // nodes. Note: the order of parameters is contraintuitive.
          distributor_t node_distributor( number_of_threads, number_of_nodes );
                    
          // Creating the threads
          FOR( w, number_of_threads )
          {
            threads[w].reset( new boost::thread(
                                maxflow_iterator<image_type>,
                                *this,
                                w,
                                node_distributor.node(w) // this is the node belonging to the segment
                                )
              );            
          } /* FOR(q, number_of_threads) */
                    
          // restricting the threads to appropriate nodes is done in
          // the thredder (distflow_iterator) function


          
          sentinel.start();
          std::cout << "starting the iteration" << std::endl;
          
          semaphores.iteration_begin->unlock();          
          
          //typedef poly_array< typename image_type::pixel_type > array_t;

          typedef BOOST_TYPEOF(pot_glob) array_t;

          // FOR(e, iteration)
          // {

          //   if ( e % REPORT_INTERVAL == 0 ) 
          //   {
          //     if ( sentinel.timeToReport() )
          //     {
          //       std::cout << "Estimated time remaining: " << (sentinel << e) << std::endl;
          //     } /* timeToReport() */
          //   } /* if iterations ... */

            
          //   FOR(ID, number_of_threads)
          //   {
              
          //     update_potencial( compact_pot[ID], fast_iterator<array_t> ); /// here I calculate my part of the potencial
          //     update_potencial( synchro_pot[ID], slow_iterator<array_t> ); // updating the common part
            
          //     // here I calculate my part of the flow
          //     FOR(w, d)
          //     {
          //       update_flow( compact_flow[w][ID], fast_iterator<array_t>, w );
          //       update_flow( synchro_flow[w][ID], slow_iterator<array_t>, w );
          //     }
            
          //     // here I calculate my part of the constrain
          //     update_constrain(compact_cons[ID], fast_iterator<array_t> );
          //     // updating the common part
          //     update_constrain(synchro_cons[ID], slow_iterator<array_t> );
            
          //     // in this instant the srcsink is updated in the threads (right here)
          //     update_srcsink( synchro_src [ID], 1. );
          //     update_srcsink( synchro_sink[ID], 0. );            

          //   }
            
          // }
          
          
          FOR( e, iteration )
          {
            if ( e % REPORT_INTERVAL == 0 ) 
            {
              if ( sentinel.timeToReport() )
              {
                std::cout << "Estimated time remaining: " << (sentinel << e) << std::endl;
              } /* timeToReport() */
            } /* if iterations ... */

            // checkpoint(semaphores.loop_restart_lockable);
            // semaphores.loop_restart->lock();            
            // semaphores.loop_begin->unlock();
            // checkpoint(semaphores.loop_end);
            // charge_mutex(semaphores.loop_restart_lockable, number_of_threads);
            // dealers->reset();
            // semaphores.loop_begin->lock();
            // charge_mutex(semaphores.loop_end, number_of_threads);
            // semaphores.loop_restart->unlock();

            // ****************************************************
            // **************  potencial update  ******************
            // ****************************************************

            semaphores.potencial_begin->unlock(); // lets the threads begin the potencial calculation
            
            checkpoint(semaphores.potencial_end); // waits until all the threads finish the potencial calculation
            charge_mutex(semaphores.potencial_end, number_of_threads); // charges the mutex for the next iteration             
            semaphores.potencial_begin->lock(); // prepares the mutex for the next iteration
            
            // ****************************************************
            // ****************  flow update  *********************
            // ****************************************************

            semaphores.flow_begin->unlock();
            
            checkpoint(semaphores.flow_end);
            charge_mutex(semaphores.flow_end, number_of_threads);
            semaphores.flow_begin->lock();
            
            // ****************************************************
            // **************  constrain update  ******************
            // ****************************************************
            
            semaphores.const_begin->unlock();
            
            checkpoint(semaphores.const_end);
            charge_mutex(semaphores.const_end, number_of_threads);
            semaphores.const_begin->lock();                       
            
            // // ****************************************************
            // // ******************  final test  ********************
            // // ****************************************************
            
            // // testing if the iteration will be the last.
            // if (e>=iteration-1)
            // {
            //   *(semaphores.keep_up)=false;              
            // }
            
            // ****************************************************
            // ****************  srcsink update  ******************
            // ****************************************************
            
            semaphores.srcsink_begin->unlock();
            
            checkpoint(semaphores.srcsink_end);
            
            charge_mutex(semaphores.srcsink_end, number_of_threads);
            semaphores.srcsink_begin->lock();

            // dealers->reset();

            
            

          } /* FOR q, iteration */

          std::cout << "the iteration has finished. joining the threads" << std::endl;          
          // waiting for the threads to shut down
          FOR( w, number_of_threads )
          {
            threads[w]->join();
          } /* FOR(w, number_of_threads) */

          //// --------------------- printing out the measured time ------------------------------
          sentinel.stop();
          std::cout << "total time of iteration: " << sentinel.elapsedTime() << std::endl;

          vint time_cheat(src_sink.get_size().size(), 0);
          time_cheat[0]=sentinel.elapsedSeconds();
          std::cout << "setting time_cheat to " << time_cheat.repr() << std::endl;    
          // !!!!!! potencial.set_center_vint(time_cheat);    
    
          this->flow_calculated = true; 

          /// !!!!!!!!!!!!! return potencial; /* measure field picture */
          //local variables are deleted automaticly

          // copy the calculated potencial to the 'potencial' image
          FOR(q, dim.prod())
          {
            potencial(q)=pot_glob[q];            
          } /* for q */
          
          potencial.set_center_vint(time_cheat);
          
          return potencial;          
        } /* virtual start*/
      

      virtual ~distributed_flow()
        {      
#         ifdef UJIMAGE_DEBUG      
          std::cout << "destroying the distflow object (" << static_cast<void*>(this) << ")" << std::endl;
#         endif /* UJIMAGE_DEBUG */        
        }
    }; /* class distributed_flow */


    /**
       \brief This is the thredding function for the distflow
       iteration. It takes the distflow object, and performs the
       necessary calculations, while it keeps itself synchronized with
       the main thread.
       
    description This function uses several semaphores to synchronize
    the calculation limits with the main thread. For now, the dibbles
    on which it's suppossed to iterate are static, that is to say, it
    is divided approximately evenly between the threads before the
    beginning of the iteration. After the final iteration, the thread
    quits.
    */
    template <class image_type>
    void
    maxflow_iterator
    (
      pink::numa::distributed_flow<image_type> & obj,
      index_t ID,
      index_t node_of_this_thread
      )
    {
      //typedef poly_array< typename image_type::pixel_type > array_t;
      typedef typename pink::numa::distributed_flow<image_type>::poly_array_t array_t;
      typedef typename pink::numa::distributed_flow<image_type>::packet_t packet_t;

      bool remains;
      packet_t packet;
      index_t resolution = obj.resolution;
      
      
      
      //!!! std::cout << "thread " << ID << " started" << std::endl;
      
      // *********************************************************
      // *********************************************************
      // ************ Initializing the NUMA part *****************
      // *********************************************************
      // *********************************************************

      //!!! std::cout << "binding the thread " << ID << " to the node " << node_of_this_thread << std::endl;     
      
      // restricting myself for the node, where the segment was
      // allocated

      // // for numa 2.0
      // struct bitmask * mask = numa_bitmask_alloc(numa_num_possible_nodes());
      // numa_bitmask_setbit(mask, node_of_this_thread);
      // numa_bind(mask);
      // numa_bitmask_free(mask);

      // for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node_of_this_thread);
      numa_bind(&mask);

      // // restricting myself to a particular core and nothing else
      // cpu_set_t cpu_set;      
      
      // size_t size;
      // int num_cpus, cpu;
      // num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

      // CPU_ZERO(&cpu_set);
      // CPU_SET(ID, &cpu_set);

      // sched_setaffinity(getpid(), sizeof(cpu_set), &cpu_set);      
      // //CPU_FREE(cpu_set);
      
      // *********************************************************
      // *********************************************************
      // *************** Starting the iterations *****************
      // *********************************************************
      // *********************************************************

      //!!! std::cout << "starting the iteration in thread " << ID << std::endl;

      // waiting for the permission to start the iteration
      obj.semaphores.iteration_begin->lock_shared();
      
      FOR(q, obj.iteration)
        //while (*obj.semaphores.keep_up)
      {
        //obj.semaphores.loop_restart_lockable->unlock_shared();        
        //shared_checkpoint(obj.semaphores.loop_begin);
        
        shared_checkpoint( obj.semaphores.potencial_begin); // waiting for the permission to begin the potencial
                
        // beginning the distributed calculation
        // packet  = obj.dealers->potencial[node_of_this_thread]();
        // remains = packet.second;
        
        // while (remains) 
        // {
        //   obj.update_potencial( packet.first, fast_iterator<array_t> );
        //   packet  = obj.dealers->potencial[node_of_this_thread]();
        //   remains = packet.second;
        // } /* while remains */

        //obj.update_potencial( obj.compact_pot[ID], fast_iterator<array_t> ); /// here I calculate my part of the potencial
        obj.update_potencial( obj.resolut_pot[node_of_this_thread][ ID % resolution ], fast_iterator<array_t> ); /// here I calculate my part of the potencial
        obj.update_potencial( obj.synchro_pot[ID], slow_iterator<array_t> ); // updating the common part
        
        obj.semaphores.potencial_end->unlock_shared(); // I'm reporting that I have finished with my part of the potencial
        shared_checkpoint(obj.semaphores.flow_begin); // waiting for the permission to begin the flow

        // here I calculate my part of the flow
        FOR(w, obj.d)
        {
          // beginning the distributed calculation
          // packet  = obj.dealers->flow[w][node_of_this_thread]();
          // remains = packet.second;          
          
          // while (remains) 
          // {
          //   obj.update_flow( packet.first, fast_iterator<array_t>, w );
          //   packet  = obj.dealers->flow[w][node_of_this_thread]();
          //   remains = packet.second;
          // } /* while remains */
        
          //obj.update_flow( obj.compact_flow[w][ID], fast_iterator<array_t>, w );
          obj.update_flow( obj.resolut_flow[w][node_of_this_thread][ ID % resolution ], fast_iterator<array_t>, w );
          obj.update_flow( obj.synchro_flow[w][ID], slow_iterator<array_t>, w );
        }

        obj.semaphores.flow_end->unlock_shared(); // I'm reporting that I have finished with my part of the flow
        shared_checkpoint(obj.semaphores.const_begin); // waiting for the permission to begin the constrain

        // // here I calculate my part of the constrain
        // // beginning the distributed calculation
        // packet  = obj.dealers->constrain[node_of_this_thread]();
        // remains = packet.second;          
        
        // while (remains) 
        // {
        //   obj.update_constrain( packet.first, fast_iterator<array_t> );
        //   packet  = obj.dealers->constrain[node_of_this_thread]();
        //   remains = packet.second;
        // } /* while remains */

        //obj.update_constrain(obj.compact_cons[ID], fast_iterator<array_t> );
        obj.update_constrain(obj.resolut_cons[node_of_this_thread][ ID % resolution ], fast_iterator<array_t> );
        // updating the common part
        obj.update_constrain(obj.synchro_cons[ID], slow_iterator<array_t> );
        
        obj.semaphores.const_end->unlock_shared(); // I'm reporting that I have finished with my part of the constrain        
        shared_checkpoint(obj.semaphores.srcsink_begin); // waiting for the permission to update the flow with the source and the sink

        // in this instant the srcsink is updated in the threads (right here)
        obj.update_srcsink( obj.synchro_src [ID], 1. );
        obj.update_srcsink( obj.synchro_sink[ID], 0. );

        // obj.semaphores.loop_end->unlock_shared();        
        // shared_checkpoint(obj.semaphores.loop_restart);        

        obj.semaphores.srcsink_end->unlock_shared(); // I'm reporting that I have finished with my part of updating the source and the sink
      } /* For q, iterations */      

    } /* maxflow_iterator */




    template <class image_type>
    image_type
    distflow( 
      char_image SS,  /* image of the source and of the sink (not the original image) */
      image_type gg, /* Boundaries */
      index_t    iteration,         /* number of iterations */
      float      glob_tau,	     /* timestep */
      index_t    number_of_threads, /* the number of threads to execute if in parallel mode */
      index_t    resolution = 1     /* the resolution of the iteration */
      )
    {
      distributed_flow<image_type> obj(
        frame_around(SS, -1),
        frame_around(gg, 0.),
        iteration,
        glob_tau,
        number_of_threads,
        resolution
        );

      image_type result = frame_remove(obj.start());


      return result;
    } /* distflow */
    
  } /* namespace numa */   
} /* end namespace pink */


// cleaning up after us
#undef REPORT_INTERVAL

// // obsolete, replaced by enum
// #undef __ETAP_POT
// #undef __ETAP_FLOW
// #undef __ETAP_CONSTR

// // obsolete, loaded as a parameter
// #undef PACKET_SIZE


#undef ATOMIC


#endif /* UIFLOW_DISTRIBUTED_HPP_*/
/* LuM end of file */
