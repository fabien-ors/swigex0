# Add 'build_tests' target for building all tests executable
add_custom_target(build_tests)

# Add 'check' target for building then running the tests
add_custom_target(check)

# Define ctest command # TODO : really needed ? (yaml use --target check)
#set(VERBOSE_TEST --verbose)
if (MSVC) # TODO use IS_MULTI_CONFIG
  # Need configuration to be tested
  set(CONFIG_TEST -C $<CONFIG>)
endif()
set(MY_CTEST_COMMAND ${CMAKE_CTEST_COMMAND} ${VERBOSE_TEST} ${CONFIG_TEST} --output-on-failure)

add_subdirectory(cpp)
set(CHECK_DEP check_cpp)

if (BUILD_PYTHON)
  add_subdirectory(py)
  list(APPEND CHECK_DEP check_py)
endif()

if (BUILD_R)
  add_subdirectory(r)
  list(APPEND CHECK_DEP check_r)
endif()

add_dependencies(check ${CHECK_DEP})
