# APFEL++
find_program(Apfel++_CONFIG apfelxx-config REQUIRED)
if (Apfel++_CONFIG)
  exec_program(${Apfel++_CONFIG}
    ARGS --cppflags
    OUTPUT_VARIABLE Apfel++_CXX_FLAGS
  )
  set(Apfel++_CXX_FLAGS ${Apfel++_CXX_FLAGS} CACHE STRING INTERNAL)
  exec_program(${Apfel++_CONFIG}
    ARGS --ldflags
    OUTPUT_VARIABLE Apfel++_LIBRARIES
  )
  set(Apfel++_LIBRARIES ${Apfel++_LIBRARIES} CACHE STRING INTERNAL)
endif(Apfel++_CONFIG)
