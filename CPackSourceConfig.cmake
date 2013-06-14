# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. Example variables are:
#   CPACK_GENERATOR                     - Generator used to create package
#   CPACK_INSTALL_CMAKE_PROJECTS        - For each project (path, name, component)
#   CPACK_CMAKE_GENERATOR               - CMake Generator used for the projects
#   CPACK_INSTALL_COMMANDS              - Extra commands to install components
#   CPACK_INSTALL_DIRECTORIES           - Extra directories to install
#   CPACK_PACKAGE_DESCRIPTION_FILE      - Description file for the package
#   CPACK_PACKAGE_DESCRIPTION_SUMMARY   - Summary of the package
#   CPACK_PACKAGE_EXECUTABLES           - List of pairs of executables and labels
#   CPACK_PACKAGE_FILE_NAME             - Name of the package generated
#   CPACK_PACKAGE_ICON                  - Icon used for the package
#   CPACK_PACKAGE_INSTALL_DIRECTORY     - Name of directory for the installer
#   CPACK_PACKAGE_NAME                  - Package project name
#   CPACK_PACKAGE_VENDOR                - Package project vendor
#   CPACK_PACKAGE_VERSION               - Package project version
#   CPACK_PACKAGE_VERSION_MAJOR         - Package project version (major)
#   CPACK_PACKAGE_VERSION_MINOR         - Package project version (minor)
#   CPACK_PACKAGE_VERSION_PATCH         - Package project version (patch)

# There are certain generator specific ones

# NSIS Generator:
#   CPACK_PACKAGE_INSTALL_REGISTRY_KEY  - Name of the registry key for the installer
#   CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS - Extra commands used during uninstall
#   CPACK_NSIS_EXTRA_INSTALL_COMMANDS   - Extra commands used during install


SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_NSIS "")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "")
SET(CPACK_BINARY_STGZ "")
SET(CPACK_BINARY_TBZ2 "")
SET(CPACK_BINARY_TGZ "")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license.")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Laszlo Marak <ujoimro@gmail.com>")
SET(CPACK_DEBIAN_PACKAGE_NAME "pink-image")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "ipython")
SET(CPACK_DEBIAN_PACKAGE_SECTION "Productivity/Scientific/Other")
SET(CPACK_DEBIAN_PACKAGE_SUGGESTS "python-vtk")
SET(CPACK_DEBIAN_PACKAGE_VERSION "1.0.0")
SET(CPACK_GENERATOR "TBZ2;TGZ")
SET(CPACK_IGNORE_FILES "/tutorial/;/private/;~$;\\.svn;/TODO")
SET(CPACK_INSTALLED_DIRECTORIES "/home/merveilo/Projets/pink;/")
SET(CPACK_INSTALL_CMAKE_PROJECTS "")
SET(CPACK_INSTALL_PREFIX "/usr/local")
SET(CPACK_MODULE_PATH "")
SET(CPACK_NSIS_CONTACT "https://www.pinkhq.com")
SET(CPACK_NSIS_DISPLAY_NAME "Pink Image Processing Library")
SET(CPACK_NSIS_DISPLAY_NAME_SET "TRUE")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "Pink Image Processing Library")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/merveilo/Projets/pink/CPackConfig.cmake")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/opt/local/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pink Image")
SET(CPACK_PACKAGE_FILE_NAME "pink-image-1.0.0-Source")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "pink-image 1.0.0")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "pink-image 1.0.0")
SET(CPACK_PACKAGE_NAME "pink-image")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "ProCarPlan s.r.o.")
SET(CPACK_PACKAGE_VERSION "1.0.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RESOURCE_FILE_LICENSE "/home/merveilo/Projets/pink/CeCill_header.txt")
SET(CPACK_RESOURCE_FILE_README "/opt/local/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/opt/local/share/cmake-2.8/Templates/CPack.GenericWelcome.txt")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "Pink is an image processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most of the operators come from mathematical morphology, but it contains operators from different fields. It is a free software licensed under the  CeCILL license.")
SET(CPACK_RPM_PACKAGE_GROUP "Productivity/Scientific/Other")
SET(CPACK_RPM_PACKAGE_LICENSE "CeCILL")
SET(CPACK_RPM_PACKAGE_NAME "pink-image")
SET(CPACK_RPM_PACKAGE_PROVIDES "pink-image-1.0")
SET(CPACK_RPM_PACKAGE_RELEASE "1")
SET(CPACK_RPM_PACKAGE_REQUIRES "")
SET(CPACK_RPM_PACKAGE_SUMMARY "Pink is an image processing library")
SET(CPACK_RPM_PACKAGE_VENDOR "ProCarPlan s.r.o.")
SET(CPACK_RPM_PACKAGE_VERSION "1.0.0")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ")
SET(CPACK_SOURCE_IGNORE_FILES "/tutorial/;/private/;~$;\\.svn;/TODO")
SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "/home/merveilo/Projets/pink;/")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/merveilo/Projets/pink/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "pink-image-1.0.0-Source")
SET(CPACK_SOURCE_TBZ2 "")
SET(CPACK_SOURCE_TGZ "")
SET(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_SOURCE_TZ "")
SET(CPACK_SOURCE_ZIP "")
SET(CPACK_STRIP_FILES "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux-Source")
