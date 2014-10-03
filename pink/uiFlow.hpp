/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_HPP_
#define UIFLOW_HPP_

#include <vector>
#include <algorithm>
#include <boost/thread.hpp>

#include "uiFrame.hpp"
#include "pynumpy.hpp"
#include "ui_systest.hpp"

namespace pink { 

#define REPORT_INTERVAL 10
  
  namespace maxflow_types
  {
    
    enum etap { pot, flow, constr, unknown };
    
  } /* namespace maxflow_types */

  const index_t MaxDibble = 1000;  
  typedef std::pair<index_t, index_t> dibble_t;  
  
// // obsolete, replaced by enum  
// #define __ETAP_POT 1717
// #define __ETAP_FLOW 1232
// #define __ETAP_CONSTR 9120
  
#  define ATOMIC( code )                                                \
  parent->m_global_lock->lock();					\
  code                                                                  \
  parent->m_global_lock->unlock();

  
  
  template <class image_type>
  class max_flow;

  

  template <class image_type>
  class packet
  {

  public:

    typedef max_flow<image_type> parent_type;
    void operator()( index_t ID, parent_type * parent, boost::shared_ptr<boost::barrier> barrier );    
    packet();
    ~packet();
    
  private:
    
    friend class max_flow<image_type>;    
    
    parent_type * m_parent;
    
    index_t m_ID, m_start_dibble, m_end_dibble, m_direction, m_current_iteration;
    maxflow_types::etap m_etap; // this is the current state of iteration
    
    
  }; /* class packet */


  

  template <class image_type>
  class max_flow {

  private:
    
    // variables for threading
    boost::shared_ptr<boost::mutex> m_global_lock;
    boost::shared_ptr<boost::shared_mutex> m_shared_lock;

    friend class packet<image_type>;

    typedef typename image_type::pixel_type pixel_type;

    // the result will be put in this image
    image_type m_gg;
    char_image m_src_sink;

//    boost::shared_array<pixel_type> pot_glob;
    pixel_type * m_g_glob;
    boost::shared_array<pixel_type> m_flow_glob;

    index_t m_starttime;
    index_t m_number_of_threads;
    bool m_flow_calculated;  // this variable hold if start function has already been called, 
                           // if it hasn't than pot_glob and flow_flob are not yet allocated
   
    // functions for threading    
    boost::shared_ptr<packet<image_type> > m_reference;
    bool conductor( packet<image_type> & thread );
    index_t m_packet_size; // this variable holds the number of consecutive
                     // dibbles to be processed by the same thread. If
                     // packet_size==n then each thread processes n
                     // dibbles for on one charge. n dibbles is called
                     // a packet.
    

    
  protected:

    index_t m_d;
    float   m_tau;
    bool    m_verbose;   // debug info messages
    index_t m_iteration; // the number of desired iterations
    index_t m_length_glob;
    pink::types::progress_bar m_sentinel;    
    boost::shared_ptr<std::vector<index_t>> m_dim;

    // functions for calculation 
    void upDateConstrain(index_t startDibble, index_t endDibble);
    void upDatePotencial(index_t startDibble, index_t endDibble);
    void upDateFlow(index_t startDibble, index_t endDibble, index_t w /*direction*/);
    void upDateSrcSink();
    index_t uiCreateDibbles( );

    std::vector<dibble_t> dibConstrain;
    std::vector<dibble_t> dibPotencial;

    image_type m_potencial;


    // this way when the smart pointer destroyes the array it will
    // destroy all the elements as well
    std::vector< std::vector<dibble_t> > m_dibFlow; 

  public:

    double m_time;    
    
    boost::shared_array<pixel_type> get_flow(); // returns the calculated flow in raw format (the length 
                               // of the array is dimension * pixels, and the vectors are grouped by direction)
    
     max_flow(
      const char_image & SS,  /* image of the source and of the sink (not the original image) */
      const image_type & gg, /* Boundaries */
      index_t iteration,     /* number of iterations */
      float   tau,		 /* timestep */
      index_t number_of_threads, /* the number of threads to execute if in parallel mode */
      index_t packet_size, /* the size of the packet to process */
      bool    verbose
      );

    virtual image_type start();

    virtual ~max_flow(){      
#     ifdef UJIMAGE_DEBUG      
      std::cout << "destroying the max_flow object (" << static_cast<void*>(this) << ")" << std::endl;
#     endif /* UJIMAGE_DEBUG */        
    }
  }; /* class max_flow */
  

  template<class image_type>
  packet<image_type>::packet( )
  {
    m_start_dibble = m_end_dibble = m_direction = m_current_iteration = 0;
    m_etap = maxflow_types::unknown;
    
  } /* packet::packet */
  
  template<class image_type>
  packet<image_type>::~packet()
  {
    // intentionally left empty
  } /* packet::~packet */
  
  template<class image_type>
  void packet<image_type>::operator()( 
    index_t ID, 
    max_flow<image_type> * parent,
    boost::shared_ptr<boost::barrier> barrier
    )
  {

    this->m_ID = ID;
    this->m_parent = parent;
    barrier->wait(); // signalling that we are ready to start the iteration
    barrier->wait(); // waiting to start the iteration
    bool _continue;
    
    ATOMIC(
      _continue = parent->conductor( *this );
      parent->m_shared_lock->lock_shared();
      );

    while ( _continue )
    {

      switch ( this->m_etap )
      {
      case maxflow_types::pot:
	parent -> upDatePotencial( m_start_dibble, m_end_dibble );
#       if UJIMAGE_DEBUG >= 3
	std::cerr << "pot " << m_start_dibble << " " << m_end_dibble << " this = " << this << "" << std::endl;
#       endif /* UJIMAGE_DEBUG >= 3 */
	break;
	
      case maxflow_types::flow:
	parent -> upDateFlow( m_start_dibble, m_end_dibble, m_direction );
#       if UJIMAGE_DEBUG >= 3
	std::cerr << "flow " << m_start_dibble << " " << m_end_dibble << " this = " << this << "" << std::endl;
#       endif /* UJIMAGE_DEBUG >= 3 */
	break;
	
      case maxflow_types::constr:
	parent -> upDateConstrain( m_start_dibble, m_end_dibble );
#       if UJIMAGE_DEBUG >= 3
	std::cerr << "constr " << m_start_dibble << " " << m_end_dibble << " this = " << this << "" << std::endl;
#       endif /* UJIMAGE_DEBUG >= 3 */
	break;
	
      default:
	  pink_error("Packet is not properly set up.");
      } /* switch */

      parent->m_shared_lock->unlock_shared();

      ATOMIC(
	_continue = parent->conductor( *this );
	parent->m_shared_lock->lock_shared();  // shared lock is used for the synchronization of the etaps
	);
    }

    parent->m_shared_lock->unlock_shared();
    barrier->wait(); // signalling the end of the the iteration
  } /* packet::operator() */


  
  template <class image_type>
  void max_flow<image_type>::upDatePotencial(index_t startDibble, index_t endDibble){
    pixel_type * p_c;
    pixel_type * f_out;
    pixel_type * f_in;
    index_t fm1 /*fm1_vec[d]*/, start, end, length, q, w, e;
    std::vector<index_t> fm1_vec(m_d);
    
    FORR(w, m_d){
      pink::reset(fm1_vec);      
      fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
      fm1=pink::position(*m_dim, fm1_vec);
      for (/*int*/ e=startDibble; e<=endDibble-1; e++){
	start = dibPotencial[e].first; // start
	end   = dibPotencial[e].second; // end
	
	p_c=&(m_potencial(start));
	f_out=&(m_flow_glob[w*m_length_glob+start]);
	f_in=&(m_flow_glob[w*m_length_glob+start-fm1]);
	length = end - start;
	//the hyper-super ultra fast loop
	FORR(q, length) {
	  p_c[q] -= m_tau * ( f_out[q]-f_in[q] );
	}
      }
    }
  } /* void upDatePotencial */


  
  template <class image_type>  
  void max_flow<image_type>::upDateFlow(index_t startDibble, index_t endDibble, index_t w /*direction*/){
    pixel_type *p, *pp1, *f;
    index_t start, end, length, pp1_pos, q, e;
    std::vector<index_t> pp1_vec(m_d);

    pink::reset(pp1_vec);    
    pp1_vec[w]=1;
    pp1_pos=pink::position(*m_dim, pp1_vec);

    for ( /*int*/ e=startDibble; e<=endDibble-1; e++ ){
      start=m_dibFlow[w][e].first;
      end=m_dibFlow[w][e].second;
      f=&(m_flow_glob[w*m_length_glob+start]);
      p=&(m_potencial(start));
      pp1=&(m_potencial(start+pp1_pos));
      length=end-start;
      FORR(q, length){
	f[q] -= m_tau*(pp1[q]-p[q]);
      }
    }
  } /* void upDateFlow */



  template <class image_type>
  void max_flow<image_type>::upDateConstrain(index_t startDibble, index_t endDibble){
//     //local copies
    pixel_type *  dFabs      = new pixel_type [m_d/*compileDim*/];
    pixel_type ** locInFlow  = new pixel_type*[m_d/*compileDim*/];
    pixel_type ** locOutFlow = new pixel_type*[m_d/*compileDim*/];
    pixel_type *  locG;
    pixel_type v;
	
    //locals
    std::vector<index_t> fm1s(m_d), fm1_vec(m_d);
    index_t start, end, length, q, w, e;
	
    // calculating differences between the flow and the point
    FORR( w, m_d ) {
      pink::reset(fm1_vec);
      fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
      fm1s[w]=pink::position(*m_dim, fm1_vec);
    }
	
    //for (/*int*/ e=0; e<=dibConstrain->length-1; e++){
    for (/*int*/ e=startDibble; e<=endDibble-1; e++){
      start=dibConstrain[e].first;
      end=dibConstrain[e].second;
      length=end-start;
      locG=&(m_g_glob[start]);
      FORR(w, m_d) {
	locOutFlow[w]=&(m_flow_glob[w*m_length_glob+start]);
	locInFlow[w]=&(m_flow_glob[w*m_length_glob+start-fm1s[w]]);
      }
      FORR(q, length) {
	v=0.;
	FORR(w, m_d) {
	  // dFabs=max(-Fin, 0, Fout)
	  dFabs[w] = -locInFlow[w][q] > locOutFlow[w][q] ? -locInFlow[w][q] : locOutFlow[w][q];
	  if (0. > dFabs[w]) dFabs[w] = 0.;
	  // v = sum(dFabs^2)
	  v += static_cast<pixel_type>(dFabs[w] * dFabs[w]);
	}

	FORR(w, m_d) {
	  // multiplying
	  if (locG[q]*locG[q] < v ) dFabs[w] *= locG[q]/static_cast<pixel_type>(sqrt(v));
	  // testing the final condition
	  if (locInFlow[w][q] < -dFabs[w]) locInFlow[w][q] = -dFabs[w];
	  if (locOutFlow[w][q] > dFabs[w]) locOutFlow[w][q] = dFabs[w];
	}
      }
    }
  } /* upDateConstrain */

  template <class image_type>
  void max_flow<image_type>::upDateSrcSink(void)
  // in this object this method is only called once, because the 
  // value of the source pixel does not change
  {

    FOR( q, m_length_glob ) 
    {
      if ( m_src_sink(q) == 1 ) 
      {
	m_potencial(q)=1.; 
      }
      else /* NOT  (*srcsink) == 1. */
      {
	if ( m_src_sink(q)==255 )
	{ 
	  m_potencial(q)=0.;	
	} /* if (*srcsink) == -1. */
      } /* NOT (*srcsink) == 1.  */
    } /* FOR */
  } /* maxmax_flow::upDateSrcSink */
  


  template <class image_type>
  index_t max_flow<image_type>::uiCreateDibbles( 
    )
  {
    // it is demanded that all the fragments on the border of the image are sink
    // for every dibble, the higher end is NOT included

// following we will parallelize the creation of the dibbles
// as there are three parts we only create 3 threads
    //#pragma omp parallel num_threads(3) 
    { 

      //#pragma omp sections
      {
        //!!! std::cout << "creating the dibbles from the potencial" << std::endl;
  
	//  -------- potencial -------
	// all the fragments different from source and sink are to be exclusively among the dibbles
        //#pragma omp section
	{
	  bool started = false;
	  index_t start=0, end=0;
	  index_t currlength=0;
	  uint8_t * curr;
	  curr = m_src_sink.get();
	  FOR(q, m_length_glob) {
	    if (started) {
	      currlength++;
	      if (curr[q]!=0) {
		end=q;
		dibPotencial.push_back(dibble_t(start, end));
		///!!! std::cout << "dibPotencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
		start=0;
		end=0;
		started=false;
	      } else /* NOT (curr[q]!=0) */
		if (currlength>=MaxDibble) {
		  end=q+1;
		  dibPotencial.push_back(dibble_t(start, end));
		  ///!!! std::cout << "maxdibble dibPotencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
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
	  } /* FOR(q, m_length_glob) */
	} /* end pragma omp section */
  
  
        //!!! std::cout << "creating the dibbles from the flow" << std::endl;
	//  -------- flow ------------
	// all the arrows with at least one end not in src or sink are to be exclusively among the dibbles
        //#pragma omp section
	{
	  bool started=false;
	  index_t start=0, end=0;
          std::vector<uint8_t*> pp1(m_d);
	  uint8_t * p;
	  std::vector<index_t> pp1_vec(m_d);
	  index_t currlength=0;
	  std::vector<index_t> pp1_pos(m_d);
	  // Calculating the shift of the neighbourh elements
	  FOR(w, m_d) {
            pink::reset(pp1_vec);            
	    pp1_vec[w]=1;
	    int currpos = pink::position( *m_dim, pp1_vec );
///!!!	std::cout << "currpos=" << currpos << "" << std::endl;
	    pp1_pos[w]=currpos;
	    pp1[w]=&(m_src_sink(currpos));
	  } /* FOR(w, d) */
	  p = m_src_sink.get();
	  FOR(w, m_d) {
	    FOR(q, m_length_glob - pp1_pos[w] /* - 1 */ ) { /////////
	      if (started){
		currlength++;
		if ((p[q]!=0) && (pp1[w][q]!=0)){
		  end=q;
		  m_dibFlow[w].push_back(dibble_t(start, end));
		  ///!!! std::cout << "dibFlow[" << w << "]->addElement(" << start << "," << end << ")\n";
		  start=0;
		  end=0;
		  started=false;
		} else /* NOT ((p[q]!=0) and (pp1[w][q]!=0)) */
		  if (currlength>=MaxDibble){
		    end=q+1;/////!!!!!!!!!!!!!!!
		    m_dibFlow[w].push_back(dibble_t(start, end));
		    start=0;
		    end=0;
		    started=false;
		  } /* (currlength>=MaxDibble) */
	      } else { /* NOT (started) */
		if ((p[q]==0) || (pp1[w][q]==0)){
		  started=true;
		  start=q;
		  currlength=1;
		}
	      } /* NOT (started) */
	    } /* FOR(q, m_length_glob-pp1_pos[w]-1) */
	  } /* FOR(w, m_d) */
	} /* pragma omp section */

        if (m_verbose)
          std::cout << "creating the dibbles from the constrain" << std::endl;
	//  -------- constrain -------
	// all the regular points and sources with at least one regular neighbour
        //#pragma omp section
	{
	  bool started=false;
	  index_t start=0, end=0;
          std::vector<uint8_t*> pp1(m_d), pm1(m_d);
	  uint8_t * p;
	  std::vector<index_t> pp1_vec(m_d);
	  index_t currlength=0;
	  index_t max=0;
	  FOR(w, m_d) {
            pink::reset(pp1_vec);            
	    pp1_vec[w]=1;
	    index_t currpos = pink::position( *m_dim, pp1_vec);
	    max = max > currpos ? max : currpos;
	    pp1[w]=&( m_src_sink(currpos) );
	    pm1[w]=&( m_src_sink(0) ) - currpos;// risky, we always read far enough from the beginning
	  } /* FOR(w, m_d) */
	  p = m_src_sink.get();
          for( index_t q = max; q <= m_length_glob - max - 1; q++ )
//	  FOR( q, m_length_glob - max /*-1*/ ){ ///////////
          {            
	    bool i_am_a_regular_point = (p[q]==0.);
            bool i_am_a_source = (p[q]==1.);
            bool there_is_a_regular_point_near = false;
            FOR(w, m_d) {
              if ((pp1[w][q]==0.) || (pm1[w][q]==0.)) there_is_a_regular_point_near=true;
	    }
	    bool i_want_to_be_in_a_dibble = (i_am_a_regular_point || (i_am_a_source && there_is_a_regular_point_near));
	    //bool i_want_to_be_in_a_dibble = (i_am_a_regular_point or i_am_a_source);
	    if (started){
	      currlength++;
	      bool too_long = (currlength>=MaxDibble);
	      if (! /*not*/ i_want_to_be_in_a_dibble){
		end=q;
		dibConstrain.push_back(dibble_t(start, end));
		///!!! std::cout << "dibConstrain->addElement(" << start << ", " << end << ")" << std::endl;
		start=0;
		end=0;
		started=false;
	      } else if (too_long){
		//printf("uiNotice: breaking because of the MaxDibble" << std::endl);
		end=q+1;///////!!!!!!
		dibConstrain.push_back(dibble_t(start, end));
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
	  } /* FOR ( q, m_length_glob-max-1 ) */
	} /* pragma omp section*/
      } /* pragma omp sections */
    } /* pragma omp parallel */
    return 0; 
  }/* int uiCreateDibbles */



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
  template <class image_type>
  max_flow<image_type>::max_flow
  (
    const char_image & SS, /* image of the source and of the sink (not the original image) */
    const image_type & gg, /* Boundaries */
    index_t iteration,     /* number of iterations */
    float   tau,               /* = 0.132,   ** timestep */
    index_t number_of_threads, /* = 0, ** the number of threads to execute if in parallel mode */
    index_t packet_size,       /* = 1000, */
    bool    verbose           /* = false */
    ) :
    m_packet_size(packet_size),
    m_verbose(verbose),
    m_d(gg.size().size()),
    m_length_glob(pink::prod(gg.size())),
    // setting up the lock
    m_global_lock( new boost::mutex ),
    m_shared_lock( new boost::shared_mutex ),
    m_tau(tau),
    m_iteration(iteration),
    m_flow_calculated(false),
    m_number_of_threads(number_of_threads)
  {
    std::cout << "max01" << std::endl;
    
#   ifdef UJIMAGE_DEBUG
    std::cout << "creating the max_flow object (" << static_cast<void*>(this) << ")" << std::endl;	
#   endif /* UJIMAGE_DEBUG */        

    m_potencial.reset(gg.size()); // "m_potencial";
    pink::fill(m_potencial, 0.);

    std::cout << "max02" << std::endl;
    
    if (( this->m_number_of_threads == 0 ) || (this->m_number_of_threads > boost::thread::hardware_concurrency() ))
    {
      this->m_number_of_threads = boost::thread::hardware_concurrency();            
    }

    std::cout << "max03" << std::endl;
    
    // Now we copy the pointers to global variables, so the threads can see them.
    // boost::shared_ptr is boost's 'shared_array' smart pointer.
    this->m_gg = gg.clone();
    m_g_glob = this->m_gg.get();    
    m_flow_glob.reset(new pixel_type[ m_d * m_length_glob ]);

    std::cout << "max04" << std::endl;
    
    //cleaning the flow
    FOR( q,  m_d * m_length_glob)
    {
      m_flow_glob[q]=0.;
    } /* end of parallel FOR */
    
    std::cout << "max05" << std::endl;
    
    // making 
    m_src_sink = SS.clone();
    //int dim [d];
    m_dim.reset(new std::vector<index_t>(m_potencial.size()));
    
    std::cout << "max06" << std::endl;
    
    if (verbose)
    {      
      std::cout << "dibble edition" << std::endl;
      // creating a local copy of image, srcsink m_potencial and flows ---------------------------
      std::cout << "dimension   = " << pink::repr(*m_dim) << " (" << m_d << "D)" << std::endl;
      std::cout << "length_glob = " << this->m_length_glob << std::endl;
      std::cout << "tau         = " << this->m_tau << std::endl;
      std::cout << "iteration   = " << this->m_iteration << std::endl;
      std::cout << "threads     = " << this->m_number_of_threads << " (of " << boost::thread::hardware_concurrency() << ")" <<  std::endl;
    } /* if verbose */

    std::cout << "max07" << std::endl;
    
    //// --------------------- setting up source -------------------------------------------

    if (verbose)
      std::cout << "setting up source" << std::endl;
    
    pixel_type *ps;
    uint8_t *ss;
    std::cout << "max08" << std::endl;
    		
    ss = &( m_src_sink(0) );
    ps = &( m_potencial(0));

    FOR(q, m_length_glob) {
      if ( ss[q] == 1 ) 
      {
	ps[q]=1.; 
      } /* ( *ss == 1. ) */
      else /* NOT ( *ss == 1. ) */
      {
	if ( ss[q]==255 )
	{ 
	  ps[q]=0.;	
	} /* if ( *ss==-1. ) */
      } /* NOT ( *ss == 1. ) */
    } /* end of parallel FOR */

    std::cout << "max09" << std::endl;
    
    //// --------------------- breaking the fields into dibbles ----------------------------

    if (verbose)
      std::cout << "breaking up the field into dibbles" << std::endl;
    
    //dibPotencial.reset( new uiDibbles() );
    //dibConstrain.reset( new uiDibbles() );
    m_dibFlow.resize(m_d+3); //reset( new boost::shared_ptr<uiDibbles>[d+3] ); // we adding here 3 becaus of the parallelization later
    //FOR(q,d+3) m_dibFlow[q].reset(new uiDibbles()); // we adding here 3 becaus of the parallelization later
	
    uiCreateDibbles();

    std::cout << "max10" << std::endl;
    
    if (verbose)
      std::cout << "the initialization is finished" << std::endl;
  } /*   max_flow<image_type>::max_flow */	


  
  /**
     \brief Gets the flow from the object.
     
     description The function returns the 'flow' object after the
     max_flow iteration. The flow is sometimes examined for the
     algorithm analysis.

  \return Returns the corresponding flow, or raises an error if the
  flow has not yet been calculated.
  */  
  template <class image_type>
  boost::shared_array<typename image_type::pixel_type>
  max_flow<image_type>::get_flow()
  {
    if ( /*not*/ ! m_flow_calculated)
    {
      pink_error("The flow has not yet been calculated. You can only call get_flow after at least 1 iteration."
	    " For the courious souls after the 0th iteration the flow is zero everywhere");
    }
    else /* (flow_calculated) */
    {
      return m_flow_glob;
    }/* (flow_calculated) */
    return m_flow_glob;
  } /* get_flow */



  /**
     \brief This method starts the max_flow calculation.
     
     description This method creates 'number_of_thread' threads and
     iterates over the flow 'iteration' times.

  \return It returns the 'potencial' after the given number of iterations.
  */  
  template <class image_type>
  image_type max_flow<image_type>::start()
  {
    //// --------------------- initializing the time measure -------------------------------
        
    if (m_verbose)
      std::cout << "starting the iteration" << std::endl;
    
    index_t nbt = this -> m_number_of_threads;
    boost::shared_ptr<boost::barrier> barrier(new boost::barrier(m_number_of_threads + 1) );    
    
    std::vector< boost::shared_ptr<boost::thread> > threads(nbt);

    std::vector< boost::shared_ptr< packet<image_type> > > packets(nbt);

    // Thread attributes
    m_reference.reset( new packet<image_type> );
    m_reference->m_etap = maxflow_types::pot;
    FOR( q, nbt )
    {
      packets[q].reset( new packet<image_type> );
      threads[q].reset( new boost::thread( (*packets[q]), q, this, barrier ) );
    } /* FOR(q, nbt) */

    barrier->wait(); // waiting the threads to initialize
    m_sentinel.maxPos(m_iteration);
    m_sentinel.minPos(0);
    m_sentinel << 0;
    m_sentinel.start(); // new time measurement
    double starttime = pink::benchmark::now();    
    barrier->wait(); // signalling the threads to begin the iteration


    barrier->wait(); // waiting the threads to finish the iteration    
    //// --------------------- printing out the measured time ------------------------------
    m_sentinel.stop();
    double endtime = pink::benchmark::now();
    this->m_time = static_cast<double>( endtime - starttime );
    
    FOR( q, nbt )
    {
      threads[q]->join();
    } /* FOR(q, nbt) */

    this->m_time = static_cast<double>(endtime - starttime);
    
    if (m_verbose)      
      std::cout << "total time of iteration (timer)   : " << this->m_time << "s" << std::endl;
    
    this->m_flow_calculated = true; 

    return m_potencial; /* measure field picture */
    //local variables are deleted automaticly
  } /*    max_flow<image_type>::start() */


  /**
     \brief The conductor synchronizes the calculation within the
     threads. 
     
  description The conductor is called after a thread has finished with
  the packet attributed to it. The conductor determines the next
  packet to attribute. If there are no more packet in a given etap,
  then the conductor waits for the threads to finish their remaining
  calculations. Afterwards it moves to the next etap, and begins to
  distribute the packet from the next etap.

  \param thread The thread which has finished it's calculation.
  
  \return Returns 'true', if there are still packets to work on. If the
  last packet from the last iteration had been distributed, then the
  conductor shuts down the threads returning 'false'.
  */
  template <class image_type>
  bool max_flow<image_type>::conductor( 
    packet<image_type> & thread 
    )
  {

    switch ( m_reference->m_etap )
    {
    case maxflow_types::pot:
      if (m_reference->m_end_dibble >= dibPotencial.size()) // the case, when the last iteration has been assigned and now we begin the next part
      {
	m_shared_lock->lock(); // we wait for the threads to finish the calculation
	m_reference->m_etap = maxflow_types::flow;
	m_reference->m_start_dibble = 0;
	m_reference->m_end_dibble = std::min<index_t>( this->m_packet_size, m_dibFlow[0].size() );
	m_reference->m_direction = 0;

	thread.m_start_dibble = m_reference->m_start_dibble;
	thread.m_end_dibble = m_reference->m_end_dibble;
	thread.m_etap = m_reference->m_etap;
	thread.m_direction = m_reference->m_direction;

	m_shared_lock->unlock(); // from now on all the threads are starting the flow iteration.
      } 
      else /* NOT m_reference->m_end_dibble >= dibPotencial->get_length() */
      {
	m_reference->m_start_dibble = m_reference->m_end_dibble;
	m_reference->m_end_dibble = std::min<index_t>( m_reference->m_end_dibble + this->m_packet_size, dibPotencial.size() );

	thread.m_etap = m_reference->m_etap;
	thread.m_start_dibble = m_reference->m_start_dibble;
	thread.m_end_dibble = m_reference->m_end_dibble;
      } /* NOT m_reference->m_end_dibble >= dibPotencial->get_length() */

      return true;
      break;
      
    case maxflow_types::flow:

      if (m_reference->m_end_dibble >= m_dibFlow[m_reference->m_direction].size()) // the case, when the last iteration has been assigned and now we begin the next part
      {

	if ( m_reference->m_direction >= m_d - 1 )
	{	  
	  m_shared_lock->lock(); // we wait for all threads to finish the calculation	  
	  m_reference->m_etap = maxflow_types::constr;
	  m_reference->m_start_dibble = 0;
	  m_reference->m_end_dibble = std::min<index_t>( this->m_packet_size, dibConstrain.size());
	  
	  thread.m_start_dibble = m_reference->m_start_dibble;
	  thread.m_end_dibble = m_reference->m_end_dibble;
	  thread.m_etap = m_reference->m_etap;	  
	  thread.m_direction = m_reference->m_direction;
	  m_shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.
	}
	else /* NOT m_reference->current_direction >= d - 1 */
	{
//	  shared_lock->lock(); // we wait for all threads to finish the calculation

	  m_reference->m_direction += 1;
	  m_reference->m_start_dibble = 0;
	  m_reference->m_end_dibble = std::min<index_t>( this->m_packet_size, 
					m_dibFlow[m_reference->m_direction].size());
	  
	  thread.m_start_dibble = m_reference->m_start_dibble;
	  thread.m_end_dibble = m_reference->m_end_dibble;
	  thread.m_direction = m_reference->m_direction;
	  thread.m_etap = m_reference->m_etap;

//	  shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.

	} /* NOT m_reference->current_direction >= d - 1 */
      } 
      else /* NOT m_reference->m_end_dibble >= m_dibFlow->get_length() */
      {
	m_reference->m_start_dibble = m_reference->m_end_dibble;
	m_reference->m_end_dibble = std::min<index_t>( m_reference->m_end_dibble + this->m_packet_size, m_dibFlow[m_reference->m_direction].size());

	thread.m_start_dibble = m_reference->m_start_dibble;
	thread.m_end_dibble = m_reference->m_end_dibble;
	thread.m_etap = m_reference->m_etap;
	thread.m_direction = m_reference->m_direction;
      } /* NOT m_reference->m_end_dibble >= m_dibFlow->get_length() */

      return true;
      break;
      
    case maxflow_types::constr:
      
      if (m_reference->m_end_dibble >= dibConstrain.size()) // the case, when the last iteration has been assigned and now we begin the next part
      {
	if ( m_reference->m_current_iteration >= m_iteration - 1 )
	{ // we are finished with the iterations.
	  // we wait for the rest of the threads
	  m_shared_lock->lock();	
	  
	  // we let the threads to finish
	  m_shared_lock->unlock();
	  return false;		  
	} 
	else /* NOT m_current_iteration >= iterations - 1 */	  
	{
          if (m_verbose)
          {            
            if ( m_reference->m_current_iteration % REPORT_INTERVAL == 0 ) 
            {
              if ( m_sentinel.timeToReport() )
              {
                std::cout << "Estimated time remaining: " << (m_sentinel << m_reference -> m_current_iteration) << std::endl;
              } /* timeToReport() */
            } /* if m_current_iterations ... */
          } /* if verbose */          
	  
	  m_shared_lock->lock(); // we wait for all threads to finish the calculation
	  m_reference->m_current_iteration++;

	  m_reference->m_etap = maxflow_types::pot;
	  m_reference->m_start_dibble = 0;
	  m_reference->m_end_dibble = std::min<index_t>(this->m_packet_size, dibPotencial.size());

	  thread.m_start_dibble = m_reference->m_start_dibble;
	  thread.m_end_dibble = m_reference->m_end_dibble;
	  thread.m_etap = m_reference->m_etap;

	  m_shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.
	  
	  return true;
	} /* NOT m_reference->m_current_iteration >= iterations - 1 */
      }
      else /* NOT m_reference->m_end_dibble 1 >= dibConstraint->get_length() */
      {
	m_reference->m_start_dibble = m_reference->m_end_dibble;
	m_reference->m_end_dibble = std::min<index_t>( m_reference->m_end_dibble + this->m_packet_size, dibConstrain.size());

	thread.m_start_dibble = m_reference->m_start_dibble;
	thread.m_end_dibble = m_reference->m_end_dibble;
	thread.m_etap = m_reference->m_etap;

	return true;
      } /* NOT m_reference->m_end_dibble >= dibConstraint->get_length() */


      break;
      
    default:
      pink_error("Packet error.");
    } /* switch */
      
   
  } /* max_flow::conductor */
  

  
  template<class image_type>
  image_type maxflow_cami(
    char_image SS,       /* image of the source and of the sink (not the original image) */
    image_type gg,     /* Boundaries */
    index_t    iteration,        /* number of iterations */
    float      glob_tau,	  /* timestep */
    index_t    number_of_threads /* the number of threads to execute if in parallel mode */
    )
  {

    pink_error("not compiled, uncomment the code.");
    
    //     boost::shared_ptr<max_flow<float_image> > maxflow_obj;
    
    //     maxflow_obj.reset( new max_flow<float_image>(SS, gg, iteration, glob_tau, number_of_threads ) );
    
    //     float_image result_image = maxflow_obj->start();
    
    
    //     // creating the n+1D flow_image
    
    //     vint dim(3,0);
    //     dim[0]=SS.size()[0];
    //     dim[1]=SS.size()[1];
    //     dim[2]=SS.size().size();
    
    //     float_image result_flow(maxflow_obj->get_flow(), "result_flow");
    
    // //    ARRAY<float> flow = maxflow_obj->get_flow();
    
    //      return result_flow;    
  }/* maxflow_cami */
  
  

  template <class image_type>
  boost::python::object
  maxflow( 
    char_image SS,         /* image of the source and of the sink (not the original image) */
    image_type gg,        /* Boundaries */
    index_t    iteration,         /* number of iterations */
    float      glob_tau,	   /* timestep */
    index_t    number_of_threads,  /* the number of threads to execute if in parallel mode */
    index_t    packet_size,
    bool       verbose = false,
    bool       debug = false    
    )
  {
    max_flow<image_type> obj(
      frame_around(SS, -1),
      frame_around(gg, 0.),
      iteration,
      glob_tau,
      number_of_threads,
      packet_size,
      true
      );

    image_type rimage = frame_remove(obj.start());

    if (debug)
    {        
      boost::python::tuple result = boost::python::make_tuple(rimage, obj.m_time);         
      return result;
    }
    else /* NOT debug */
    {
      boost::python::object result(rimage);
      return result;        
    } /* NOT debug */

  } /* maxflow_float */


  template <class image_type>
  boost::python::object
  pymaxflow( 
    boost::python::object /*char_image*/ pySS,         /* image of the source and of the sink (not the original image) */
    boost::python::object /*image_type*/ pygg,        /* Boundaries */
    index_t    iteration,         /* number of iterations */
    float      glob_tau,	   /* timestep */
    index_t    number_of_threads,  /* the number of threads to execute if in parallel mode */
    index_t    packet_size,
    bool       verbose = false,
    bool       debug = false    
    )
  {
    std::cout << "ici01" << std::endl;    
    char_image SS(pySS);    
    image_type gg(pygg);
    std::cout << "SS.size().size() = " << SS.size().size() << std::endl;
    std::cout << "gg.size().size() = " << gg.size().size() << std::endl;
    
    std::cout << "ici02" << std::endl;          
    max_flow<image_type> obj( frame_around(SS, -1), frame_around(gg, 0.), iteration, glob_tau, number_of_threads, packet_size, verbose );
    std::cout << "ici02a" << std::endl;          
    image_type rimage = frame_remove(obj.start());
    std::cout << "ici03" << std::endl;          

    if (debug)
    {
      std::cout << "ici04" << std::endl;          
          
      boost::python::tuple result = boost::python::make_tuple(rimage, obj.m_time);
      std::cout << "ici05" << std::endl;          
      return result;
    }
    else /* NOT debug */
    {
      std::cout << "ici06" << std::endl;          
      boost::python::object result(rimage);
      std::cout << "ici07" << std::endl;          
      return result;        
    } /* NOT debug */

  } /* maxflow_float */

  
} /* end namespace pink */


// cleaning up after us
#undef REPORT_INTERVAL

// // obsolete, replaced by enum
// #undef __ETAP_POT
// #undef __ETAP_FLOW
// #undef __ETAP_CONSTR

#undef ATOMIC


#endif /*UIFLOW_HPP_*/
/* LuM end of file */
