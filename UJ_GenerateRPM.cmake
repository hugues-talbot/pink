# -*- sh -*-
# UjoImro, 2011
# This script generates an rpm source and build package

SET(PINK_MAJOR "1")
SET(PINK_MINOR "0")
if (NOT PINK_REVISION)
    SET(PINK_REVISION "0")
endif(NOT PINK_REVISION)

string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} TBZ2  TBZ2PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} ZIP   ZIPPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} TGZ   TGZPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-GENERIC  RPM-GENERICPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-GENERIC  DEB-GENERICPACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-DEBIAN4  DEB-DEBIAN4PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} DEB-DEBIAN6  DEB-DEBIAN6PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-FEDORA17      RPM-FEDORA17PACKAGE )
string( COMPARE EQUAL ${PINK_PACKAGE_TYPE} RPM-OpenSuSE12.1  RPM-OpenSuSE12.1PACKAGE )

if (TBZ2PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "TBZ2" )
elseif (ZIPPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "ZIP" )
elseif (TGZPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "TGZ" )
elseif (RPM-GENERICPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, cmake >= 2.6" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
elseif (DEB-GENERICPACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (DEB-DEBIAN4PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (DEB-DEBIAN6PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib" )
	SET( CPACK_GENERATOR              "DEB;TBZ2" )
elseif (RPM-FEDORA17PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, cmake >= 2.6" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
elseif (RPM-OpenSuSE12.1PACKAGE)
	SET( CPACK_RPM_PACKAGE_REQUIRES   "python >= 2.6.0, cmake >= 2.6" )
	SET( CPACK_DEBIAN_PACKAGE_DEPENDS "" )
	SET( CPACK_GENERATOR              "RPM;TBZ2" )
else()
	MESSAGE( FATAL "cmake configuration error: unknown package type" )
endif()
### Setting up CPack for the source package generation --------
### auto set SET(CPACK_GENERATOR                    "TBZ2;DEB" )
SET(CPACK_SOURCE_GENERATOR             "TBZ2;TGZ" )
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY  "Pink Image" )
SET(CPACK_PACKAGE_VENDOR               "ProCarPlan s.r.o." )
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/readme" )
SET(CPACK_RESOURCE_FILE_LICENSE        "${CMAKE_CURRENT_SOURCE_DIR}/CeCill_header.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR        ${PINK_MAJOR} )
SET(CPACK_PACKAGE_VERSION_MINOR        ${PINK_MINOR} )
SET(CPACK_PACKAGE_VERSION_PATCH        ${PINK_REVISION} )

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
"/tutorial/"
"/private/"
"~$"
"\\\\.svn"
"/TODO"
)

# **************************************************************
# *************************** RPM ******************************
# **************************************************************

### Special settings for RPM generation -----------------------
SET(CPACK_RPM_PACKAGE_SUMMARY          "Pink is an image processing library" )
SET(CPACK_RPM_PACKAGE_NAME             "pink-image" )
SET(CPACK_RPM_PACKAGE_VERSION          ${PINK_MAJOR}.${PINK_MINOR}.${PINK_REVISION} )
SET(CPACK_RPM_PACKAGE_ARCHITECTURE     "x86_64" )
SET(CPACK_RPM_PACKAGE_RELEASE 	       1 )
SET(CPACK_RPM_PACKAGE_LICENSE 	       "CeCILL" )
SET(CPACK_RPM_PACKAGE_GROUP 	       "Productivity/Scientific/Other" )
SET(CPACK_RPM_PACKAGE_VENDOR 	       "ProCarPlan s.r.o." )
SET(CPACK_RPM_PACKAGE_DESCRIPTION      "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license." )
SET(CPACK_RPM_PACKAGE_REQUIRES         "python >= 2.6.0, cmake >= 2.6")
SET(CPACK_RPM_PACKAGE_PROVIDES         "pink-image-1.0" )
#SET(CPACK_RPM_SPEC_INSTALL_POST 	May be used to set an RPM post-install command inside the spec file. For example setting it to "/bin/true" may be used to prevent rpmbuild to strip binaries (see [Bug7435])	-
#SET(CPACK_RPM_SPEC_MORE_DEFINE 	May be used to add any %define lines to the generated spec file.	-
#SET(CPACK_RPM_USER_BINARY_SPECFILE 	May be used to specify a user provided spec file instead of generating one. This is an feature which currently needs a patch see [Bug9679]	-
#SET(CPACK_RPM_GENERATE_USER_BINARY_SPECFILE_TEMPLATE 	May be used to generate a template for a user provided spec file. This is an feature which currently needs a patch see [Bug9679]	-
#SET(CPACK_RPM_<POST/PRE>_<UN>INSTALL_SCRIPT_FILE 	The content of the specified files will be embedded in the RPM spec file in the appropriate sections. This is an feature which currently needs a patch see [Bug8988] 	-
#SET(CPACK_RPM_PACKAGE_DEBUG 	May be set when invoking cpack in order to trace debug informations during CPack RPM run. For example you may launch CPack like this cpack -D CPACK_RPM_PACKAGE_DEBUG=1 -G RPM 	- 

# **************************************************************
# *************************** DEB ******************************
# **************************************************************

### Special settings for DEBIAN package generation -----------------------
SET( CPACK_DEBIAN_PACKAGE_NAME   "pink-image" )
SET( CPACK_DEBIAN_PACKAGE_VERSION ${PINK_MAJOR}.${PINK_MINOR}.${PINK_REVISION} )
SET( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
## autoset SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libgsl0ldbl, libncurses5, libboost-dev, liblapack3gf, libtiff4, python, python-imaging, python-imaging-tk, python-tk, python-numpy, python-matplotlib" )
SET( CPACK_DEBIAN_PACKAGE_MAINTAINER "Laszlo Marak <ujoimro@gmail.com>" )
SET( CPACK_DEBIAN_PACKAGE_DESCRIPTION "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license." )
SET( CPACK_DEBIAN_PACKAGE_SECTION "Productivity/Scientific/Other" )
SET( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
SET( CPACK_DEBIAN_PACKAGE_RECOMMENDS "ipython" )
SET( CPACK_DEBIAN_PACKAGE_SUGGESTS "python-vtk" )
#SET( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/prerm;" )



### Calling CPack ---------------------------------------------
INCLUDE(CPack)









# LuM end of file

