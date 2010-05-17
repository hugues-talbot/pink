# -*- sh -*-
# UjoImro, 2010

#message("Abandon every hope, ye who entereth here.")

#message(CMAKE_SYSTEM_TYPE\ ${CMAKE_SYSTEM_TYPE} )
#message(CMAKE_HOST_SYSTEM_NAME\ ${CMAKE_HOST_SYSTEM_NAME} )
#message(CMAKE_HOST_SYSTEM\ ${CMAKE_HOST_SYSTEM} )
#message( APPLE\ ${APPLE} )
#message( CMAKE_HOST_APPLE\ ${CMAKE_HOST_APPLE} )
#message( CMAKE_HOST_WIN32\ ${CMAKE_HOST_WIN32} )
#message( CMAKE_HOST_UNIX\ ${CMAKE_HOST_UNIX} )

# ----------------------------------------------
# -------- Generalities ------------------------
# ----------------------------------------------

cmake_minimum_required (VERSION 2.6)
project(pink)
INCLUDE (CheckIncludeFile)
INCLUDE (CheckIncludeFileCXX)
set( POTENCIAL_LIBDIRS /usr/lib /usr/local/lib /opt/local/lib /usr/lib64 /usr/local/lib64 /opt/local/lib64 )
set( INCLUDES include /usr/include /usr/local/include )

include_directories( ${INCLUDES} )


# --- Library extention (.so, .dylib, .a) ---

string( COMPARE EQUAL ${CMAKE_HOST_SYSTEM_NAME} Linux THIS_IS_LINUX )

if (THIS_IS_LINUX)
    set( LIBEXT so )
elseif (CMAKE_HOST_APPLE)
    set( LIBEXT dylib )
elseif (CMAKE_HOST_WIN32)
    set( LIBEXT a ) # extension for mingw
endif (THIS_IS_LINUX)


if (CMAKE_HOST_UNIX)

    option( BASH_FRONT_END "Compiles the executable files if you switch this of you can still compile the Python front-end and the shared library." ON )

    CHECK_INCLUDE_FILE( X11/Xlib.h  HTEST1 ) 
    if (NOT HTEST1) 
	message( FATAL_ERROR "error X11 header file X11/Xlib.h not found") 
    endif(NOT HTEST1)
	
    CHECK_INCLUDE_FILE( X11/Xutil.h  HTEST2 )
    if (NOT HTEST2)
	message( FATAL_ERROR "error X11 header file X11/Xutil.h not found")
    endif(NOT HTEST2)

    # documentation
    option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" ON )

    # unix defines
    add_definitions( -DUNIXIO -DPC )

    find_library( MATH_LIB libm.${LIBEXT} PATHS ${POTENCIAL_LIBDIRS} )
    set( LIBS ${MATH_LIB} )

    option( PYTHON_FRONT_END "Compile the python front-end and the cpp functions" OFF )

    if ( PYTHON_FRONT_END )

	SET(PYTHON_INCLUDE "/usr/include/python" CACHE STRING "The directory of the python headers.")
	SET(PYTHON_FLAG "-lpython2.6" CACHE STRING "The flag to use for the python library.")

	find_library( BOOST_PYTHON libboost_python.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	set( LIBS ${LIBS} ${BOOST_PYTHON} )
	find_library( BOOST_THREAD libboost_thread-mt.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	set( LIBS ${LIBS} ${BOOST_THREAD} )

	# looking for the python library in a given order
	set( CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} ${PYTHON_FLAG} )
	set( CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${INCLUDES} ${PYTHON_INCLUDE} )
			
	find_library( GSL_LIB libgsl.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	set( LIBS ${LIBS} ${GSL_LIB} )

	find_library( GSLCBLAS_LIB libgslcblas.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	set( LIBS ${LIBS} ${GSLCBLAS_LIB} )

	CHECK_INCLUDE_FILE( gsl/gsl_interp.h HTEST ) 
	if (NOT HTEST)  
	    message( FATAL_ERROR "gsl header files failed to compile") 
         endif(NOT HTEST)
	 
	 CHECK_INCLUDE_FILE_CXX( boost/shared_ptr.hpp  HTEST3 )
	 if (NOT HTEST3)
	     message( FATAL_ERROR "error boost/shared_ptr.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	 endif(NOT HTEST3)
		
	 CHECK_INCLUDE_FILE_CXX( boost/shared_array.hpp HTEST4 )
	 if (NOT HTEST4)
	     message( FATAL_ERROR "error boost/shared_array.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
         endif(NOT HTEST4)
	 
	 CHECK_INCLUDE_FILE_CXX( boost/python/list.hpp  HTEST5 )
	 if (NOT HTEST5)
	     message( FATAL_ERROR "error boost/python/list.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
         endif(NOT HTEST5)
			
	 CHECK_INCLUDE_FILE_CXX( boost/python/extract.hpp  HTEST5 )
	 if (NOT HTEST5)
	     message( FATAL_ERROR "error boost/python/extract.hpp does not compile. Check if its correctly installed. You may need a newer version or set up python paths correctly.")
	 endif(NOT HTEST5)
			    
	 CHECK_INCLUDE_FILE_CXX( boost/lexical_cast.hpp  HTEST7 )
	 if (NOT HTEST7)
	     message( FATAL_ERROR "error boost/lexical_cast.hpp does not compile. Check if its correctly installed. You may need a newer version or set up python paths correctly.")
         endif(NOT HTEST7)
	 
	 CHECK_INCLUDE_FILE_CXX( boost/thread.hpp  HTEST8 )
	 if (NOT HTEST8)
	     message( FATAL_ERROR "error boost/thread.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	 endif(NOT HTEST8)
				    
	 CHECK_INCLUDE_FILE_CXX( boost/algorithm/minmax_element.hpp  HTEST9 )
	 if (NOT HTEST9)
	     message( FATAL_ERROR "error boost/algorithm/minmax_element.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	 endif(NOT HTEST9)
					
	 CHECK_INCLUDE_FILE_CXX( boost/accumulators/accumulators.hpp  HTEST10)
	 if (NOT HTEST10)
	     message( FATAL_ERROR "error boost/accumulators/accumulators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	 endif(NOT HTEST10)

	 CHECK_INCLUDE_FILE_CXX( boost/accumulators/statistics.hpp HTEST11 )
	 if (NOT HTEST11)
	     message( FATAL_ERROR "error boost/accumulators/statistics.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	 endif(NOT HTEST11)
    endif ( PYTHON_FRONT_END )

endif (CMAKE_HOST_UNIX)




# -------- Libraries ---------------------------

find_package( X11 REQUIRED )
find_package( Threads REQUIRED )

set( LIBS ${LIBS} ${X11_LIBRARIES} )
set( LIBS ${LIBS} ${THREADS_LIBRARIES} )



# -------- The shared library ------------------

add_subdirectory( src/lib lib )

# -------- Bash front-end ----------------------

if (BASH_FRONT_END)
    add_subdirectory( src/com bash )
endif(BASH_FRONT_END)

# -------- Python front-end --------------------

if (CMAKE_HOST_UNIX)
    if ( HAVE_BOOST_LIBRARIES )
	add_subdirectory( src/python python )
    endif ( HAVE_BOOST_LIBRARIES )
endif (CMAKE_HOST_UNIX)

# ----------------------------------------------
# -------- Documentation -----------------------
# ----------------------------------------------

if (BUILD_DOCUMENTATION)
    add_subdirectory( doc doc )
endif(BUILD_DOCUMENTATION)















# LuM end of file
