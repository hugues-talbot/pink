# -*- sh -*-
# UjoImro, 2010-2013

INCLUDE(CheckTypeSize)

# ------------------------------------
# -------- The shared library --------
# ------------------------------------
cmake_minimum_required(VERSION 2.8)
FILE( GLOB lib_src_c *.c )
FILE( GLOB lib_src_c_linux linux/*.c ) # these are the files which are specific to linux (for example they use XLib)
# we should use only one kind of extension for C++ please
FILE( GLOB lib_src_cpp *.cpp )
# however, for the time being, the .cxx files are those that do not depend on Boost.
FILE( GLOB lib_src_cxx *.cxx )
#message( FATAL_ERROR "lib_src_cxx = '" ${lib_src_cxx} "'" )
FILE( GLOB lib_src_bdd ../bdd/*.c )
#message(info="LIAR components"
## LIAR is no longer an option ## if (PYTHON_FRONT_END) # LIAR obsolete
FILE( GLOB lib_src_liar_c ../liar/*.c)
FILE( GLOB lib_src_liar_cpp ../liar/*.cpp)
    # for generic programming in C
include_directories( ../liar )
## LIAR is no longer an option ## endif(PYTHON_FRONT_END) # LIAR obsolete
#message(info="DEVEL components")
if (DEVELOPMENT)
    FILE( GLOB lib_development_c development/*.c )
    FILE( GLOB lib_development_cpp development/*.cpp )
    FILE( GLOB lib_development_cxx development/*.cxx )
endif (DEVELOPMENT)

# unix defines
set( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_FLAG} )
set( LINK_FLAGS ${LINK_FLAGS} ${PYTHON_FLAG} ${BOOST_INCLUDE} )
include_directories( ${PYTHON_INCLUDE} ${Boost_INCLUDE_DIR} ${EIGEN_INCLUDE} )

if ( NOT PYTHON_FRONT_END )
    if (THIS_IS_MSVC)
	add_library(pinkstatic STATIC ${lib_src_c} ${lib_src_cxx} ${lib_src_bdd} ${lib_src_liar_c} ${lib_src_liar_cpp} ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} )
    else (THIS_IS_MSVC)
	add_library(pink STATIC ${lib_src_c} ${lib_src_cxx} ${lib_src_bdd} ${lib_src_liar_c} ${lib_src_liar_cpp} ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} ${lib_src_c_linux} )
	add_library(pink SHARED ${lib_src_c} ${lib_src_cxx} ${lib_src_bdd} ${lib_src_liar_c} ${lib_src_liar_cpp} ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} ${lib_src_c_linux} )
    endif (THIS_IS_MSVC)
else ( NOT PYTHON_FRONT_END )
    if (THIS_IS_MSVC)
	add_library(pinkstatic STATIC ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_liar_c} ${lib_src_liar_cpp} ${lib_src_cxx}  ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} )
    else (THIS_IS_MSVC)
	add_library(pink SHARED ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_liar_c} ${lib_src_liar_cpp} ${lib_src_cxx}  ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx}  ${lib_src_c_linux} )
    endif (THIS_IS_MSVC)
endif ( NOT PYTHON_FRONT_END )

# message("LIBS in lib = " ${LIBS})

if (THIS_IS_MSVC)
    target_link_libraries( pinkstatic ${LIBS} )
else (THIS_IS_MSVC)
    target_link_libraries( pink ${LIBS} )
endif(THIS_IS_MSVC)

### build type (32 or 64bit)
CHECK_TYPE_SIZE (void* POINTER_SIZE) #4 if 32bit, 8 if 64bit.

string( COMPARE EQUAL "${POINTER_SIZE}" "8" THIS_IS_64bit )

if (NOT PINK_LIBDIR)
    if ( THIS_IS_64bit )
	set(PINK_LIBDIR "lib64" )
    else ( THIS_IS_64bit )
	set(PINK_LIBDIR "lib" )
    endif ( THIS_IS_64bit )
endif (NOT PINK_LIBDIR)

configure_file( ${CMAKE_SOURCE_DIR}/platform_specific.h.in ${CMAKE_BINARY_DIR}/platform_specific.h )

if (CMAKE_HOST_UNIX)
install(
    TARGETS pink LIBRARY
    DESTINATION ${PINK_LIBDIR}
    ARCHIVE DESTINATION ${PINK_LIBDIR}
)
install( 
    DIRECTORY ${CMAKE_SOURCE_DIR}/pink
    DESTINATION include
)
install(
    FILES ${CMAKE_BINARY_DIR}/platform_specific.h
    DESTINATION include/pink
)
if (PYTHON_FRONT_END)
install(
    FILES ${CMAKE_BINARY_DIR}/python_doc.h
    DESTINATION include/pink
)
endif (PYTHON_FRONT_END)
else (CMAKE_HOST_UNIX) ### on Windows(tm) static library does not have to be installed
    # if (THIS_IS_MSVC)
    # 	install(
    # 	    TARGETS pinkstatic LIBRARY
    # 	    ARCHIVE DESTINATION pink-image
    # 	    RUNTIME DESTINATION bin )
    # endif (THIS_IS_MSVC)
endif(CMAKE_HOST_UNIX)




















# LuM end of file
