# -*- sh -*-
# UjoImro, 2010

INCLUDE(CheckTypeSize)

# ------------------------------------
# -------- The shared library --------
# ------------------------------------ 

FILE( GLOB lib_src_c *.c )
# we should use only one kind of extension for C++ please
FILE( GLOB lib_src_cpp *.cpp )
# however, for the time being, the .cxx files are those that do not depend on Boost.
FILE( GLOB lib_src_cxx *.cxx )
#message( FATAL_ERROR "lib_src_cxx = '" ${lib_src_cxx} "'" )
FILE( GLOB lib_src_bdd ../bdd/*.c )
#message(info="LIAR components"
if (PYTHON_FRONT_END) # LIAR obsolete
    FILE( GLOB lib_src_liar ../liar/*.c )
    # for generic programming in C
    include_directories( ../liar )
endif(PYTHON_FRONT_END) # LIAR obsolete
#message(info="DEVEL components")
if (DEVELOPMENT)
    FILE( GLOB lib_development_c development/*.c )
    FILE( GLOB lib_development_cpp development/*.cpp )
    FILE( GLOB lib_development_cxx development/*.cxx )
endif (DEVELOPMENT)


# unix defines
add_definitions( -DUNIXIO -DPC )
set( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_FLAG} )
set( LINK_FLAGS ${LINK_FLAGS} ${PYTHON_FLAG} ${BOOST_INCLUDE} )
include_directories( ${PYTHON_INCLUDE} )


if ( NOT PYTHON_FRONT_END )
    #message( info= ${lib_src_cxx} )
    #message( info= ${lib_src_cpp} )
    add_library(pink SHARED ${lib_src_c} ${lib_src_cxx} ${lib_src_bdd} ${lib_src_liar} ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} )
    #message( FATAL_ERROR "pink=" ${pink} )
else ( NOT PYTHON_FRONT_END )
    add_library(pink SHARED ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_liar} ${lib_src_cxx}  ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} )
    set( LIBS ${LIBS} ${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY} ${PYTHON_LIBRARY} )
endif ( NOT PYTHON_FRONT_END )

    #message( info= ${LIBS} )
target_link_libraries( pink ${LIBS} )

if (CMAKE_HOST_WIN32)
    add_custom_command( TARGET pink POST_BUILD 
	COMMAND cp ${CMAKE_BINARY_DIR}/lib/cygpink.dll ${CMAKE_BINARY_DIR}/bash/cygpink.dll
    )
endif (CMAKE_HOST_WIN32)

### build type (32 or 64bit)
CHECK_TYPE_SIZE (void* POINTER_SIZE) #4 if 32bit, 8 if 64bit.

string( COMPARE EQUAL ${POINTER_SIZE} "8" THIS_IS_64bit )

if ( THIS_IS_64bit )
    set(PINK_LIBDIR "lib64" )
else ( THIS_IS_64bit )
    set(PINK_LIBDIR "lib" )
endif ( THIS_IS_64bit )

install( 
    TARGETS pink LIBRARY 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${PINK_LIBDIR}
)





















# LuM end of file
