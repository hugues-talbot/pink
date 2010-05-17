# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- The shared library --------
# ------------------------------------ 

FILE( GLOB lib_src_c *.c )
FILE( GLOB lib_src_cpp *.cpp )
FILE( GLOB lib_src_bdd ../bdd/*.c )
FILE( GLOB lib_src_cxx *.cxx )

if ( NOT HAVE_BOOST_LIBRARIES )
# message( info= ${lib_src_cxx} )
   add_library(pink SHARED ${lib_src_c} ${lib_src_bdd} ${lib_src_cxx} )
else ( NOT HAVE_BOOST_LIBRARIES )
   add_library(pink SHARED ${lib_src_c} ${lib_src_cpp} ${lib_src_bdd} ${lib_src_cxx} )
endif ( NOT HAVE_BOOST_LIBRARIES )

target_link_libraries( pink ${LIBS} )
























# LuM end of file
