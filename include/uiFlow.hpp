/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_HPP_
#define UIFLOW_HPP_

#include <boost/thread.hpp>
#include "pyujimage.hpp"

#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in uiFlow.hpp: " << msg; call_error(fullmessage.str());}

namespace pink { 

#define REPORT_INTERVAL 10
  
#define PACKET_SIZE 1000
  
#define __ETAP_POT 1717
#define __ETAP_FLOW 1232
#define __ETAP_CONSTR 9120
  
#define ATOMIC( code )						\
  parent->global_lock->lock();					\
  code								\
  parent->global_lock->unlock();
  

  float_image maxflow_float( 
    char_image SS,  /* image of the source and of the sink (not the original image) */
    float_image gg, /* Boundaries */
    int iteration,     /* number of iterations */
    float glob_tau,		 /* timestep */
    int number_of_threads /* the number of threads to execute if in parallel mode */
    );
  
  
  float_image maxflow_cami(
    char_image SS,  /* image of the source and of the sink (not the original image) */
    float_image gg, /* Boundaries */
    int iteration,     /* number of iterations */
    float glob_tau,		 /* timestep */
    int number_of_threads /* the number of threads to execute if in parallel mode */
    );
  
  
  template <class image_type>
  class maxflow;
   

  template <class image_type>
  class packet
  {

  public:

    typedef maxflow<image_type> parent_type;
    void operator( )( int ID, parent_type * parent );    
    packet();
    ~packet();
    
  private:
    
    friend class maxflow<image_type>;    
    
    parent_type * parent;
    
    int ID, etap, start_dibble, end_dibble, direction, current_iteration;    
    
  }; /* packet */


  

  template <class image_type>
  class maxflow {

  private:

    // variables for threading
    boost::shared_ptr<boost::mutex> global_lock;
    boost::shared_ptr<boost::shared_mutex> shared_lock;

    friend class packet<image_type>;

    typedef typename image_type::pixel_type pixel_type;


    // the result will be put in this image
    image_type gg;
    char_image src_sink;

//    boost::shared_array<pixel_type> pot_glob;
    boost::shared_array<pixel_type> flow_glob;
    boost::shared_array<pixel_type> g_glob;


    int starttime;
    int number_of_threads;
    bool flow_calculated;  // this variable hold if start function has already been called, 
                           // if it hasn't than pot_glob and flow_flob are not yet allocated
   
    // functions for threading    
    boost::shared_ptr<packet<image_type> > reference;
    bool conductor( packet<image_type> & thread );
    
  protected:

    int d;
    int length_glob;
    boost::shared_ptr<vint> dim;

    progressBar sentinel;    
    float tau;
    // the number of desired iterations
    int iteration;

    // functions for calculation 
    void upDateConstrain(int startDibble, int endDibble);
    void upDatePotencial(int startDibble, int endDibble);
    void upDateFlow(int startDibble, int endDibble, int w /*direction*/);
    void upDateSrcSink();
    int uiCreateDibbles( );

    boost::shared_ptr<uiDibbles> dibConstrain;
    boost::shared_ptr<uiDibbles> dibPotencial;

    image_type potencial;


    // this way when the smart pointer destroyes the array it will
    // destroy all the elements as well
    boost::shared_array< boost::shared_ptr<uiDibbles> > dibFlow; 


  public:

    boost::shared_array<pixel_type> get_flow(); // returns the calculated flow in raw format (the length 
                               // of the array is dimension * pixels, and the vectors are grouped by direction)
    
     maxflow(
      const char_image & SS,  /* image of the source and of the sink (not the original image) */
      const image_type & gg, /* Boundaries */
      int iteration,     /* number of iterations */
      float tau,		 /* timestep */
      int number_of_threads /* the number of threads to execute if in parallel mode */
      );

    virtual image_type start();

    virtual ~maxflow(){      
      #ifdef UJIMAGE_DEBUG      
      std::cout << "destroying the maxflow object (" << static_cast<void*>(this) << ")\n";
      #endif /* UJIMAGE_DEBUG */        
    }
  }; /* maxflow */


  template<class image_type>
  packet<image_type>::packet( )
  {
    etap = start_dibble = end_dibble = direction = current_iteration = 0;
  } /* packet::packet */
  
  template<class image_type>
  packet<image_type>::~packet()
  {
    // intentionally left empty
  } /* packet::~packet */
  
  template<class image_type>
  void packet<image_type>::operator()( 
    int ID, 
    maxflow<image_type> * parent 
    )
  {



    this->ID = ID;
    this->parent = parent;

    bool _continue;
    
    ATOMIC(
      _continue = parent->conductor( *this );
      parent->shared_lock->lock_shared();
      );


   

    while ( _continue )
    {

      switch ( this->etap )
      {
      case __ETAP_POT:
	parent -> upDatePotencial( start_dibble, end_dibble );
	//std::cout << "__ETAP_POT " << start_dibble << " " << end_dibble << " this = " << this << "\n";
	break;
	
      case __ETAP_FLOW:
	parent -> upDateFlow( start_dibble, end_dibble, direction );
	//std::cout << "__ETAP_FLOW " << start_dibble << " " << end_dibble <<  " this = " << this <<  "\n";
	break;
	
      case __ETAP_CONSTR:
	parent -> upDateConstrain( start_dibble, end_dibble );
	//std::cout << "__ETAP_CONSTR " << start_dibble << " " << end_dibble <<  " this = " << this <<  "\n";
	break;
	
      default:
	  error("Packet is not properly set up.");
      } /* switch */

      parent->shared_lock->unlock_shared();
      

      ATOMIC(
	_continue = parent->conductor( *this );
	parent->shared_lock->lock_shared();  // shared lock is used for the synchronization of the etaps
	);
    }

    parent->shared_lock->unlock_shared();
      
    
  } /* packet::operator() */


  
  template <class image_type>
  void maxflow<image_type>::upDatePotencial(int startDibble, int endDibble){
    pixel_type * p_c;
    pixel_type * f_out;
    pixel_type * f_in;
    int fm1 /*fm1_vec[d]*/, start, end, length, q, w, e;
    vint fm1_vec(d);
    
    FORR(w, d){
      fm1_vec.reset();
      fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
      fm1=dim->position(fm1_vec);
      for (/*int*/ e=startDibble; e<=endDibble-1; e++){
	start=dibPotencial->values[e].start;
	end=dibPotencial->values[e].end;
	
	p_c=&(potencial[start]);
	f_out=&(flow_glob[w*length_glob+start]);
	f_in=&(flow_glob[w*length_glob+start-fm1]);
	length = end - start;
	//the hyper-super ultra fast loop
	FORR(q, length) {
	  p_c[q] -= tau * ( f_out[q]-f_in[q] );
	}
      }
    }
  } /* void upDatePotencial */


  
  template <class image_type>  
  void maxflow<image_type>::upDateFlow(int startDibble, int endDibble, int w /*direction*/){
    pixel_type *p, *pp1, *f;
    int start, end, length, pp1_pos, q, e;
    vint pp1_vec(d);

    pp1_vec.reset();
    pp1_vec[w]=1;
    pp1_pos=dim->position(pp1_vec);

    for ( /*int*/ e=startDibble; e<=endDibble-1; e++ ){
      start=dibFlow[w]->values[e].start;
      end=dibFlow[w]->values[e].end;
      f=&(flow_glob[w*length_glob+start]);
      p=&(potencial[start]);
      pp1=&(potencial[start+pp1_pos]);
      length=end-start;
      FORR(q, length){
	f[q] -= tau*(pp1[q]-p[q]);
      }
    }
  } /* void upDateFlow */



  template <class image_type>
  void maxflow<image_type>::upDateConstrain(int startDibble, int endDibble){
//     //local copies
    pixel_type dFabs[d/*compileDim*/];
    pixel_type * locInFlow[d/*compileDim*/];
    pixel_type * locOutFlow[d/*compileDim*/];
    pixel_type * locG;
    pixel_type v;
	
    //locals
    vint fm1s(d), fm1_vec(d);
    int start, end, length, q, w, e;
	
    // calculating differences between the flow and the point
    FORR( w, d ) {
      fm1_vec.reset();
      fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
      fm1s[w]=dim->position(fm1_vec);
    }
	
    //for (/*int*/ e=0; e<=dibConstrain->length-1; e++){
    for (/*int*/ e=startDibble; e<=endDibble-1; e++){
      start=dibConstrain->values[e].start;
      end=dibConstrain->values[e].end;
      length=end-start;
      locG=&(g_glob[start]);
      FORR(w,d) {
	locOutFlow[w]=&(flow_glob[w*length_glob+start]);
	locInFlow[w]=&(flow_glob[w*length_glob+start-fm1s[w]]);
      }
      FORR(q, length) {
	v=0.;
	FORR(w, d) {
	  // dFabs=max(-Fin, 0, Fout)
	  dFabs[w] = -locInFlow[w][q] > locOutFlow[w][q] ? -locInFlow[w][q] : locOutFlow[w][q];
	  if (0. > dFabs[w]) dFabs[w] = 0.;
	  // v = sum(dFabs^2)
	  v += static_cast<pixel_type>(dFabs[w] * dFabs[w]);
	}

	FORR(w, d) {
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
  void maxflow<image_type>::upDateSrcSink(void)
  // in this object this method is only called once, because the 
  // value of the source pixel does not change
  {

    FOR( q, length_glob ) 
    {
      if ( src_sink[q] == 1 ) 
      {
	potencial[q]=1.; 
      }
      else /* NOT  (*srcsink) == 1. */
      {
	if ( src_sink[q]==255 )
	{ 
	  potencial[q]=0.;	
	} /* if (*srcsink) == -1. */
      } /* NOT (*srcsink) == 1.  */
    } /* FOR */
  } /* maxflow::upDateSrcSink */
  


  template <class image_type>
  int maxflow<image_type>::uiCreateDibbles( 
    ){
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
	  int start=0, end=0;
	  int currlength=0;
	  boost::shared_array<unsigned char> curr;
	  curr = src_sink.get_pixels();
	  FOR(q, length_glob) {
	    if (started) {
	      currlength++;
	      if (curr[q]!=0) {
		end=q;
		dibPotencial->addElement(start, end);
		///!!! std::cout << "dibPotencial->addElement(" << start << ", " << end << ");\n";  //////
		start=0;
		end=0;
		started=false;
	      } else /* NOT (curr[q]!=0) */
		if (currlength>=MaxDibble) {
		  end=q+1;
		  dibPotencial->addElement(start, end);
		  ///!!! std::cout << "maxdibble dibPotencial->addElement(" << start << ", " << end << ");\n";  //////
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
	  int start=0, end=0;
	  unsigned char *pp1[d];
	  boost::shared_array<unsigned char> p;
	  vint pp1_vec(d);
	  int currlength=0;
	  vint pp1_pos(d);
	  // Calculating the shift of the neighbourh elements
	  FOR(w, d) {
	    pp1_vec.reset();
	    pp1_vec[w]=1;
	    int currpos = dim->position(pp1_vec);
///!!!	std::cout << "currpos=" << currpos << "\n";
	    pp1_pos[w]=currpos;
	    pp1[w]=&(src_sink[currpos]);
	  } /* FOR(w, d) */
	  p=src_sink.get_pixels();
	  FOR(w, d) {
	    FOR(q, length_glob - pp1_pos[w] /* - 1 */ ) { /////////
	      if (started){
		currlength++;
		if ((p[q]!=0) and (pp1[w][q]!=0)){
		  end=q;
		  dibFlow[w]->addElement(start, end);
		  ///!!! std::cout << "dibFlow[" << w << "]->addElement(" << start << "," << end << ")\n";
		  start=0;
		  end=0;
		  started=false;
		} else /* NOT ((p[q]!=0) and (pp1[w][q]!=0)) */
		  if (currlength>=MaxDibble){
		    end=q+1;/////!!!!!!!!!!!!!!!
		    dibFlow[w]->addElement(start, end);
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
	  int start=0, end=0;
	  unsigned char *pp1[d], *pm1[d];
	  boost::shared_array<unsigned char> p;
	  vint pp1_vec(d);
	  int currlength=0;
	  int max=0;
	  FOR(w, d) {
	    pp1_vec.reset();
	    pp1_vec[w]=1;
	    int currpos = dim->position(pp1_vec);
	    max = max > currpos ? max : currpos;
	    pp1[w]=&( src_sink[currpos] );
	    pm1[w]=&( src_sink[0] ) - currpos;// risky, we always read far enough from the beginning
	  } /* FOR(w, d) */
	  p=src_sink.get_pixels();
	  FOR( q, length_glob - max /*-1*/ ){ ///////////
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
		dibConstrain->addElement(start, end);
		///!!! std::cout << "dibConstrain->addElement(" << start << ", " << end << ")\n";
		start=0;
		end=0;
		started=false;
	      } else if (too_long){
		//printf("uiNotice: breaking because of the MaxDibble\n");
		end=q+1;///////!!!!!!
		dibConstrain->addElement(start, end);
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
  maxflow<image_type>::maxflow(
    const char_image & SS,  /* image of the source and of the sink (not the original image) */
    const image_type & gg, /* Boundaries */
    int iteration,     /* number of iterations */
    float tau,		 /* timestep */
    int number_of_threads=0 /* the number of threads to execute if in parallel mode */
    ) {

    #ifdef UJIMAGE_DEBUG
    std::cout << "creating the maxflow object (" << static_cast<void*>(this) << ")\n";	
    #endif /* UJIMAGE_DEBUG */        
   
    potencial.copy(gg); // "potencial";
    potencial.fill(0.);
    

    // creating a local copy of image, srcsink potencial and flows ---------------------------
    d = potencial.get_size().size();

    std::cout << "dimension = " << d << "D\n";

    length_glob = potencial.get_size().prod();

    std::cout << "length_glob=" << this->length_glob << "\n";

    if (number_of_threads!=0){      
      this->number_of_threads = number_of_threads;
    }
    else /* NOT (number_of_threads!=0) */
    {

      #ifdef __OPENMP
      this->number_of_threads = omp_get_num_procs();
      #else /* __OPENMP */
      this->number_of_threads = 1;
      #endif /* __OPENMP */

    }/* NOT (number_of_threads!=0) */

    #ifdef __OPENMP
    std::cout << "Using " << this->number_of_threads << " threads from " <<  omp_get_num_procs() << " available.\n";
    #else /* __OPENMP */
    std::cout << "maxflow compiled WITHOUT OPENMP support.\n";
    #endif /* __OPENMP */

    // setting up the lock
    global_lock.reset( new boost::mutex );
    shared_lock.reset( new boost::shared_mutex );

    // Now we copy the pointers to global variables, so the threads can see them.
    // boost::shared_ptr is boost's 'shared_array' smart pointer.
    this->gg.copy(gg);
    g_glob = this->gg.get_pixels();
    
    flow_glob.reset(new pixel_type[d*length_glob]);

    //cleaning the flow
    //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
    FOR(q, d*length_glob){
      flow_glob[q]=0.;
    } /* end of parallel FOR */



    // making 
    ///!!!! src_sink.reset(new char_image(SS, "maxflow::src_sink"));
    src_sink.copy(SS);
    
    this->tau = tau;
    std::cout << "tau=" << this->tau << "\n";

    this->iteration = iteration;
    std::cout << "iteration=" << this->iteration << "\n";

    
    this->flow_calculated = false; 
	
    //int dim [d];
    dim.reset(new vint(potencial.get_size()));

	
    //// --------------------- setting up source -------------------------------------------
	
    std::cout << "setting up source\n";
    pixel_type *ps;
    unsigned char *ss;
		
    ss = &( src_sink[0] );
    ps = &( potencial[0]);


    
    //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
    FOR(q, length_glob) {
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
  
    //// --------------------- breaking the fields into dibbles ----------------------------
	
    std::cout << "breaking up the field into dibbles\n";
    dibPotencial.reset( new uiDibbles() );
    dibConstrain.reset( new uiDibbles() );
    dibFlow.reset( new boost::shared_ptr<uiDibbles>[d+3] ); // we adding here 3 becaus of the parallelization later
    FOR(q,d+3) dibFlow[q].reset(new uiDibbles()); // we adding here 3 becaus of the parallelization later
	
    uiCreateDibbles( );
  } /*   maxflow<image_type>::maxflow */	



  template <class image_type>
  boost::shared_array<typename image_type::pixel_type> maxflow<image_type>::get_flow(){
    if (not flow_calculated){
      error("The flow has not yet been calculated. You can only call get_flow after at least 1 iteration."
	    " For the courious souls after the 0th iteration the flow is zero everywhere");
    }
    else /* (flow_calculated) */
    {
      return flow_glob;
    }/* (flow_calculated) */
    return flow_glob;
  } /* get_flow */



  template <class image_type>
  image_type maxflow<image_type>::start(){
    //// --------------------- initializing the time measure -------------------------------
    sentinel.maxPos(iteration);
    sentinel.minPos(0);
    sentinel << 0;
    sentinel.start();
    std::cout << "starting the iteration\n";


    #ifdef __OPENMP    
    FOR( e, iteration ) {    

      if ( e % REPORT_INTERVAL == 0 ) 
      {
	if ( sentinel.timeToReport() )
	{
	  std::cout << "Estimated time remaining: " << (sentinel << e) << std::endl;
	} /* timeToReport() */
      } /* if iterations ... */
      
      
      //#pragma omp parallel for schedule(static, 100) num_threads(this->number_of_threads)    
      FOR( par, dibPotencial->get_length() ) {
	upDatePotencial( par, par + 1 ); /// for all the dibbles update the potencial on dible par 
	                                 /// (from par to par+1 not included)
      } /* FOR */
      
      
      
      FOR( w, d ){
        //#pragma omp parallel for schedule(static, 100) num_threads(this->number_of_threads)    
	FOR( par, dibFlow[w]->get_length() ){
	  upDateFlow( par, par + 1, w );
	} /* FOR */
      } /* FOR(w,d) */ 
      
      
      //#pragma omp parallel for schedule(static, 100) num_threads(this->number_of_threads)    
      FOR( par, dibConstrain->get_length() ){
	upDateConstrain( par, par + 1 );
      } /* FOR */
    } /* FOR(e, iteration) */


    #else /* __OPENMP */
      // Posix threading



    int nbt = this -> number_of_threads;

    boost::shared_array< boost::shared_ptr<boost::thread> > threads(new boost::shared_ptr<boost::thread>[nbt]);

    boost::shared_array< boost::shared_ptr< packet<image_type> > > packets(new boost::shared_ptr<packet<image_type> >[nbt]);

    // Thread attributes

    reference.reset(new packet<image_type>());
    reference->etap = __ETAP_POT;

    FOR( q, nbt )
    {
      packets[q].reset( new packet<image_type>() );
      threads[q].reset( new boost::thread( (*packets[q]), q, this ) );
    } /* FOR(q, nbt) */


    FOR( q, nbt )
    {
      threads[q]->join();
    } /* FOR(q, nbt) */

    #endif /* __OPENMP */


      


    //// --------------------- printing out the measured time ------------------------------
    sentinel.stop();
    std::cout << "total time of iteration: " << sentinel.elapsedTime() << std::endl;
	
    this->flow_calculated = true; 

    return potencial; /* measure field picture */
    //local variables are deleted automaticly
  } /*    maxflow<image_type>::start() */





  template <class image_type>
  bool maxflow<image_type>::conductor( 
    packet<image_type> & thread 
    )
  {

    switch ( reference->etap )
    {
    case __ETAP_POT:
      if (reference->end_dibble >= dibPotencial->get_length()) // the case, when the last iteration has been assigned and now we begin the next part
      {
	shared_lock->lock(); // we wait for the threads to finish the calculation
	reference->etap = __ETAP_FLOW;
	reference->start_dibble = 0;
	reference->end_dibble = _min( PACKET_SIZE, dibFlow[0]->get_length() );
	reference->direction = 0;

	thread.start_dibble = reference->start_dibble;
	thread.end_dibble = reference->end_dibble;
	thread.etap = reference->etap;
	thread.direction = reference->direction;

	shared_lock->unlock(); // from now on all the threads are starting the flow iteration.
      } 
      else /* NOT reference->end_dibble >= dibPotencial->get_length() */
      {
	reference->start_dibble = reference->end_dibble;
	reference->end_dibble = _min( reference->end_dibble + PACKET_SIZE, dibPotencial->get_length() );

	thread.etap = reference->etap;
	thread.start_dibble = reference->start_dibble;
	thread.end_dibble = reference->end_dibble;
      } /* NOT reference->end_dibble >= dibPotencial->get_length() */

      return true;
      break;
      
    case __ETAP_FLOW:

      if (reference->end_dibble >= dibFlow[reference->direction]->get_length()) // the case, when the last iteration has been assigned and now we begin the next part
      {

	if ( reference->direction >= d - 1 )
	{	  
	  shared_lock->lock(); // we wait for all threads to finish the calculation	  
	  reference->etap = __ETAP_CONSTR;
	  reference->start_dibble = 0;
	  reference->end_dibble = _min( PACKET_SIZE, dibConstrain->get_length());
	  
	  thread.start_dibble = reference->start_dibble;
	  thread.end_dibble = reference->end_dibble;
	  thread.etap = reference->etap;	  
	  thread.direction = reference->direction;
	  shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.
	}
	else /* NOT reference->current_direction >= d - 1 */
	{
//	  shared_lock->lock(); // we wait for all threads to finish the calculation

	  reference->direction += 1;
	  reference->start_dibble = 0;
	  reference->end_dibble = _min( PACKET_SIZE, 
					dibFlow[reference->direction] -> get_length());
	  
	  thread.start_dibble = reference->start_dibble;
	  thread.end_dibble = reference->end_dibble;
	  thread.direction = reference->direction;
	  thread.etap = reference->etap;

//	  shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.

	} /* NOT reference->current_direction >= d - 1 */
      } 
      else /* NOT reference->end_dibble >= dibFlow->get_length() */
      {
	reference->start_dibble = reference->end_dibble;
	reference->end_dibble = _min( reference->end_dibble + PACKET_SIZE, dibFlow[reference->direction]->get_length());

	thread.start_dibble = reference->start_dibble;
	thread.end_dibble = reference->end_dibble;
	thread.etap = reference->etap;
	thread.direction = reference->direction;
      } /* NOT reference->end_dibble >= dibFlow->get_length() */

      return true;
      break;
      
    case __ETAP_CONSTR:
      
      if (reference->end_dibble >= dibConstrain->get_length()) // the case, when the last iteration has been assigned and now we begin the next part
      {
	if ( reference->current_iteration >= iteration - 1 )
	{ // we are finished with the iterations.
	  // we wait for the rest of the threads
	  shared_lock->lock();	
	  
	  // we let the threads to finish
	  shared_lock->unlock();
	  return false;		  
	} 
	else /* NOT current_iteration >= iterations - 1 */	  
	{

	  if ( reference->current_iteration % REPORT_INTERVAL == 0 ) 
	  {
	    if ( sentinel.timeToReport() )
	    {
	      std::cout << "Estimated time remaining: " << (sentinel << reference -> current_iteration) << std::endl;
	    } /* timeToReport() */
	  } /* if iterations ... */

	  
	  shared_lock->lock(); // we wait for all threads to finish the calculation
	  reference->current_iteration++;
	  

	  reference->etap = __ETAP_POT;
	  reference->start_dibble = 0;
	  reference->end_dibble = _min(PACKET_SIZE, dibPotencial->get_length());

	  thread.start_dibble = reference->start_dibble;
	  thread.end_dibble = reference->end_dibble;
	  thread.etap = reference->etap;

	  shared_lock->unlock(); // from now on all the threads are starting the constraint iteration.
	  
	  return true;
	} /* NOT reference->current_iteration >= iterations - 1 */
      }
      else /* NOT reference->end_dibble 1 >= dibConstraint->get_length() */
      {
	reference->start_dibble = reference->end_dibble;
	reference->end_dibble = _min( reference->end_dibble + PACKET_SIZE, dibConstrain->get_length());

	thread.start_dibble = reference->start_dibble;
	thread.end_dibble = reference->end_dibble;
	thread.etap = reference->etap;

	return true;
      } /* NOT reference->end_dibble >= dibConstraint->get_length() */


      break;
      
    default:
      error("Packet error.");
    } /* switch */
      
   
  } /* maxflow::conductor */

} /* end namespace pink */


// cleaning up after us
#undef REPORT_INTERVAL
#undef PACKET_SIZE
#undef __ETAP_POT
#undef __ETAP_FLOW
#undef __ETAP_CONSTR
#undef ATOMIC


#endif /*UIFLOW_HPP_*/
/* LuM end of file */
