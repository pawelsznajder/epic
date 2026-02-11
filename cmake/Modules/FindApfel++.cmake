# APFEL++

find_program(
	Apfel++_CONFIG apfelxx-config
	HINTS
	${Apfel++_HINT}
	REQUIRED
)

if (Apfel++_CONFIG)

  exec_program(${Apfel++_CONFIG}
    ARGS --version
    OUTPUT_VARIABLE Apfel++_VERSION
  )
  set(Apfel++_VERSION ${Apfel++_VERSION} CACHE STRING INTERNAL)

  exec_program(${Apfel++_CONFIG}
    ARGS --cppflags
    OUTPUT_VARIABLE Apfel++_CXX_FLAGS
  )
  set(Apfel++_CXX_FLAGS ${Apfel++_CXX_FLAGS} CACHE STRING INTERNAL)

  exec_program(${Apfel++_CONFIG}
    ARGS --libdir
    OUTPUT_VARIABLE Apfel++_LIB_DIR
  )
  set(Apfel++_LIB_DIR ${Apfel++_LIB_DIR} CACHE STRING INTERNAL)

  find_library(Apfel++_LIBRARIES
    NAMES apfelxx 
    PATHS ${Apfel++_LIB_DIR}
    NO_DEFAULT_PATH
  )

  exec_program(${Apfel++_CONFIG}
    ARGS --incdir
    OUTPUT_VARIABLE Apfel++_INCLUDE_DIR
  )
  set(Apfel++_INCLUDE_DIR ${Apfel++_INCLUDE_DIR} CACHE PATH INTERNAL)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Apfel++ REQUIRED_VARS Apfel++_LIBRARIES VERSION_VAR Apfel++_VERSION)

else(Apfel++_CONFIG)
  message(FATAL_ERROR "Apfel++ not found")
endif(Apfel++_CONFIG)
