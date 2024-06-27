# Add 'build_tests' target for building all tests executable
add_custom_target(build_tests)

# Add 'check' target for building then running the tests
add_custom_target(check)

# Define ctest command
if (IS_MULTI_CONFIG)
  set(CONFIG_TEST -C $<CONFIG>)
endif()
set(MY_CTEST_COMMAND ${CMAKE_CTEST_COMMAND} ${VERBOSE_TEST} ${CONFIG_TEST} --output-on-failure)

set(CHECK_DEP "")

if (BUILD_PYTHON)
  add_subdirectory(py)
  list(APPEND CHECK_DEP check_py)
endif()

if (BUILD_R)
  add_subdirectory(r)
  list(APPEND CHECK_DEP check_r)
endif()

# include check_cpp last to be executed first (MSYS/R)...
add_subdirectory(cpp)
list(APPEND CHECK_DEP check_cpp)

add_dependencies(check ${CHECK_DEP})
