
# This is a CMake example for Python

cmake_minimum_required(VERSION 2.7)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if (CMAKE_HOST_UNIX)
    add_definitions( -DUNIXIO -DPC -fPIC -std=c99 )
endif (CMAKE_HOST_UNIX)

if (THIS_IS_MSVC)
    add_definitions( -D_WINDOWS -DWIN32 )
endif(THIS_IS_MSVC)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/../src/liar/)

#include_directories(../include)
    
ADD_LIBRARY( pinklib STATIC
	     ../src/lib/mcimage.c
	     ../src/lib/mccodimage.c
             ../src/lib/lwshedtopo.c
             ../src/lib/ldilateros.c
	     ../src/lib/ldilateros3d.c
	     ../src/lib/lsym.c
             ../src/lib/lminima.c
             ../src/lib/lmaxima.c
	     ../src/lib/llabelextrema.c
             ../src/lib/lgeodesic.c
             ../src/lib/mcindic.c
             ../src/lib/mccomptree.c
	     ../src/lib/mcunionfind.c
             ../src/lib/mcfahsalembier.c
	     ../src/lib/mcfah.c
	     ../src/lib/mcfahpure.c
             ../src/lib/mclifo.c
             ../src/lib/mcrlifo.c
             ../src/lib/mcfifo.c
	     ../src/lib/mcliste.c
	     ../src/lib/lseuil.c
	     ../src/lib/ldist.c
	     ../src/lib/lcrop.c
	     ../src/lib/larith.c
	     ../src/lib/lborder.c
	     ../src/lib/lattribheight.c
	     ../src/lib/lattribarea.c
	     ../src/lib/lattribvol.c
	     ../src/lib/mcgeo.c
	     ../src/lib/lskeletons.c
	     ../src/lib/mctopo.c
	     ../src/lib/mctopo3d.c
	     ../src/lib/mcrbt.c
	     ../src/lib/lseltopo.c
	     ../src/lib/mckhalimsky3d.c
	     ../src/bdd/bdd1.alphacube.c
	     ../src/bdd/bdd2.alpha.c
	     ../src/bdd/bdd2.beta.c
	     ../src/bdd/bdd3.c
	     ../src/lib/jcimage.c
	     ../src/lib/jccodimage.c
	     ../src/lib/lderiche.c
	     ../src/lib/lppm2GA.c
	     ../src/lib/lMSF.c
	     ../src/lib/lga2khalimsky.c
	     ../src/lib/llabelextrema.c
	     ../src/lib/ldraw.c
	     ../src/lib/lbresen.c
	     ../src/lib/mcsort.c
	     ../src/lib/lhisto.c
	     ../src/lib/lconvert.c
	     ../src/lib/lhtkern.c
	     ../src/lib/lhtkern3d.c
	     ../src/lib/llambdakern.c
	     ../src/lib/lfiltreordre.c
	     ../src/lib/lfiltrestopo.c
#	     ../src/lib/liar_line.cpp
	     )

SET_SOURCE_FILES_PROPERTIES(pinkswig.i PROPERTIES C ON)
SET_SOURCE_FILES_PROPERTIES(pinkswg.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(pinkswig python pinkswig.i com/pinkswig.c)

SWIG_LINK_LIBRARIES(pinkswig ${PYTHON_LIBRARIES} pinklib)

SET_SOURCE_FILES_PROPERTIES(imview.i PROPERTIES C ON)
SET_SOURCE_FILES_PROPERTIES(imview.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(imview python imview.i
		       com/imview.c
		       ../src/liar/imclient.c
		       ../src/liar/liarwrap.c
	       )

SWIG_LINK_LIBRARIES(imview ${PYTHON_LIBRARIES})
