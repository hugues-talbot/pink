// fejes attila
/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_OPENCL_HPP_
#define UIFLOW_OPENCL_HPP_

#define BOOST_DISABLE_ASSERTS

#include <CL/cl.h>
#include <vector>
#include <utility>

//#include "uiFlow.hpp"
#include "uiFrame.hpp"
#include "lminmax.hpp"
#include "pyujimage.hpp"
#include "ui_numa_types.hpp"


namespace pink {
  
  namespace opencl {

#   define REPORT_INTERVAL 10
    const index_t CUDA_CORES = 512;
    


    const char * compiler_options = "-cl-mad-enable -cl-no-signed-zeros -cl-unsafe-math-optimizations -cl-finite-math-only";
// note fast relaxed math does not compile
//"-cl-mad-enable -cl-no-signed-zeros -cl-unsafe-math-optimizations -cl-finite-math-only -cl-fast-relaxed-math";
    
    
    const char * opencl_source =
      "    __kernel void kernel_pot(             \n"                    \
      "      __global float * pot,               \n"                    \
      "      __global float * flow,              \n"                    \
      "      __global float * srcsink,           \n"                    \
      "      __const  int   p_c,                 \n"                    \
      "      __const  int   f_out,               \n"                    \
      "      __const  int   f_in,                \n"                    \
      "      __const  float tau,                 \n"                    \
      "      __const  int   length               \n"                    \
      "      )                                   \n"                    \
      "    {                                     \n"                    \
      "      size_t id  = get_global_id(0);      \n"                    \
      "      if (id >= length) return;           \n"                    \
      "      float norm = fabs(srcsink[ p_c + id ]) > 0 ? 0 : pot[ p_c + id ]; \n" \
      "      norm = srcsink[ p_c + id ] > 0 ? 1 : norm; \n"             \
      "      pot[ p_c + id ] = norm - tau * ( flow[ f_out + id ] - flow[ f_in + id ] );\n" \
      "    }                                     \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "    __kernel void kernel_flow(            \n"                    \
      "      __global float * pot,               \n"                    \
      "      __global float * flow,              \n"                    \
      "      __const  int   f,                   \n"                    \
      "      __const  int   p,                   \n"                    \
      "      __const  int   pp1,                 \n"                    \
      "      __const  float tau,                 \n"                    \
      "      __const  int   length               \n"                    \
      "      )                                   \n"                    \
      "    {                                     \n"                    \
      "      size_t id       = get_global_id(0); \n"                    \
      "      if (id >= length) return;           \n"                    \
      "      flow[ f + id ] -= tau * ( pot[ pp1 + id ] - pot[ p + id ] ) ; \n" \
      "    }                                     \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "    __kernel void kernel_srcsink(         \n"                    \
      "      __global float * pot,               \n"                    \
      "      __global float * src,               \n"                    \
      "      __global float * srcsink,           \n"                    \
      "      __const  int   length               \n"                    \
      "      )                                   \n"                    \
      "    {                                     \n"                    \
      "      size_t id = get_global_id(0);       \n"                    \
      "      if (id >= length) return;           \n"                    \
      "      pot[id] = pot[id]*srcsink[id] + src[id];\n"                \
      "    }                                     \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "                                          \n"                    \
      "    __kernel void kernel_constrain(       \n"                    \
      "      __global float * flow,              \n"                    \
      "      __global float * g,                 \n"                    \
      "      __const  int start,                 \n"                    \
      "      __const  int length_glob,           \n"                    \
      "      __const  int fm1s_0,                \n"                    \
      "      __const  int fm1s_1,                \n"                    \
      "      __const  int length                 \n"                    \
      "        )                                 \n"                    \
      "    {                                     \n"                    \
      "      size_t id = get_global_id(0);       \n"                    \
      "      if (id >= length) return;           \n"                    \
      "      float v=0.;                         \n"                    \
      "      float dFabs[2];                     \n"                    \
      "      float locInFlow[2];                 \n"                    \
      "      float locOutFlow[2];                \n"                    \
      "      float locG;\n"                                             \
      "\n"                                                              \
      "\n"                                                              \
      "      locG = g[ start + id ];\n"                                 \
      "\n"                                                              \
      "      locOutFlow[0] = flow[ length_glob * 0 + start + id ];\n"   \
      "      locInFlow [0] = flow[ length_glob * 0 + start - fm1s_0 + id ];\n" \
      "      locOutFlow[1] = flow[ length_glob * 1 + start + id ];\n"   \
      "      locInFlow [1] = flow[ length_glob * 1 + start - fm1s_1 + id ];\n" \
      "      \n"                                                        \
      "\n"                                                              \
      "      dFabs[0] = -locInFlow[0] > locOutFlow[0] ? -locInFlow[0] : locOutFlow[0];\n" \
      "      v += (0. < dFabs[0]) * dFabs[0] * dFabs[0];\n"             \
      "\n"                                                              \
      "      dFabs[1] = -locInFlow[1] > locOutFlow[1] ? -locInFlow[1] : locOutFlow[1];\n" \
      "      v += (0. < dFabs[1]) * dFabs[1] * dFabs[1];\n"             \
      "\n"                                                              \
      "      float quotient = ( locG * locG < v ) ? (locG / sqrt(v)) : 1; \n" \
      "\n"                                                              \
      "      dFabs[0] *= quotient; \n"                                  \
      "      dFabs[1] *= quotient; \n"                                  \
      "      if ( locInFlow[0]  < -dFabs[0]) flow[ length_glob * 0 + start - fm1s_0 + id ] = -dFabs[0];\n" \
      "      if ( locOutFlow[0] > dFabs[0] ) flow[ length_glob * 0 + start + id ]          =  dFabs[0];\n" \
      "      if ( locInFlow[1]  < -dFabs[1]) flow[ length_glob * 1 + start - fm1s_1 + id ] = -dFabs[1];\n" \
      "      if ( locOutFlow[1] > dFabs[1] ) flow[ length_glob * 1 + start + id ]          =  dFabs[1];\n" \
      "      \n"                                                        \
      "    } /* kernel_constrain */\n"                        
      ;


    
    template <class T0, class T1>
    inline void
    success(T0 t0, T1 t1)
    {
      if (t0 != CL_SUCCESS)
      {
        std::cout << "operation " << t1 << " failed with code " << t0 << std::endl;
        pink_error("operation failed");
      }
      return;
      
    }

    template <class T0, class T1>
    size_t shrRoundUp( T0 group_size, T1 global_size ) 
    {
      int r = global_size % group_size;
      if( r == 0 )
        return global_size;
      return global_size + group_size - r;
    }


    
    template <class T0>
    class opencl_t
    {

    public:
      typedef T0 value_type;
      
    private:                                          
      cl_context            context;                  
      cl_context_properties properties[3];                  
      cl_kernel             kernel_pot, kernel_flow, kernel_cons;      
      cl_command_queue      command_queue;            
      cl_program            program;                  
      cl_int                choosen_platform; 
      cl_uint               num_of_platforms;       
      cl_platform_id        platform_ids[10];              
      cl_device_id          device_ids[10];                
      cl_uint               num_of_devices;
      cl_mem                gpu_pot, gpu_flow, gpu_g, gpu_srcsink;
      bool                  verbose;      

    public:

      template <class MT0, class MT1, class MT2, class MT3>
      opencl_t(
        MT0 & cpu_pot,
        MT1 & cpu_flow,
        MT2 & cpu_g,
        MT3 & cpu_srcsink,
        bool verbose = false
        ) : verbose(verbose)      
        {
          cl_int err;
          
          char dname[500];
          size_t namesize;
          // retreive the number of platforms avaible          
          clGetPlatformIDs (0, NULL, &num_of_platforms);
          if (verbose)
          {            
            std::cout << num_of_platforms << " opencl platforms found" << std::endl;
          }          

          if (clGetPlatformIDs(num_of_platforms, platform_ids, NULL )!= CL_SUCCESS)
          {
            pink_error("Unable to get platform_id");
          }

          for(cl_uint i = 0; i < num_of_platforms; ++i)
          {
            char cBuffer[1024];
            clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, 1024, cBuffer, NULL);
            if (verbose)
            {              
              std::cout << "platform [" << i << "]: " << cBuffer << std::endl;
            }            
            // if(strstr(cBuffer, "NVIDIA") != NULL)
            // {
              choosen_platform = i;              
              //}
          } /* for i in num_of_platforms */

          if (verbose)
          {            
            std::cout << "Choosen device = " << choosen_platform << std::endl;
          } /* if verbose */
                    
          // retreive OpenCL device CPU or GPU
          // try to get a supported GPU device
          if (clGetDeviceIDs(platform_ids[choosen_platform], CL_DEVICE_TYPE_ALL, 10, device_ids, &num_of_devices) != CL_SUCCESS)
          {
            pink_error("Unable to get device_id");
          }

          if (num_of_devices > 10)
          {
            pink_error("You have more than 10 OpenCL enabled devices. You need to make an adjustment in the source for this to succeed");            
          }
                    
          FOR (q, num_of_devices)
          {
            clGetDeviceInfo(device_ids[q], CL_DEVICE_NAME, 500, dname, &namesize);
            if (verbose)
            {              
              std::cout << "Device #" << q << " name = " << dname << std::endl;
            }            
          } /* FOR q in num_of_devices */

          // context properties list - must be terminated with 0
          properties[0] = CL_CONTEXT_PLATFORM;
          properties[1] = (cl_context_properties)platform_ids[choosen_platform];
          properties[2] = 0;
  
          // create a context with the GPU device
          context = clCreateContext( properties, num_of_devices, device_ids, NULL, NULL, &err );
          success(err, "create_context");
          
          // create command queue using the context and device
          command_queue = clCreateCommandQueue(context, device_ids[0/*!!!!!!!!*/], 0 /*CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE */, &err);
          success(err, "command_queue");
          
          
          // create a program from the kernel source code
          program = clCreateProgramWithSource(context, 1, (const char **)(&opencl_source), NULL, &err);
          success(err, "create_program");
          
          // compile the program
          if (clBuildProgram(program, 0, NULL, compiler_options, NULL, NULL) != CL_SUCCESS)
          {
            std::cout << "Build failed. Error Code = " << err << std::endl;
             
            size_t len;
            char buffer[2048];
            // get the build log
            clGetProgramBuildInfo(
              program, device_ids[0 /*!!!!!!!*/], CL_PROGRAM_BUILD_LOG,
              sizeof(buffer),
              buffer,
              &len
              );
            std::cout << "--- Build Log -- " << std::endl << buffer << std::endl;
            pink_error("couldn't build the opencl program");
          }

          // create kernel object
          kernel_pot     = clCreateKernel(program, "kernel_pot",       &err);
          success(err, "kernel_pot");
          kernel_flow    = clCreateKernel(program, "kernel_flow",      &err);
          success(err, "kernel_flow");
          kernel_cons    = clCreateKernel(program, "kernel_constrain", &err);
          success(err, "kernel_cons");
          
          // create memory objects 
          gpu_pot     = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(value_type) * cpu_pot .size(),    NULL, &err);
          success(err, "gpu_pot");
          gpu_flow    = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(value_type) * cpu_flow.size(),    NULL, &err);
          success(err, "gpu_flow");
          gpu_g       = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(value_type) * cpu_g.size(),       NULL, &err);
          success(err, "gpu_g");
          gpu_srcsink = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(value_type) * cpu_srcsink.size(), NULL, &err);
          success(err, "gpu_srcsink");

          // copy data to memory objects
          success(clEnqueueWriteBuffer(command_queue, gpu_pot,     CL_TRUE, 0, sizeof(value_type) * cpu_pot .size(),    cpu_pot .get(),    0, NULL, NULL), "gpu_pot copy to gpu");
          success(clEnqueueWriteBuffer(command_queue, gpu_flow,    CL_TRUE, 0, sizeof(value_type) * cpu_flow.size(),    cpu_flow.get(),    0, NULL, NULL), "gpu_flow copy to gpu");
          success(clEnqueueWriteBuffer(command_queue, gpu_g,       CL_TRUE, 0, sizeof(value_type) * cpu_g.size(),       cpu_g.get(),       0, NULL, NULL), "gpu_g copy to gpu");
          success(clEnqueueWriteBuffer(command_queue, gpu_srcsink, CL_TRUE, 0, sizeof(value_type) * cpu_srcsink.size(), cpu_srcsink.get(), 0, NULL, NULL), "gpu_srcsink copy to gpu");
          
          success(clFinish(command_queue), "write_sync" );

          // default kernel arguments (the ones which do not change)
          clSetKernelArg( kernel_pot, 0, sizeof(cl_mem),   &gpu_pot  );
          clSetKernelArg( kernel_pot, 1, sizeof(cl_mem),   &gpu_flow );
          clSetKernelArg( kernel_pot, 2, sizeof(cl_mem),   &gpu_srcsink );

          clSetKernelArg( kernel_flow, 0, sizeof(cl_mem),   &gpu_pot  );
          clSetKernelArg( kernel_flow, 1, sizeof(cl_mem),   &gpu_flow );

          success( clSetKernelArg( kernel_cons, 0, sizeof(cl_mem), &gpu_flow    ), "constrain_arg1");
          success( clSetKernelArg( kernel_cons, 1, sizeof(cl_mem), &gpu_g       ), "constrain_arg2");

        } /* opencl::opencl */


      template <class MT0>
      void sync( MT0 & cpu_pot )
        {
          // read data back from device
          success(clEnqueueReadBuffer(command_queue, gpu_pot,  CL_TRUE, 0, sizeof(value_type) * cpu_pot .size(), cpu_pot .get(), 0, NULL, NULL), "read pot to ram");
          // success(clEnqueueReadBuffer(command_queue, gpu_flow, CL_TRUE, 0, sizeof(value_type) * cpu_flow.size(), cpu_flow.get(), 0, NULL, NULL), "read flow to ram");
          success(clFinish(command_queue), "ramread barrier");
        } /* opencl::sync */
      
      
      
      void update_potencial(
        size_t   length,
        cl_int   p_c,
        cl_int   f_out,
        cl_int   f_in,
        cl_float tau )
        {
          cl_int err;
          // clSetKernelArg( kernel_pot, 0, sizeof(cl_mem),   &gpu_pot  );
          // clSetKernelArg( kernel_pot, 1, sizeof(cl_mem),   &gpu_flow );
          // clSetKernelArg( kernel_srcsink, 2, sizeof(cl_mem),   &gpu_srcsink );          
          clSetKernelArg( kernel_pot, 3, sizeof(cl_int),   &p_c      );          
          clSetKernelArg( kernel_pot, 4, sizeof(cl_int),   &f_out    );          
          clSetKernelArg( kernel_pot, 5, sizeof(cl_int),   &f_in     );          
          clSetKernelArg( kernel_pot, 6, sizeof(cl_float), &tau      );
          clSetKernelArg( kernel_pot, 7, sizeof(cl_int),   &length );
          
          size_t local_ws = CUDA_CORES;
          size_t global   = shrRoundUp(local_ws, length ); // Total number of work-item
          
          // enqueue kernel in command queue
          err = clEnqueueNDRangeKernel( command_queue, kernel_pot, 1, NULL, &global, &local_ws, 0, NULL, NULL );
          success(err, "pot_calculation");
          //success(clFinish(command_queue), "ramread barrier");
        } /* update_pot */


      
      void update_flow(
        size_t   length,
        cl_int   f,
        cl_int   p,
        cl_int   pp1,
        cl_float tau )
        {
          cl_int err;
          size_t local_ws = CUDA_CORES;
          size_t global   = shrRoundUp(local_ws, length ); // Total number of work-item
          // success( clSetKernelArg( kernel_flow, 0, sizeof(cl_mem),   &gpu_pot  ), "flow_arg1");
          // success( clSetKernelArg( kernel_flow, 1, sizeof(cl_mem),   &gpu_flow ), "flow_arg2");
          clSetKernelArg( kernel_flow, 2, sizeof(cl_int),   &f   );
          clSetKernelArg( kernel_flow, 3, sizeof(cl_int),   &p   );
          clSetKernelArg( kernel_flow, 4, sizeof(cl_int),   &pp1 );
          clSetKernelArg( kernel_flow, 5, sizeof(cl_float), &tau );
          clSetKernelArg( kernel_flow, 6, sizeof(cl_int),   &length );

          // enqueue kernel in command queue
          err = clEnqueueNDRangeKernel( command_queue, kernel_flow, 1, NULL, &global, &local_ws, 0, NULL, NULL );
          success(err, "flow_calculation");
          //success(clFinish(command_queue), "ramread barrier");
        } /* update_flow */


      
      template <class MT0>
      void update_constrain(
        cl_int length,
        cl_int length_glob,
        cl_int max,
        MT0 & fm1s )
        {
          cl_int err;
          cl_int fm1s_0   = fm1s[0];
          cl_int fm1s_1   = fm1s[1];
          size_t local_ws = CUDA_CORES;
          size_t global   = shrRoundUp(local_ws, length ); // Total number of work-item
          
          //success( clSetKernelArg( kernel_cons, 0, sizeof(cl_mem), &gpu_flow    ), "constrain_arg1");
          //success( clSetKernelArg( kernel_cons, 1, sizeof(cl_mem), &gpu_g       ), "constrain_arg2");
          clSetKernelArg( kernel_cons, 2, sizeof(cl_int), &max       );
          clSetKernelArg( kernel_cons, 3, sizeof(cl_int), &length_glob );
          clSetKernelArg( kernel_cons, 4, sizeof(cl_int), &fm1s_0      );
          clSetKernelArg( kernel_cons, 5, sizeof(cl_int), &fm1s_1      );
          clSetKernelArg( kernel_cons, 6, sizeof(cl_int), &length      );

          // enqueue kernel in command queue
          err = clEnqueueNDRangeKernel( command_queue, kernel_cons, 1, NULL, &global, &local_ws, 0, NULL, NULL );
          success(err, "constrain_calculation");
          //success(clFinish(command_queue), "ramread barrier");
        } /* update_cons */
      
      
      
      ~opencl_t()
        {
          // // clean up
          // // cleanup - release OpenCL resources
          clReleaseMemObject(    gpu_pot  );
          clReleaseMemObject(    gpu_flow );
          clReleaseProgram(      program  );
          clReleaseKernel(       kernel_pot );
          clReleaseKernel(       kernel_flow );
          clReleaseCommandQueue( command_queue );
          clReleaseContext(      context );
        } /* opencl::~opencl */
        
    }; /* class opencl */
    
    
    
    template <class image_type>
    class opencl_flow 
    {
      
    private:

      typedef typename image_type::pixel_type        pixel_type;
      typedef pink::types::shared_vector<pixel_type> poly_array_t;      

      // the result will be put in this image
      image_type potencial;      

      // these distributed arrays contain the fields during the iterations
      /* changed from std::vector*/ poly_array_t flow_glob; // flow is allocated in each direction, so we have as many flows as directions
      poly_array_t  g_glob;
      poly_array_t  pot_glob;
      poly_array_t  srcsink_glob;

      index_t starttime;
      bool verbose; // for debugging      
      
      boost::shared_ptr<opencl_t<pixel_type> > opencl;
      
    protected:

      index_t d;
      index_t length_glob;
      vint    dim;
      float   tau;
      
      progressBar sentinel;

      // the number of desired iterations
      index_t       iteration;

      // this way when the vector destroys the array it will
      // destroy all the elements as well
      pink::types::vector2D<pink::types::dibble_t> dib_flow; 
      
      
      // functions for calculation
      void update_constrain()        
        {

          //locals
          vint fm1s(d), fm1_vec(d);
          index_t start, end, length, max;

          max = 0;
          
          // calculating differences between the flow and the point
          FOR( w, d )
          {
            fm1_vec.reset();
            fm1_vec[w] = 1;//we are calculating the distance the opposite direction but it should be the same
            fm1s[w]    = dim.position(fm1_vec);
            max        = fm1s[w] > max ? fm1s[w] : max;            
          }
          
          opencl->update_constrain(            
            length_glob - 2 * max,
            length_glob,
            max,
            fm1s
            );
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
      void update_potencial()
        {
          index_t fm1 /*fm1_vec[d]*/, start, end;            
          vint fm1_vec(d);
            
          FOR(w, d)
          {
            fm1_vec.reset();
            fm1_vec[w] = 1; //we are calculating the distance the opposite direction but it should be the same
            fm1 = dim.position(fm1_vec);

            
            opencl->update_potencial(
              length_glob - fm1,
              fm1 /* start */,
              length_glob * w + fm1,
              length_glob * w,
              tau
              );            
          } /* FORR d */
        } /* update_potencial */


      template <class MT0>
      void update_flow(MT0 w /*direction*/)
        {
          index_t pp1_pos;
          vint pp1_vec(d);

          pp1_vec.reset();
          pp1_vec[w]=1;
          pp1_pos=dim.position(pp1_vec);

          opencl->update_flow(
            length_glob - pp1_pos,
            length_glob * w,
            0,
            pp1_pos,
            tau
            );

        } /* update_flow */

      

      void update_srcsink()
        {
          opencl->update_srcsink( pot_glob.size() );
        } /* update_src */
     
      

    public:

     
        
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
      opencl_flow(
        const char_image & SS,     /* image of the source and of the sink (not the original image) */
        const image_type & gg,     /* Boundaries */
        index_t iteration,         /* number of iterations */
        float   tau,		   /* timestep */
        bool    verbose = false    /* debug info */
        ) :
        tau(          tau),
        d(            gg.get_size().size() ),
        dim(          gg.get_size() ),
        iteration(    iteration ),
        length_glob(  gg.get_size().prod()),
        g_glob(       gg.get_size().prod()),
        pot_glob(     gg.get_size().prod()),
        srcsink_glob( gg.get_size().prod()),
        flow_glob(    gg.get_size().prod() * gg.get_size().size() ), // number_of_threads, d*length_glob, NUMA )
        potencial(    gg.get_size() ), // we will put the result in this image
        verbose(      verbose)
        {
#         ifdef UJIMAGE_DEBUG
          std::cout << "creating the clflow object (" << static_cast<void*>(this) << ")" << std::endl;	
#         endif /* UJIMAGE_DEBUG */        

          if ( d != 2 )
          {
            pink_error("this function is only prepared for 2D images at this time");            
          } /* d != 2 */
          
          // these lines are no longer valid!
          // potencial.copy(gg); // "potencial";
          // potencial.fill(0.);

          if (verbose)
          {            
            std::cout << "opencl edition" << std::endl;
            // creating a local copy of image, srcsink potencial and flows ---------------------------
            std::cout << "dimension   = " << dim.repr() << " (" << d << "D)" << std::endl;
            std::cout << "length_glob = " << this->length_glob << std::endl;
            std::cout << "tau         = " << this->tau << std::endl;
            std::cout << "iteration   = " << this->iteration << std::endl;
          } /* if verbose */

          if (verbose)
            std::cout << "initializing the arrays" << std::endl;
          //cleaning the flow
          //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
          FOR(q, d * length_glob)
          {            
            flow_glob[q]    = 0.;
          } /* for d */

          FOR(q, length_glob)
          {                      
            g_glob[q]       = 0.;
            pot_glob[q]     = 0.;
            srcsink_glob[q] = 0.;
          }
          
          FOR(q, gg.get_size().prod() /* NOT length_glob*/)
          {
            g_glob[q]   = gg(q);            
          }

          if (verbose)
          {            
            std::cout << "setting up source" << std::endl;
          } /* if verbose */
          
          FOR(q, SS.get_size().prod() /* NOT length_glob*/)
          {
            // setting up the eliminator part
            if (SS(q)==1) 
            {
              srcsink_glob[q] = 1.;
            }
            else if (SS(q)==255)
            {
              srcsink_glob[q] = -1.;
            }
          }

          if (verbose)
          {            
            std::cout << "initializing OpenCL" << std::endl;
          } /* if verbose */
          opencl.reset( new opencl_t<pixel_type>( pot_glob, flow_glob, g_glob, srcsink_glob, verbose ) );
                    
        } /* constructor opencl_flow */
      

      
      virtual image_type start()
        {
          //// --------------------- initializing the time measure -------------------------------
          sentinel.maxPos(iteration);
          sentinel.minPos(0);
          sentinel << 0;

          sentinel.start();
          
          if (verbose)
          {            
            std::cout << "starting the iteration" << std::endl;
          }          
          
          FOR(e, iteration)
          {
            update_potencial();
            FOR(w, d)
            {
              update_flow(w);
            }            
             update_constrain();
          } /* FOR e in iteration */

          //// --------------------- printing out the measured time ------------------------------

          opencl->sync(pot_glob);          

          sentinel.stop();
          if (verbose)
          {            
            std::cout << "total time of iteration: " << sentinel.elapsedTime() << std::endl;
          }          

          vint time_cheat(potencial.get_size().size(), 0);
          time_cheat[0] = sentinel.elapsedSeconds();

          if (verbose)
          {            
            std::cout << "setting time_cheat to " << time_cheat.repr() << std::endl;
          }          

          // copy the calculated potencial to the 'potencial' image
          FOR( q, dim.prod() )
          {
            potencial(q)=pot_glob[q];            
          } /* for q */
          
          potencial.set_center_vint(time_cheat);

          return potencial;          
        } /* virtual start*/
      

      virtual ~opencl_flow()
        {      
#         ifdef UJIMAGE_DEBUG      
          std::cout << "destroying the clflow object (" << static_cast<void*>(this) << ")" << std::endl;
#         endif /* UJIMAGE_DEBUG */        
        }
    }; /* class opencl_flow */



    template <class image_type>
    image_type
    clflow( 
      char_image SS,  /* image of the source and of the sink (not the original image) */
      image_type gg, /* Boundaries */
      index_t    iteration,          /* number of iterations */
      float      glob_tau = 0.132,   /* timestep */
      bool       verbose = false     /* print details about the iteration */
      )
    {
      opencl_flow<image_type> obj(
        frame_around(SS, -1),
        frame_around(gg, 0.),
        iteration,
        glob_tau,
        verbose
        );
      
      image_type result = frame_remove(obj.start());
      return result;
    } /* clflow */
    
  } /* namespace opencl */   
} /* end namespace pink */


// cleaning up after us
#undef REPORT_INTERVAL

// // obsolete, replaced by enum
// #undef __ETAP_POT
// #undef __ETAP_FLOW
// #undef __ETAP_CONSTR

// // obsolete, loaded as a parameter
// #undef PACKET_SIZE


#endif /* UIFLOW_OPENCL_HPP_*/
/* LuM end of file */
