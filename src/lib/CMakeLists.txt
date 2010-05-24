# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- The shared library --------
# ------------------------------------ 

FILE( GLOB lib_src_c *.c )
FILE( GLOB lib_src_cpp *.cpp )
FILE( GLOB lib_src_bdd ../bdd/*.c )
FILE( GLOB lib_src_cxx *.cxx )

# unix defines
add_definitions( -DUNIXIO -DPC )
set( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_FLAG} )
set( LINK_FLAGS ${LINK_FLAGS} ${PYTHON_FLAG} ${BOOST_INCLUDE} )
include_directories( ${PYTHON_INCLUDE} )


if ( NOT PYTHON_FRONT_END )
# message( info= ${lib_src_cxx} )
   add_library(pink SHARED ${lib_src_c} ${lib_src_bdd} ${lib_src_cxx} )
else ( NOT PYTHON_FRONT_END )
   add_library(pink SHARED ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_cxx} )
endif ( NOT PYTHON_FRONT_END )

target_link_libraries( pink ${LIBS} )

if (CMAKE_HOST_WIN32)
add_custom_command( TARGET pink POST_BUILD 
		    COMMAND cp ${CMAKE_BINARY_DIR}/lib/cygpink.dll ${CMAKE_BINARY_DIR}/bash/cygpink.dll
		    )
endif (CMAKE_HOST_WIN32)
install( TARGETS pink LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )





















# LuM end of file
