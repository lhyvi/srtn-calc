install(
    TARGETS srtn-calc_exe
    RUNTIME COMPONENT srtn-calc_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
