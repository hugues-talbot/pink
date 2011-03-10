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

### **********************************************************************************************
### **********************************************************************************************
### *************************** Generalities *****************************************************
### **********************************************************************************************
### **********************************************************************************************

cmake_minimum_required (VERSION 2.6)
project(pink)
INCLUDE (UJ_CheckIncludeFile.cmake)
INCLUDE (UJ_CheckIncludeFileCXX.cmake)
#INCLUDE (FindCFITSIO.cmake)

set( POTENTIAL_LIBDIRS /usr/lib /usr/local/lib /opt/local/lib /usr/lib64 /usr/local/lib64 /opt/local/lib64 /Library )
set( INCLUDES include src/python /usr/include /usr/local/include )

include_directories( ${INCLUDES} )

## This does not work
### I didn't think of this myself (HT)
##set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
###message("1- CMAKE module path = " ${CMAKE_MODULE_PATH})
##set(EXTRA_MODULES "${pink_SOURCE_DIR}")
###message("2- EXTRA_MODULES = " ${EXTRA_MODULES})
##set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  "${EXTRA_MODULES}" )
##message("3- new CMAKE module path = " ${CMAKE_MODULE_PATH})

set(CFITSIO_DIR "${pink_SOURCE_DIR}")


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

### **********************************************************************************************
### **********************************************************************************************
### *************************** General Options **************************************************
### **********************************************************************************************
### **********************************************************************************************

    if(NOT BOOST_INCLUDEDIR)
	SET(BOOST_INCLUDEDIR "/usr/include" CACHE PATH "The directory of the boost headers. Remember in the case of /usr/local/include/boost, you specify /usr/local/include." )
    endif(NOT BOOST_INCLUDEDIR)

### **********************************************************************************************
### **********************************************************************************************
### *************************** Functions in development *****************************************
### **********************************************************************************************
### **********************************************************************************************

    option( DEVELOPMENT "Compiles additional functions which are not-yet part of the core library (Mostly because they require additional dependencies). " OFF )

    option( LIAR "Compiles additional functions adapted from the Library of Image Analysis Routines," OFF)

### **********************************************************************************************
### **********************************************************************************************
### *************************** Library **********************************************************
### **********************************************************************************************
### **********************************************************************************************

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

    CHECK_INCLUDE_FILE( curses.h HTEST122 )
    if (NOT HTEST122)
	message( FATAL_ERROR "curses.h header file failed to compile. If it is installed, than check the python flags") 
    endif(NOT HTEST122)

    # unix defines
    add_definitions( -DUNIXIO -DPC )

    find_library( MATH_LIB libm.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
    set( LIBS ${MATH_LIB} )


    
    ##
    ## We need the optional CFITSIO library for reading the 
    ## FITS image format, widely use in astronomy
    ## Hugues Talbot	 9 Mar 2011
    ##
    find_package(CFITSIO)

    if (HT_CFITSIO_FOUND)
	message("cfitsio found, library= " ${CFITSIO_LIBRARY})
	add_definitions(-DHAVE_FITS_LIB)
    else (HT_CFITSIO_FOUND)
	#message("cfitsio library not found, FITS handling disabled")
    endif (HT_CFITSIO_FOUND)


    #find_library( STDCPP_LIB libstdc++.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
    #set( LIBS ${STDCPP_LIB} )

   
    # Libraries required to compile the pink library
    find_package( X11 REQUIRED )
    find_package( Threads REQUIRED )

    set( LIBS ${LIBS} ${X11_LIBRARIES} )
    set( LIBS ${LIBS} ${THREADS_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

    ### 
    ### Option LIAR
    ###
    
    if (LIAR)
	## we need POSIX semaphores to communicate with imview
	CHECK_INCLUDE_FILE(semaphore.h SEMTEST)
	if (NOT SEMTEST)
	    message("POSIX semaphores not found")
	else (NOT SEMTEST)
	    add_definitions( -DHAVE_POSIX_IPC )
        endif(NOT SEMTEST)

	CHECK_INCLUDE_FILE(sys/sem.h SYSVTEST)
	if (NOT SYSVTEST)
	    message("SysV IPC not found")
	else (NOT SYSVTEST)
	    add_definitions(-DHAVE_SYSV_IPC)
        endif(NOT SYSVTEST)

	FIND_PACKAGE(TIFF)

	if (TIFF_FOUND)
	    #message("Found libtiff headers in ${TIFF_INCLUDE_DIR}, library at ${TIFF_LIBRARIES}")
	    ADD_DEFINITIONS(-DHAVE_TIFF_LIB)
	    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIR})
	    set( LIBS ${LIBS} ${TIFF_LIBRARIES} )
	else(TIFF_FOUND)
	    message( FATAL_ERROR "Libtiff not found. TIFF support will be disabled")
	endif(TIFF_FOUND)
  
	if (THIS_IS_LINUX)
		find_library( POSIX_REALTIME_LIB librt.${LIBEXT} PATHS ${POTENTIAL_LIBDIRS} )
		set( LIBS ${LIBS} ${POSIX_REALTIME_LIB} )
	endif (THIS_IS_LINUX)

  endif (LIAR)


    ### ******************************************************
    ### *************** Option Development *******************
    ### ******************************************************
    if (DEVELOPMENT)
	if(NOT EIGEN_INCLUDE)
 	    SET(EIGEN_INCLUDE "/usr/include" CACHE PATH "The directory of the eigen2 headers. Remember in the case of /usr/local/include/eigen2, you specify /usr/local/include." )
 	endif(NOT EIGEN_INCLUDE)

 	find_library( GSL_LIB libgsl.${LIBEXT} ${POTENTIAL_LIBDIRS} )
 	set( LIBS ${LIBS} ${GSL_LIB} )

 	find_library( GSLCBLAS_LIB libgslcblas.${LIBEXT} ${POTENTIAL_LIBDIRS} )
 	set( LIBS ${LIBS} ${GSLCBLAS_LIB} )

 	find_library( LAPACK_LIB liblapack.${LIBEXT} ${POTENTIAL_LIBDIRS} )
 	set( LIBS ${LIBS} ${LAPACK_LIB} )

 	CHECK_INCLUDE_FILE( gsl/gsl_interp.h HTEST )
 	if (NOT HTEST)
 	    message( FATAL_ERROR "gsl header file failed to compile. If it is installed, than check the gsl flags") 
         endif(NOT HTEST)

 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/Core HTEST12 )
 	if (NOT HTEST12)
 	    message( FATAL_ERROR "error eigen2/Eigen/Core does not compile. Check if its correctly installed. You may need a newer version. ")
 	endif(NOT HTEST12)

 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/LU HTEST14 )
 	if (NOT HTEST14)
 	    message( FATAL_ERROR "error eigen2/Eigen/LU does not compile. Check if its correctly installed. You may need a newer version. ")
 	endif(NOT HTEST14)

 	CHECK_INCLUDE_FILE_CXX( eigen2/Eigen/Geometry HTEST15 )
 	if (NOT HTEST15)
 	    message( FATAL_ERROR "error eigen2/Eigen/Geometry does not compile. Check if its correctly installed. You may need a newer version. ")
 	endif(NOT HTEST15)

    endif (DEVELOPMENT)



    add_subdirectory( src/lib lib )


### **********************************************************************************************
### **********************************************************************************************
### *************************** Bash front-end ***************************************************
### **********************************************************************************************
### **********************************************************************************************

    option( BASH_FRONT_END "Compiles the executable files if you switch this of you can still compile the Python front-end and the shared library." OFF )

    if (BASH_FRONT_END)
	add_subdirectory( src/com bash )
    endif(BASH_FRONT_END)


### **********************************************************************************************
### **********************************************************************************************
### ******************************** DOXYGEN Documentation ***************************************
### **********************************************************************************************
### **********************************************************************************************

    find_package(Doxygen)
    if ( NOT DOXYGEN_FOUND )
	option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" OFF )
    else ( NOT DOXYGEN_FOUND )
        #message( "not building documentation" )
	option( BUILD_DOCUMENTATION "Builds the documentation files with Doxygen" OFF )
    endif ( NOT DOXYGEN_FOUND )

    if (BUILD_DOCUMENTATION)
	add_subdirectory( doc doc )
    endif(BUILD_DOCUMENTATION)


### **********************************************************************************************
### **********************************************************************************************
### *************************** Python front-end *************************************************
### **********************************************************************************************
### **********************************************************************************************


    option( PYTHON_FRONT_END "Compile the python front-end and the cpp functions" OFF )

    if ( PYTHON_FRONT_END )

	find_package(PythonLibs)
	find_package(PythonInterp)

	if(NOT PYTHON_INCLUDE)
	    SET(PYTHON_INCLUDE ${PYTHON_INCLUDE_PATH} CACHE PATH "The directory of the python headers." )
	endif(NOT PYTHON_INCLUDE)

	if(NOT PYTHON_LIBRARY)
	    SET(PYTHON_LIBRARY ${PYTHON_LIBRARIES} CACHE STRING "The python library.")
	endif(NOT PYTHON_LIBRARY)

	find_package(Boost COMPONENTS python thread )

	# required version of BOOST 1.35 or newer
#	set( Boost_MAJOR_VERSION_REQUIRED 1 )
#	set( Boost_MINOR_VERSION_REQUIRED 37 )

#	if ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )
#	    message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
#	elseif ( Boost_MINOR_VERSION LESS Boost_MINOR_VERSION_REQUIRED )
#	    message( FATAL_ERROR "For the core components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
#	endif ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )

	set( LIBS ${LIBS} ${Boost_LIBRARIES} )

	# looking for the python library 
	set( CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} ${PYTHON_LIBRARY} ${LIBS} ${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY} )
	set( CMAKE_REQUIRED_INCLUDES  ${Boost_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES} ${INCLUDES} ${PYTHON_INCLUDE} ${EIGEN_INCLUDE} )
	include_directories( ${CMAKE_REQUIRED_INCLUDES} )


	CHECK_INCLUDE_FILE( ${PYTHON_INCLUDE}/pyconfig.h HTEST )
	if (NOT HTEST)
	    message( ${PYTHON_INCLUDE}/pyconfig.h )
	    message( FATAL_ERROR " pyconfig.h header file failed to compile. If it is installed, than check the python flags and directories") 
        endif(NOT HTEST)

	CHECK_INCLUDE_FILE( curses.h HTEST )
	if (NOT HTEST)
	    message( FATAL_ERROR " curses.h header file failed to compile. If it is installed, than check the include directories") 
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
 	    message( FATAL_ERROR "error boost/python/list.hpp does not compile. You need at least version 1.37 or newer. Check if its correctly installed. There is a known issue. If "
 		"PYTHON_LIBRARY='" ${PYTHON_LIBRARY} "' does not point to the correct location of python's libraries, please specify them manually"
		" or delete the file in question. (seen on OpenSuSE 11.2)" )
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

	CHECK_INCLUDE_FILE_CXX( boost/operators.hpp  HTEST921 )
	if (NOT HTEST921)
	    message( FATAL_ERROR "error boost/operators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
	endif(NOT HTEST921)


        ### ******************************************************
        ### *************** Option Development *******************
        ### ******************************************************
        if (DEVELOPMENT)

            # required version of BOOST 1.39 or newer
	    set( Boost_MAJOR_VERSION_REQUIRED 1 )
	    set( Boost_MINOR_VERSION_REQUIRED 39 )

	    if ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )
		message( FATAL_ERROR "For the development components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
	    elseif ( Boost_MINOR_VERSION LESS Boost_MINOR_VERSION_REQUIRED )
	        message( FATAL_ERROR "For the development components you need a boost version at least " ${Boost_MAJOR_VERSION_REQUIRED} "." ${Boost_MINOR_VERSION_REQUIRED} )
	    endif ( Boost_MAJOR_VERSION LESS Boost_MAJOR_VERSION_REQUIRED )


    	    CHECK_INCLUDE_FILE_CXX( boost/accumulators/accumulators.hpp  HTEST10)
 	    if (NOT HTEST10)
 		message( FATAL_ERROR "error boost/accumulators/accumulators.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
 	    endif(NOT HTEST10)

 	    CHECK_INCLUDE_FILE_CXX( boost/accumulators/statistics.hpp HTEST11 )
 	    if (NOT HTEST11)
 		message( FATAL_ERROR "error boost/accumulators/statistics.hpp does not compile. Check if its correctly installed. You may need a newer version. ")
 	    endif(NOT HTEST11)

	endif (DEVELOPMENT)




	add_subdirectory( src/python python )


    endif ( PYTHON_FRONT_END )
endif (CMAKE_HOST_UNIX)


















# LuM end of file
