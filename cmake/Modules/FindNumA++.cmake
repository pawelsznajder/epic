# - Try to find NumA++ (both in system folders if it was installed, and in the current workspace)
#
# Once done this will define
#
#  NumA++_FOUND - system has NumA++ lib
#  NumA++_LIBRARIES - the path of the library
#  NumA++_INCLUDE_DIR - the include directory

if (NumA++_INCLUDE_DIR AND NumA++_LIBRARIES)

  # in cache already
  set(NumA++_FOUND TRUE)

else (NumA++_INCLUDE_DIR AND NumA++_LIBRARIES)

  find_path(NumA++_INCLUDE_DIR NAMES NumA/integration/Integrator.h NumA++/integration/Integrator.h
      PATHS
      ${CMAKE_INSTALL_PREFIX}
      ${CMAKE_SOURCE_DIR}/../numa
      ${CMAKE_SOURCE_DIR}/../NumA
      ${CMAKE_SOURCE_DIR}/../NumA++
      ${CMAKE_SOURCE_DIR}/numa
      ${CMAKE_SOURCE_DIR}/NumA
      ${CMAKE_SOURCE_DIR}/NumA++
      ${CMAKE_SOURCE_DIR}
      HINTS 
      ${NumA++_HINT}
      PATH_SUFFIXES include/PARTONS include
    )
    
  find_library(NumA++_LIBRARIES
                     NAMES NumA NumA++
      		     HINTS 
      		     ${NumA++_HINT}
                     PATHS
                     ${CMAKE_INSTALL_PREFIX}
                     ${CMAKE_SOURCE_DIR}/../NumA++
                     ${CMAKE_SOURCE_DIR}/../NumA
                     ${CMAKE_SOURCE_DIR}/../numa
                     ${CMAKE_SOURCE_DIR}/NumA++
                     ${CMAKE_SOURCE_DIR}/NumA
                     ${CMAKE_SOURCE_DIR}/numa
                     ${CMAKE_SOURCE_DIR}
                     PATH_SUFFIXES lib/PARTONS lib64/PARTONS bin/PARTONS lib lib64 bin)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(NumA++ DEFAULT_MSG NumA++_LIBRARIES)

endif(NumA++_INCLUDE_DIR AND NumA++_LIBRARIES)

