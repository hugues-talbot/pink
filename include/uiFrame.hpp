/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_FRAME__
#define UI_FRAME__

#include "mcimage.h"
#include "ujimage.hpp"
#include <boost/thread.hpp>

// This is more of a curiosity question than something that needs
// actual solving, but is there a way to determine how many cores a
// machine has from C++ in a platform-independent way? If no such
// thing exists, what about determining it per-platform
// (Windows/*nix/Mac)?

// Win32:

// SYSTEM_INFO sysinfo;
// GetSystemInfo( &sysinfo );

// numCPU = sysinfo.dwNumberOfProcessors;

// Linux, Solaris, & AIX (per comments):

//  numCPU = sysconf( _SC_NPROCESSORS_ONLN );

// FreeBSD, MacOS X, NetBSD, OpenBSD, etc.:

// int mib[4];
// size_t len = sizeof(numCPU); 

// /* set the mib for hw.ncpu */
// mib[0] = CTL_HW;
// mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

// /* get the number of CPUs from the system */
// sysctl(mib, 2, &numCPU, &len, NULL, 0);

// if( numCPU < 1 ) 
// {
//      mib[1] = HW_NCPU;
//      sysctl( mib, 2, &numCPU, &len, NULL, 0 );

//      if( numCPU < 1 )
//      {
//           numCPU = 1;
//      }
// }

// HPUX:

// numCPU = mpctl(MPC_GETNUMSPUS, NULL, NULL);

// IRIX:

// numCPU = sysconf( _SC_NPROC_ONLN );

// Mac OS X using Objective-C (10.5 and higher):

// NSUInteger a = [[NSProcessInfo processInfo] processorCount];
// NSUInteger b = [[NSProcessInfo processInfo] activeProcessorCount];

// link|edit|flag
	
// edited Dec 31 '10 at 15:41

	
// answered Sep 29 '08 at 22:14
// ceretullis
// 7,16852056

namespace pink { 

  template <class image_type>
  image_type frame(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {

    image_type result;
    result.copy(src_image);

    vint curr( src_image.get_size() );
    
    FOR(q, src_image.get_size().prod())
    {
      src_image.get_size().nextStep( q, curr );
      if (src_image.get_size().on_side( curr ))
      {
	result[q]=withval;	
      } /* if */
    } /* FOR */

    return result;

  } /* frame */


  template <class image_type>
  void frame_around_copy(
    image_type src_image,
    image_type result,
    index_t from,
    index_t to,
    typename image_type::pixel_type withval
    )
  {
    vint size(result.get_size());
    int d=size.size();
    vint curr(d);

    for (index_t q = from; q < to; q++)
    {
      
      // first we treat the case of the sides      
      size.nextStep( q, curr );

      if (size.on_side(curr))
      {
        result[curr]=withval;
      }
      else /* NOT size.on_side(curr) */
      {
        FOR(w, d)
        {
          curr[w]-=1;          
        }
        result(q)=src_image[curr];        
      } /* NOT size.on_side(curr) */      
      
    } /* for q in from..to */

    return;    
  } /* frame_around_copy */
  


  template <class image_type>
  image_type frame_around(
    const image_type & src_image, 
    typename image_type::pixel_type withval 
    )
  {
    int num_cpu = sysconf( _SC_NPROCESSORS_ONLN );
    
    boost::shared_ptr<boost::thread> threads[num_cpu];
    
#   if UJIMAGE_DEBUG > 1
    std::cout << "Framing; I have detected " << num_cpu << " cpu cores" << std::endl;
#   endif /* UJIMAGE_DEBUG */
    
    vint new_size(src_image.get_size().size(), "new_size" );
    
    // calculating the size of the framed image
    FOR(q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] + 2;      
    } /* FOR */
    
    image_type result(new_size);
    index_t nbp = result.get_size().prod();


    // saving the center of the image
    result.set_center_vint(src_image.get_center());
    
    vint curr( src_image.get_size().size(), "curr"  );


    if (nbp > 10000000)
    {
#     ifdef UJIMAGE_DEBUG
      std::cout <<
        "Parallelizing during framing because the size of the image is big enough; "
        "I have detected " << num_cpu << " cpu cores" << std::endl;
#     endif /* UJIMAGE_DEBUG */      
    
      // launching the threads
      FOR(q, num_cpu)
      {
        threads[q].reset(
          new boost::thread(
            frame_around_copy<image_type>,
            src_image,
            result,
            nbp * q / num_cpu, /* from */
            nbp * (q+1) / num_cpu, /* to */
            withval
            )
          );        
      } /* FOR, q, num_cpu */

      FOR(q, num_cpu)
      {
        threads[q]->join();        
      } /* FOR, q, num_cpu */
      
    } /* end of the parallel code */    
    else /* NOT nbp > 10000000*/
    {
      frame_around_copy(src_image, result, 0, nbp, withval);      
    } /* NOT nbp > 10000000*/
    
    return result;

  } /* frame_around */

  template <class image_type>
  image_type frame_remove( const image_type & src_image )
  {
    
    vint new_size(src_image.get_size().size());


    
    // calculating the size of the framed image
    FOR( q, new_size.size() )
    {
      new_size[q] = src_image.get_size()[q] - 2;
    } /* FOR */
    
    image_type result(new_size);

    // saving the center of the image
    result.set_center_vint(src_image.get_center());
    
    vint curr( src_image.get_size() );
        
    // copying the original image into the new image
    FOR( q, result.get_size().prod() )
    {
      result.get_size().nextStep(q, curr);
      FOR( w, new_size.size() )
      {
	curr[w]+=1;  // same as with frame around
      } /* FOR */
      result(q)=src_image[curr];
    } /* FOR */
   
    return result;

  } /* remove_frame */

































  } /* namespace pink */


#endif /* UI_FRAME__ */
/* LuM end of file */
