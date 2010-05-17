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
    add_definitions( -DUNIXIO -DPC )
    include_directories( include src/python /usr/include/python2.6 /opt/local/include /opt/local/include/python2.6 )
    set( POTENCIAL_LIBDIRS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /opt/local/lib64 /opt/local/lib )

# ----------------------------------------------
# -------- Options -----------------------------
# ----------------------------------------------

    option( HAVE_BOOST_LIBRARIES "Compile the python front-end and the cpp functions" OFF )

    if ( HAVE_BOOST_LIBRARIES )
	find_library( BOOST_PYTHON libboost_python.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	find_library( BOOST_THREAD libboost_thread-mt.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	find_library( PYTHON_LIB libpython2.6.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	find_library( GSL_LIB libgsl.${LIBEXT} ${POTENCIAL_LIBDIRS} )
	find_library( GSLCBLAS_LIB libgslcblas.${LIBEXT} ${POTENCIAL_LIBDIRS} )
    endif ( HAVE_BOOST_LIBRARIES )

endif (CMAKE_HOST_UNIX)

# -------- Libraries ---------------------------



find_package(X11)
find_package(Threads)

find_library( MATH_LIB libm.${LIBEXT} ${POTENCIAL_LIBDIRS} )

set( LIBS ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIB} ${X11_LIBRARIES} ${BOOST_PYTHON} ${PYTHON_LIB} ${GSL_LIB} ${GSLCBLAS_LIB} ${BOOST_THREAD} )

# -------- The shared library ------------------

add_subdirectory( src/lib lib )

# -------- Bash front-end ----------------------

add_subdirectory( src/com bash )

# -------- Python front-end --------------------

if (CMAKE_HOST_UNIX)
    if ( HAVE_BOOST_LIBRARIES )
	add_subdirectory( src/python python )
    endif ( HAVE_BOOST_LIBRARIES )
endif (CMAKE_HOST_UNIX)
















# LuM end of file
