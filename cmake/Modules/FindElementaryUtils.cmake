# - Try to find ElementaryUtils (both in system folders if it was installed, and in the current workspace)
#
# Once done this will define
#
#  ElementaryUtils_FOUND - system has Elementary_Utils lib
#  ElementaryUtils_LIBRARIES - the path of the library
#  ElementaryUtils_INCLUDE_DIR - the include directory

if (ElementaryUtils_INCLUDE_DIR AND ElementaryUtils_LIBRARIES)

  # in cache already
  set(ElementaryUtils_FOUND TRUE)

else (ElementaryUtils_INCLUDE_DIR AND ElementaryUtils_LIBRARIES)

  find_path(ElementaryUtils_INCLUDE_DIR ElementaryUtils/PropertiesManager.h
      PATHS
      ${CMAKE_INSTALL_PREFIX}
      ${CMAKE_SOURCE_DIR}/../ElementaryUtils
      ${CMAKE_SOURCE_DIR}/ElementaryUtils
      ${CMAKE_SOURCE_DIR}/../elementary-utils
      ${CMAKE_SOURCE_DIR}/elementary-utils
      ${CMAKE_SOURCE_DIR}
      HINTS 
      ${ElementaryUtils_HINT}
      PATH_SUFFIXES include/PARTONS include
    )
    
  find_library(ElementaryUtils_LIBRARIES
                     NAMES ElementaryUtils
		     HINTS 
		     ${ElementaryUtils_HINT}
                     PATHS
                     ${CMAKE_INSTALL_PREFIX}
                     ${CMAKE_SOURCE_DIR}/../ElementaryUtils
                     ${CMAKE_SOURCE_DIR}/ElementaryUtils
                     ${CMAKE_SOURCE_DIR}/../elementary-utils
                     ${CMAKE_SOURCE_DIR}/elementary-utils
                     ${CMAKE_SOURCE_DIR}
                     PATH_SUFFIXES lib/PARTONS lib64/PARTONS bin/PARTONS lib lib64 bin)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(ElementaryUtils DEFAULT_MSG ElementaryUtils_LIBRARIES)

endif(ElementaryUtils_INCLUDE_DIR AND ElementaryUtils_LIBRARIES)

