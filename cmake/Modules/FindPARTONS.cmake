# - Try to find PARTONS (both in system folders if it was installed, and in the current workspace)
#
# Once done this will define
#
#  PARTONS_FOUND - system has PARTONS lib
#  PARTONS_LIBRARIES - the path of the library
#  PARTONS_INCLUDE_DIR - the include directory

if (PARTONS_INCLUDE_DIR AND PARTONS_LIBRARIES)

  # in cache already
  set(PARTONS_FOUND TRUE)

else (PARTONS_INCLUDE_DIR AND PARTONS_LIBRARIES)

  find_path(PARTONS_INCLUDE_DIR NAMES partons/Partons.h
      PATHS
      ${CMAKE_INSTALL_PREFIX}
      ${CMAKE_SOURCE_DIR}/../PARTONS
      ${CMAKE_SOURCE_DIR}/../partons
      ${CMAKE_SOURCE_DIR}/PARTONS
      ${CMAKE_SOURCE_DIR}/partons
      ${CMAKE_SOURCE_DIR}
      PATH_SUFFIXES include/PARTONS include
    )
    
  find_library(PARTONS_LIBRARIES
                     NAMES PARTONS
                     PATHS
                     ${CMAKE_INSTALL_PREFIX}
                     ${CMAKE_SOURCE_DIR}/../PARTONS
                     ${CMAKE_SOURCE_DIR}/../partons
                     ${CMAKE_SOURCE_DIR}/PARTONS
                     ${CMAKE_SOURCE_DIR}/partons
                     ${CMAKE_SOURCE_DIR}
                     PATH_SUFFIXES lib/PARTONS lib64/PARTONS bin/PARTONS lib lib64 bin)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(PARTONS DEFAULT_MSG PARTONS_INCLUDE_DIR PARTONS_LIBRARIES)

endif(PARTONS_INCLUDE_DIR AND PARTONS_LIBRARIES)

