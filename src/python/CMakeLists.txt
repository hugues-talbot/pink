# -*- sh -*-
# UjoImro, 2010

# ------------------------------------
# -------- The Python front-end ------
# ------------------------------------ 

FILE( GLOB py_src_cpp *.cpp )

if (DEVELOPMENT)
    FILE( GLOB py_src_development_cpp development/*.cpp )
endif (DEVELOPMENT)

add_library( cpp_pink SHARED ${py_src_cpp} ${py_src_development_cpp} )
add_dependencies( cpp_pink pink  )

target_link_libraries( cpp_pink pink ${LIBS} )

# unix defines
add_definitions( -DUNIXIO -DPC )
set( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_FLAG} )
include_directories( ${PYTHON_INCLUDE}  ${BOOST_INCLUDE} )
set( CMAKE_INCLUDE_DIRS_CONFIG ${CMAKE_REQUIRED_INCLUDES} )

if (NOT PYTHON_EXE)
    find_program(PYTHON_EXE ipython)
    if (NOT PYTHON_EXE)
	find_program(PYTHON_EXE python)
    endif (NOT PYTHON_EXE)
endif (NOT PYTHON_EXE)

if (NOT PYTHON_EXE)
    message( WARNING "python executable not found. If you have it installed than adjust the PYTHON_EXE variable. You still may be able to compile, but the python front-end will not work." )
endif (NOT PYTHON_EXE)



if (THIS_IS_LINUX)
add_custom_command( TARGET cpp_pink POST_BUILD 
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND rm -r -f `find ${CMAKE_BINARY_DIR}/python/ -type d -name ".svn"`
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# Generated automatically on"`date --iso-8601` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "LD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib" PYTHONPATH=${PYTHONPATH}:${CMAKE_BINARY_DIR}/python ${PYTHON_EXE} $$\\@  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )



elseif (CMAKE_HOST_APPLE)
add_custom_command( TARGET cpp_pink POST_BUILD 
                    COMMAND ln -sf ${CMAKE_BINARY_DIR}/python/libcpp_pink.dylib ${CMAKE_BINARY_DIR}/python/libcpp_pink.so
		    # .svn directories cause permission denied
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND rm -r -f `find ${CMAKE_BINARY_DIR}/python/ -type d -name ".svn"`
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
    		    COMMAND echo "\\# Generated automatically on"`date` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "DYLD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib" PYTHONPATH=${PYTHONPATH}:${CMAKE_BINARY_DIR}/python ${PYTHON_EXE}  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )



elseif (CMAKE_HOST_WIN32)
add_custom_command( TARGET cpp_pink POST_BUILD 
		    COMMAND cp -r ${CMAKE_SOURCE_DIR}/python/* ${CMAKE_BINARY_DIR}/python/
		    COMMAND echo "\\#\\!" /bin/bash > ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# UjoImro, 2010"  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "\\# Generated automatically on"`date --iso-8601` >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "" >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND echo "LD_LIBRARY_PATH=\\\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/python:${CMAKE_BINARY_DIR}/lib" PYTHONPATH=${PYTHONPATH}:${CMAKE_BINARY_DIR}/python ${PYTHON_EXE}  >> ${CMAKE_BINARY_DIR}/pink.sh
		    COMMAND chmod u+x ${CMAKE_BINARY_DIR}/pink.sh
		    )



endif (THIS_IS_LINUX)





















# LuM end of file
