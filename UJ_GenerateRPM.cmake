# -*- sh -*-
# UjoImro, 2011
# This script generates an rpm source and build package


# **************************************************************
# ******************* This macro extracts the ******************
# ********************  svn revision number ********************
# **************************************************************
MACRO(Subversion_GET_REVISION dir variable)
EXECUTE_PROCESS(COMMAND ${SVN_EXECUTABLE} info ${dir}
    OUTPUT_VARIABLE ${variable}
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
    "\\2" ${variable} "${${variable}}"
)
ENDMACRO(Subversion_GET_REVISION)

### Getting the revision number -------------------------------
FIND_PROGRAM(SVN_EXECUTABLE svn
	DOC "subversion command line client")

if (SVN_EXECUTABLE)
    Subversion_GET_REVISION(${CMAKE_CURRENT_SOURCE_DIR} PINK_SVN_REVISION)
    #MESSAGE(STATUS "Source dir is ${CMAKE_CURRENT_SOURCE_DIR}")
    #MESSAGE(STATUS "Revision is ${PINK_SVN_REVISION}")
else (SVN_EXECUTABLE)
    SET(PINK_SVN_REVISION "_unknown")
endif(SVN_EXECUTABLE)


### Setting up CPack for the source package generation --------
SET(CPACK_GENERATOR                    "TBZ2")
SET(CPACK_SOURCE_GENERATOR             "TBZ2")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY  "Pink image")
SET(CPACK_PACKAGE_VENDOR               "Esiee Engineering")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/readme")
SET(CPACK_RESOURCE_FILE_LICENSE        "${CMAKE_CURRENT_SOURCE_DIR}/CeCill_header.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR        "0")
SET(CPACK_PACKAGE_VERSION_MINOR        "9")
SET(CPACK_PACKAGE_VERSION_PATCH        r${PINK_SVN_REVISION})

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
"/tutorial/"
"~$"
"\\\\.svn"
"/TODO"
)

# # **************************************************************
# # ******************* RPM DOES NOT WORK!!!! ********************
# # **************************************************************

# ### Special settings for RPM generation -----------------------
# SET(CPACK_RPM_PACKAGE_SUMMARY          "Pink is an image processing library" )
# SET(CPACK_RPM_PACKAGE_NAME             "pink-image" )
# SET(CPACK_RPM_PACKAGE_VERSION          "0.9"r${PINK_SVN_REVISION} )
# SET(CPACK_RPM_PACKAGE_ARCHITECTURE     "x86_64" )
# SET(CPACK_RPM_PACKAGE_RELEASE 	       1 )
# SET(CPACK_RPM_PACKAGE_LICENSE 	       "CeCILL" )
# SET(CPACK_RPM_PACKAGE_GROUP 	       "Productivity/Scientific/Other" )
# SET(CPACK_RPM_PACKAGE_VENDOR 	       "ESIEE Engineering")
# SET(CPACK_RPM_PACKAGE_DESCRIPTION      "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license." )
# SET(CPACK_RPM_PACKAGE_REQUIRES         "python >= 2.6.0, cmake >= 2.6")
# SET(CPACK_RPM_PACKAGE_PROVIDES         "pink-image-0.9" )
# #SET(CPACK_RPM_SPEC_INSTALL_POST 	May be used to set an RPM post-install command inside the spec file. For example setting it to "/bin/true" may be used to prevent rpmbuild to strip binaries (see [Bug7435])	-
# #SET(CPACK_RPM_SPEC_MORE_DEFINE 	May be used to add any %define lines to the generated spec file.	-
# #SET(CPACK_RPM_USER_BINARY_SPECFILE 	May be used to specify a user provided spec file instead of generating one. This is an feature which currently needs a patch see [Bug9679]	-
# #SET(CPACK_RPM_GENERATE_USER_BINARY_SPECFILE_TEMPLATE 	May be used to generate a template for a user provided spec file. This is an feature which currently needs a patch see [Bug9679]	-
# #SET(CPACK_RPM_<POST/PRE>_<UN>INSTALL_SCRIPT_FILE 	The content of the specified files will be embedded in the RPM spec file in the appropriate sections. This is an feature which currently needs a patch see [Bug8988] 	-
# #SET(CPACK_RPM_PACKAGE_DEBUG 	May be set when invoking cpack in order to trace debug informations during CPack RPM run. For example you may launch CPack like this cpack -D CPACK_RPM_PACKAGE_DEBUG=1 -G RPM 	- 


### Calling CPack ---------------------------------------------
INCLUDE(CPack)









# LuM end of file

