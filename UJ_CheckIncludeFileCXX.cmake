# -*- sh -*-
# - Check if the include file exists.
#  CHECK_INCLUDE_FILE_CXX(INCLUDE VARIABLE)
#
#  INCLUDE  - name of include file
#  VARIABLE - variable to return result
#  
# An optional third argument is the CFlags to add to the compile line 
# or you can use CMAKE_REQUIRED_FLAGS.
#
# The following variables may be set before calling this macro to
# modify the way the check is run:
#
#  CMAKE_REQUIRED_FLAGS = string of compile command line flags
#  CMAKE_REQUIRED_DEFINITIONS = list of macros to define (-DFOO=bar)
#  CMAKE_REQUIRED_INCLUDES = list of include directories
#
MACRO(CHECK_INCLUDE_FILE_CXX INCLUDE VARIABLE)
#  IF("${VARIABLE}" MATCHES "^${VARIABLE}$")
    IF(CMAKE_REQUIRED_INCLUDES)
      SET(CHECK_INCLUDE_FILE_CXX_INCLUDE_DIRS "-DINCLUDE_DIRECTORIES=${CMAKE_REQUIRED_INCLUDES}")
    ELSE(CMAKE_REQUIRED_INCLUDES)
      SET(CHECK_INCLUDE_FILE_CXX_INCLUDE_DIRS)
    ENDIF(CMAKE_REQUIRED_INCLUDES)
#    MESSAGE("From within UJ-blabla-CXX: CMAKE_REQUIRED_FLAGS = " ${CMAKE_REQUIRED_FLAGS})
    SET(MACRO_CHECK_INCLUDE_FILE_FLAGS ${CMAKE_REQUIRED_FLAGS})
    SET(CHECK_INCLUDE_FILE_VAR ${INCLUDE})
    CONFIGURE_FILE(CheckIncludeFile.cxx.in
      ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/CheckIncludeFile.cxx IMMEDIATE)
    MESSAGE(STATUS "Looking for C++ include ${INCLUDE}")
    IF(${ARGC} EQUAL 3)
      SET(CMAKE_CXX_FLAGS_SAVE ${CMAKE_CXX_FLAGS})
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARGV2}")
    ENDIF(${ARGC} EQUAL 3)

    ## Hugues Talbot, reverse-engineering this stuff
    SET( CMAKE_FLAGS ${CMAKE_FLAGS} /LIBPATH:"C:/Program Files/Python/Python 2.7/x86_64/libs" )
    MESSAGE("from within UJ-xx-cxx: CMAKE_BINARY_DIR"= ${CMAKE_BINARY_DIR})
    MESSAGE("from within UJ-xx-cxx: CMAKE_REQUIRED_DEFINITIONS"= ${CMAKE_REQUIRED_DEFINITIONS})
    MESSAGE("from within UJ-xx-cxx: CMAKE_FLAGS"= ${CMAKE_FLAGS})
    MESSAGE("from within UJ-xx-cxx: MACRO_CHECK_INCLUDE_FILE_FLAGS"= ${MACRO_CHECK_INCLUDE_FILE_FLAGS})
    MESSAGE("from within UJ-xx-cxx: CHECK_INCLUDE_FILE_CXX_INCLUDE_DIRS"= ${CHECK_INCLUDE_FILE_CXX_INCLUDE_DIRS})

    # TRY_COMPILE(${VARIABLE}
    #   ${CMAKE_BINARY_DIR}
    #   ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/CheckIncludeFile.cxx
    #   COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
    #   CMAKE_FLAGS -DCMAKE_CXX_LINK_EXECUTABLE='echo not linking now...' -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_INCLUDE_FILE_FLAGS} "${CHECK_INCLUDE_FILE_CXX_INCLUDE_DIRS}" /link /LIBPATH:"C:\\Program Files\\Python\\Python 2.7\\x86_64\\libs"
    #   LINK_DIRECTORIES "C:/Program Files/Python/Python 2.7/x86_64/libs"
    #   LINK_LIBRARIES "C:/Program Files/Python/Python 2.7/x86_64/libs/python27.lib"
    #   OUTPUT_VARIABLE OUTPUT) 

    SET( MIDR "C:/Pink/Build/boost_1_52_0_python/boost_1_52_0_python_2.7_x86_64;C:/Pink/Build/eigen2/eigen-eigen-b23437e61a07" )
#    SET( MIDR ${MIDR} C:/Pink/Build/eigen2/eigen-eigen-b23437e61a07 )
    SET( MIDR C:/Pink/Build/eigen2/eigen-eigen-b23437e61a07 )

    TRY_COMPILE ( ${VARIABLE} 
	C:/Pink/Build/Pink/x86_64/release
	C:/Pink/Build/Pink/x86_64/release/CMakeFiles/CMakeTmp/CheckIncludeFile.cxx
	OUTPUT_VARIABLE OUTPUT
	CMAKE_FLAGS -DCMAKE_CXX_FLAGS:STRING="/IC:/Pink/Build/boost_1_52_0_python/boost_1_52_0_python_2.7_x86_64 /IC:/Pink/Build/eigen2/eigen-eigen-b23437e61a07"
    )

    IF(${ARGC} EQUAL 3)
      SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_SAVE})
    ENDIF(${ARGC} EQUAL 3)

    IF(${VARIABLE})
      MESSAGE(STATUS "Looking for C++ include ${INCLUDE} - found")
      SET(${VARIABLE} 1 CACHE INTERNAL "Have include ${INCLUDE}")
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log 
        "Determining if the include file ${INCLUDE} "
        "exists passed with the following output:\n"
        "${OUTPUT}\n\n")
    ELSE(${VARIABLE})
      MESSAGE(STATUS "Looking for C++ include ${INCLUDE} - not found")
      SET(${VARIABLE} "" CACHE INTERNAL "Have include ${INCLUDE}")
      FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log 
        "Determining if the include file ${INCLUDE} "
        "exists failed with the following output:\n"
        "${OUTPUT}\n\n")
    ENDIF(${VARIABLE})
#  ENDIF("${VARIABLE}" MATCHES "^${VARIABLE}$")
ENDMACRO(CHECK_INCLUDE_FILE_CXX)
