if(NOT BUILD_PYTHON)
  return()
endif()

# Python package name
set(PYTHON_PACKAGE_NAME ${PROJECT_NAME})
# Beware : the real package name is the PROJECT_NAME (for better import)

###################################
# DEPENDENCIES

# Look for Python3
#set(CMAKE_FIND_DEBUG_MODE TRUE)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module NumPy)
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)"
  Python3_NumPy_VERSION_MATCH ${Python3_NumPy_VERSION})
set(Python3_NumPy_VERSION_MAJOR ${CMAKE_MATCH_1})
math(EXPR Python3_NumPy_NEXT_MAJOR "${Python3_NumPy_VERSION_MAJOR} + 1")

# Look for SWIG 4 [No need for customized swig]
find_package(SWIG 4.0 REQUIRED)
include(${SWIG_USE_FILE})

######################################
# WRAPPER GENERATION & BUILD

# Python package interface file
set(SRC pyswigex0.i) 

# Generation folders
if (IS_MULTI_CONFIG)
  set(PYTHON_PACKAGE_ROOT_FOLDER      ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
else()
  set(PYTHON_PACKAGE_ROOT_FOLDER      ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif()
set(PYTHON_PACKAGE_DESTINATION_FOLDER ${PYTHON_PACKAGE_ROOT_FOLDER}/${PYTHON_PACKAGE_NAME})
# Location for SWIG target language code
set(CMAKE_SWIG_OUTDIR                 ${PYTHON_PACKAGE_DESTINATION_FOLDER})
# Location for SWIG wrapper library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY    ${PYTHON_PACKAGE_DESTINATION_FOLDER}) # Use LIBRARY (gcc)!
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY    ${PYTHON_PACKAGE_DESTINATION_FOLDER}) # Use RUNTIME (msvc)!

# SWIG interface source file properties
set_source_files_properties(${SRC} PROPERTIES
  CPLUSPLUS ON
  # Swig wrapper will be regenerated if C++ headers are changed
  # https://stackoverflow.com/a/70375654/3952924 (CMake 3.20 minimum)
  USE_SWIG_DEPENDENCIES TRUE
  # C++ headers will be found by %import swig directive
  USE_TARGET_INCLUDE_DIRECTORIES TRUE
  # Python package name will be the name of the installed folder (no module)
  SWIG_MODULE_NAME __init__
)

# Tell to CMake that ${SRC} depends on some other files
set_property(SOURCE ${SRC} PROPERTY
  DEPENDS
    ${PROJECT_SOURCE_DIR}/swig/swig_exp.i
    ${PROJECT_SOURCE_DIR}/swig/swig_inc.i
)

# SWIG flags
set(CMAKE_SWIG_FLAGS -keyword) # Use -keyword option to activate kwargs
# Note: Use -E option for seeing SWIG preprocessor output

# SWIG target (generate wrapper & python script then build [so,pyd] library)
swig_add_library(
  python_build
  LANGUAGE   python
  TYPE       SHARED
  SOURCES    ${SRC}
)

if (APPLE)
  # To prevent 'import' from crashing under Mac (swig output suffix is .dynlib by default) 
  set_target_properties(python_build PROPERTIES SUFFIX ".so")
endif()

# Reuse shared library objects
target_sources(
  python_build
  PRIVATE
  $<TARGET_OBJECTS:${PROJECT_NAME}::shared>
)
target_include_directories(python_build
  PRIVATE
  $<TARGET_PROPERTY:${PROJECT_NAME}::shared,INCLUDE_DIRECTORIES>
)
target_link_libraries(python_build
  PRIVATE
  $<TARGET_PROPERTY:${PROJECT_NAME}::shared,LINK_LIBRARIES>
)

# Transmit the fact that we are linking to static ${PROJECT_NAME} library
set(COMP_FLAGS "-D${PROJECT_NAME_UP}_STATIC_DEFINE")
# SWIG generated code warnings
if (MSVC)
  set(COMP_FLAGS "${COMP_FLAGS} /wd4706 /wd4456 /wd4127 /wd4100")
else()
  set(COMP_FLAGS "${COMP_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
endif()

# Set some properties on SWIG target
set_target_properties(python_build PROPERTIES
  # Modify the generated library name
  OUTPUT_NAME ${PROJECT_NAME}
  # Compiler flags for the SWIG library
  COMPILE_FLAGS ${COMP_FLAGS}
)

# Find appropriate Python.h
target_include_directories(python_build PRIVATE ${Python3_INCLUDE_DIRS})

if (MSVC OR APPLE)
  # Link to python's libraries (needed for MSVC and MacOS)
  # Prefer using Python3::Module (which works for MacOS) in place of ${Python3_LIBRARIES}
  # https://gitlab.kitware.com/cmake/cmake/-/issues/17664
  target_link_libraries(python_build PRIVATE Python3::Module)
endif()


########################################
# PACKAGING (target independent)

# Generate setup.py automatically for each configuration
# First step: replace variables (@VAR@)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/setup.py.in
  @ONLY
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml.in
  @ONLY
)

# Second step: replace generator expression ($<>)
file(GENERATE
  OUTPUT ${PYTHON_PACKAGE_ROOT_FOLDER}/setup.py
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py.in
)
file(GENERATE
  OUTPUT ${PYTHON_PACKAGE_ROOT_FOLDER}/pyproject.toml
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml.in
)

# Generate version.py automatically for each configuration
# First step: replace variables (@VAR@)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.py.in 
  ${CMAKE_CURRENT_BINARY_DIR}/version.py.in
  @ONLY
)

# Second step: replace generator expression ($<>)
file(GENERATE
  OUTPUT ${PYTHON_PACKAGE_ROOT_FOLDER}/${PYTHON_PACKAGE_NAME}/version.py
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/version.py.in
)

# Generate README.md for each configuration
file(GENERATE
  OUTPUT ${PYTHON_PACKAGE_ROOT_FOLDER}/README.md
  INPUT  ${CMAKE_CURRENT_SOURCE_DIR}/README.md
)

######################################
# INSTALLATION

# Find for UV
find_program(UV NAMES uv REQUIRED DOC "Python package and project manager")

# Add a custom target for python package installation
# TODO: Do also installation each time README and below files are modified
add_custom_target(python_install
  COMMAND ${UV} pip install ${PYTHON_PACKAGE_ROOT_FOLDER}
  COMMENT "Installing python package"
  VERBATIM
)

# Tell to CMake that python package must be built before installation
add_dependencies(python_install python_build)

if(NOT DEFINED ENV{VIRTUAL_ENV})
  add_custom_target(python_create_venv
    COMMAND ${UV} venv
    COMMENT "Creating a new venv with uv"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  add_dependencies(python_install python_create_venv)
endif()
