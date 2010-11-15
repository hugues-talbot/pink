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
INCLUDE (UJ_CheckIncludeFile.cmake)
INCLUDE (UJ_CheckIncludeFileCXX.cmake)

set( POTENCIAL_LIBDIRS /usr/lib /usr/local/lib /opt/local/lib /usr/lib64 /usr/local/lib64 /opt/local/lib64 /Library )
set( INCLUDES include src/python /usr/include /usr/local/include )

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

    CHECK_INCLUDE_FILE( curses.h  HTEST101 ) 
    if (NOT HTEST101) 
	message( FATAL_ERROR "error ncurses header file curses.h not found") 
    endif(NOT HTEST101)
	
    CHECK_INCLUDE_FILE( X11/Xutil.h  HTEST2 )
    if (NOT HTEST2)
	message( FATAL_ERROR "error X11 header file X11/Xutil.h not found")
    endif(NOT HTEST2)

    # documentation
    find_package(Doxygen)
    if ( NOT DOXYGEN_FOUND )
	option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" OFF )
    else ( NOT DOXYGEN_FOUND )
#	message( "not building documentation" )
	option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" ON )
    endif ( NOT DOXYGEN_FOUND )

    # unix defines
    add_definitions( -DUNIXIO -DPC )

    find_library( MATH_LIB libm.${LIBEXT} PATHS ${POTENCIAL_LIBDIRS} )
    set( LIBS ${MATH_LIB} )

    option( PYTHON_FRONT_END "Compile the python front-end and the cpp functions" OFF )

    if ( PYTHON_FRONT_END )

	find_package(PythonLibs)
	#MESSAGE( WARNING "PYTHONLIBS_FOUND=" ${PYTHONLIBS_FOUND} )
        #MESSAGE( WARNING "PYTHON_LIBRARIES=" ${PYTHON_LIBRARIES} )
        #MESSAGE( WARNING "PYTHON_INCLUDE_PATH=" ${PYTHON_INCLUDE_PATH} )
        #MESSAGE( WARNING "PYTHON_DEBUG_LIBRARIES=" ${PYTHON_DEBUG_LIBRARIES} )
	
	find_package(PythonInterp)
	#MESSAGE( WARNING "PYTHONINTERP_FOUND=" ${PYTHONINTERP_FOUND} )
        #MESSAGE( WARNING "PYTHON_EXECUTABLE=" ${PYTHON_EXECUTABLE} )


	if(NOT PYTHON_INCLUDE)
	    SET(PYTHON_INCLUDE ${PYTHON_INCLUDE_PATH} CACHE PATH "The directory of the python headers." )
	endif(NOT PYTHON_INCLUDE)

	if(NOT BOOST_INCLUDE)
	    SET(BOOST_INCLUDE "/usr/include" CACHE PATH "The directory of the boost headers. Remember in the case of /usr/local/include/boost, you specify /usr/local/include." )
	endif(NOT BOOST_INCLUDE)

	if(NOT PYTHON_LIBRARY)
	    SET(PYTHON_LIBRARY ${PYTHON_LIBRARIES} CACHE STRING "The flag to use for the python library.")
	endif(NOT PYTHON_LIBRARY)

# 	if(NOT EIGEN_INCLUDE)
# 	    SET(EIGEN_INCLUDE "/usr/include" CACHE PATH "The directory of the eigen2 headers. Remember in the case of /usr/local/include/eigen2, you specify /usr/local/include." )
# 	endif(NOT EIGEN_INCLUDE)

	find_package(Boost COMPONENTS python thread )

# 	MESSAGE( "Boost_FOUND=" ${Boost_FOUND} )
# 	MESSAGE( "Boost_INCLUDE_DIRS=" ${Boost_INCLUDE_DIRS} )
# 	MESSAGE( "Boost_LIBRARIES=" ${Boost_LIBRARIES} )
# 	MESSAGE( "Boost_LIBRARY_DIRS=" ${Boost_LIBRARY_DIRS} )
# 	MESSAGE( "Boost_VERSION=" ${Boost_VERSION} )
# 	MESSAGE( "Boost_LIB_VERSION=" ${Boost_LIB_VERSION} )
# 	MESSAGE( "Boost_LIB_DIAGNOSTIC_DEFINITIONS=" ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} )
# 	MESSAGE( "Boost_PYTHON_FOUND=" ${Boost_PYTHON_FOUND} )
# 	MESSAGE( "Boost_PYTHON_LIBRARY=" ${Boost_PYTHON_LIBRARY} )
# 	MESSAGE( "Boost_THREAD_FOUND=" ${Boost_THREAD_FOUND} )
# 	MESSAGE( "Boost_THREAD_LIBRARY=" ${Boost_THREAD_LIBRARY} )
# 	MESSAGE( "Boost_MAJOR_VERSION=" ${Boost_MAJOR_VERSION} )
# 	MESSAGE( "Boost_MINOR_VERSION=" ${Boost_MINOR_VERSION} )

	# required version of BOOST 1.35 or newer
	set( Boost_MAJOR_VERSION_REQUIRED 1 )
	set( Boost_MINOR_VERSION_REQUIRED 35 )

	if ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )
	    message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
	elseif ( Boost_MINOR_VERSION LESS Boost_MINOR_VERSION_REQUIRED )
	    message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
	endif ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )






	set( LIBS ${LIBS} ${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY} )

	# looking for the python library 
	set( CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} ${PYTHON_LIBRARY} ${LIBS}  )
	set( CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${INCLUDES} ${PYTHON_INCLUDE} ${BOOST_INCLUDE} ${EIGEN_INCLUDE} )
	include_directories( ${CMAKE_REQUIRED_INCLUDES} )

# 	find_library( GSL_LIB libgsl.${LIBEXT} ${POTENCIAL_LIBDIRS} )
# 	set( LIBS ${LIBS} ${GSL_LIB} )

# 	find_library( GSLCBLAS_LIB libgslcblas.${LIBEXT} ${POTENCIAL_LIBDIRS} )
# 	set( LIBS ${LIBS} ${GSLCBLAS_LIB} )

# 	find_library( LAPACK_LIB liblapack.${LIBEXT} ${POTENCIAL_LIBDIRS} )
# 	set( LIBS ${LIBS} ${LAPACK_LIB} )

	CHECK_INCLUDE_FILE( ${PYTHON_INCLUDE}/pyconfig.h HTEST )
	if (NOT HTEST)
	    message( ${PYTHON_INCLUDE}/pyconfig.h )
	    message( FATAL_ERROR " pyconfig.h header file failed to compile. If it is installed, than check the python flags and directories") 
        endif(NOT HTEST)

	CHECK_INCLUDE_FILE( curses.h HTEST )
	if (NOT HTEST)
	    message( FATAL_ERROR " curses.h header file failed to compile. If it is installed, than check the include directories") 
        endif(NOT HTEST)


# 	CHECK_INCLUDE_FILE( gsl/gsl_interp.h HTEST )
# 	if (NOT HTEST)
# 	    message( FATAL_ERROR "gsl header file failed to compile. If it is installed, than check the gsl flags") 
#         endif(NOT HTEST)

#	CHECK_INCLUDE_FILE( curses.h HTEST122 )
#	if (NOT HTEST)
#	    message( FATAL_ERROR "curses.h header file failed to compile. If it is installed, than check the python flags") 
#        endif(NOT HTEST122)
	 

	

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
					
# 	CHECK_INCLUDE_FILE_CXX( boost/accumulators/accumulators.hpp  HTEST10)
# 	if (NOT HTEST10)
# 	    message( FATAL_ERROR "error boost/accumulators/accumulators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
# 	endif(NOT HTEST10)

# 	CHECK_INCLUDE_FILE_CXX( boost/accumulators/statistics.hpp HTEST11 )
# 	if (NOT HTEST11)
# 	    message( FATAL_ERROR "error boost/accumulators/statistics.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
# 	endif(NOT HTEST11)

# 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/Core HTEST12 )
# 	if (NOT HTEST12)
# 	    message( FATAL_ERROR "error eigen2/Eigen/Core does not compile. Check if its correctly installed. You may need a newer version. ")
# 	endif(NOT HTEST12)

# 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/LU HTEST14 )
# 	if (NOT HTEST14)
# 	    message( FATAL_ERROR "error eigen2/Eigen/LU does not compile. Check if its correctly installed. You may need a newer version. ")
# 	endif(NOT HTEST14)

# 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/Geometry HTEST15 )
# 	if (NOT HTEST15)
# 	    message( FATAL_ERROR "error eigen2/Eigen/Geometry does not compile. Check if its correctly installed. You may need a newer version. ")
# 	endif(NOT HTEST15)


#eigen2/Eigen/Core
#eigen2/Eigen/LU
#eigen2/Eigen/Geometry


    endif ( PYTHON_FRONT_END )

endif (CMAKE_HOST_UNIX)




# -------- Libraries ---------------------------

find_package( X11 REQUIRED )
find_package( Threads REQUIRED )

set( LIBS ${LIBS} ${X11_LIBRARIES} )
set( LIBS ${LIBS} ${THREADS_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )



# -------- The shared library ------------------

add_subdirectory( src/lib lib )

# -------- Bash front-end ----------------------

if (BASH_FRONT_END)
    add_subdirectory( src/com bash )
endif(BASH_FRONT_END)

# -------- Python front-end --------------------

if (CMAKE_HOST_UNIX)
    if ( PYTHON_FRONT_END )
	add_subdirectory( src/python python )
    endif ( PYTHON_FRONT_END )
endif (CMAKE_HOST_UNIX)

# ----------------------------------------------
# -------- Documentation -----------------------
# ----------------------------------------------

if (BUILD_DOCUMENTATION)
    add_subdirectory( doc doc )
endif(BUILD_DOCUMENTATION)















# LuM end of file
