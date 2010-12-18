# -*- sh -*-
# UjoImro, 2010

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
    add_library(pink SHARED ${lib_src_c} ${lib_src_cxx} ${lib_src_bdd} ${lib_development_c} ${lib_development_cpp} ${lib_development_cxx} )
    #message( FATAL_ERROR "pink=" ${pink} )
else ( NOT PYTHON_FRONT_END )
    add_library(pink SHARED ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_cxx} )
    set( LIBS ${LIBS} ${Boost_PYTHON_LIBRARY} ${Boost_THREAD_LIBRARY} ${PYTHON_LIBRARY} )
endif ( NOT PYTHON_FRONT_END )

message( info= ${LIBS} )
target_link_libraries( pink ${LIBS} )

if (CMAKE_HOST_WIN32)
    add_custom_command( TARGET pink POST_BUILD 
	COMMAND cp ${CMAKE_BINARY_DIR}/lib/cygpink.dll ${CMAKE_BINARY_DIR}/bash/cygpink.dll
    )
endif (CMAKE_HOST_WIN32)
install( TARGETS pink LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )





















# LuM end of file
