# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- Bash front-end ------------
# ------------------------------------

#-lgsl -lgslcblas -lm -lX11 -lXext /usr/lib64/libboost_thread-il-mt-1_39.so.1.39.0
FILE( GLOB com_src_c *.c )
FILE( GLOB com_src_cpp *.cpp )
FILE( GLOB com_src_cxx *.cxx ) # cpp files that compile without boost

# -------- Iteration over the executables -------- 

if ( NOT HAVE_BOOST_LIBRARIES )
   set( com_src ${com_src_c} ${com_src_cxx} )
else ( NOT HAVE_BOOST_LIBRARIES )
   set( com_src ${com_src_c} ${com_src_cpp} ${com_src_cxx} )
endif ( NOT HAVE_BOOST_LIBRARIES )


foreach( q ${com_src} )
	 string( REGEX MATCH [^/]*.c[^.]*$ filename ${q} )
	 string( REGEX REPLACE .c[^.]*$ "" executable ${filename} )
	 add_executable( ${executable} ${q} )
	 add_dependencies( ${executable} pink )
	 target_link_libraries( ${executable} pink ${LIBS} )
endforeach(q)





















# LuM end of file
