if(NOT BUILD_PYTHON)
  return()
endif()

# Test sources
set(TEST_SOURCES testFibo.py)

# Look for Python3 (needed here because Python3_EXECUTABLE not populated everywhere)
find_package(Python3 REQUIRED)

# Generation folder
if (IS_MULTI_CONFIG)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR $<CONFIG>
             OUTPUT_VARIABLE TEST_DST_DIR)
else()
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${CMAKE_BUILD_TYPE}
             OUTPUT_VARIABLE TEST_DST_DIR)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TEST_DST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TEST_DST_DIR})

# Create the target and the output directory for logs
# Look parent CMakeLists for MY_CTEST_COMMAND definition
add_custom_target(check_py
                  COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_DST_DIR}"
                  COMMAND ${MY_CTEST_COMMAND})

# Add dependency for check_py
add_dependencies(check_py python_install)

# Python script test runner path
cmake_path(APPEND CMAKE_CURRENT_SOURCE_DIR run_test.py
           OUTPUT_VARIABLE RUN_TEST_SCRIPT)

# Display test output in case of failure
set(CTEST_OUTPUT_ON_FAILURE ON)

# Run each python scripts
foreach(TEST_SOURCE_FILE ${TEST_SOURCES})
    # Retrieve script file name without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE_FILE} NAME_WE)
    # Build the path to the python test script
    cmake_path(APPEND CMAKE_CURRENT_SOURCE_DIR ${TEST_SOURCE_FILE}
               OUTPUT_VARIABLE TEST_SOURCE_FILE)
    # Run the test (and generate log in ${TEST_DST_DIR}) (use a test runner script for stdout redirection)
    add_test(NAME ${TEST_NAME}_py
             COMMAND ${Python3_EXECUTABLE} ${RUN_TEST_SCRIPT} ${TEST_SOURCE_FILE} ${TEST_DST_DIR})
    # Compare the output result (diff output is shown when CTEST_OUTPUT_ON_FAILURE=1)
    # Use git diff (cross-platform) with no-index (no need to have files under git control) and ignore white spaces (-w)
    add_test(NAME ${TEST_NAME}_py_cmp
             COMMAND git diff --no-index -w ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.ref ${TEST_DST_DIR}/${TEST_NAME}.out)
endforeach(TEST_SOURCE_FILE ${TEST_SOURCES})

