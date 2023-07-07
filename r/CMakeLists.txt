if(NOT BUILD_R)
  return()
endif()

# R package name
set(R_PACKAGE_NAME ${PROJECT_NAME})
# Beware : the real package name is the PROJECT_NAME (for better import)

###################################
# DEPENDENCIES

# Look for R
#set(CMAKE_FIND_DEBUG_MODE TRUE)
include(root/FindR.cmake)
if(NOT R_FOUND)
  message(FATAL_ERROR "R not found")
endif()

# Look for SWIG 4.2.0 [customized]
find_package(SWIG 4.2.0 REQUIRED)

include(${SWIG_USE_FILE})

######################################
# WRAPPER GENERATION & BUILD

# R package interface file
set(SRC rswigex0.i)

# Generation folders
if (IS_MULTI_CONFIG)
  set(R_PACKAGE_ROOT_FOLDER        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
else()
  set(R_PACKAGE_ROOT_FOLDER        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif()
set(R_PACKAGE_DESTINATION_FOLDER   ${R_PACKAGE_ROOT_FOLDER}/${R_PACKAGE_NAME})
set(R_PACKAGE_R_FOLDER             ${R_PACKAGE_DESTINATION_FOLDER}/R)
set(R_PACKAGE_SRC_FOLDER           ${R_PACKAGE_DESTINATION_FOLDER}/src)

# Location for SWIG target language code
set(CMAKE_SWIG_OUTDIR                 ${R_PACKAGE_R_FOLDER})
# Location for SWIG wrapper library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY    ${R_PACKAGE_SRC_FOLDER}) # Use LIBRARY (gcc)!
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY    ${R_PACKAGE_SRC_FOLDER}) # Use RUNTIME (msvc)!

# SWIG interface source file properties
set_source_files_properties(${SRC} PROPERTIES
  CPLUSPLUS ON
  # Swig wrapper will be regenerated if C++ headers are changed
  # https://stackoverflow.com/a/70375654/3952924 (CMake 3.20 minimum)
  USE_SWIG_DEPENDENCIES TRUE
  # C++ headers will be found by %import swig directive
  USE_TARGET_INCLUDE_DIRECTORIES TRUE
)

# Tell to CMake that ${SRC} depends on some other files
set_property(SOURCE ${SRC} PROPERTY
  DEPENDS
    ${PROJECT_SOURCE_DIR}/swig/swig_exp.i
    ${PROJECT_SOURCE_DIR}/swig/swig_inc.i
)

# SWIG flags
#set(CMAKE_SWIG_FLAGS -debug-classes)
#set(CMAKE_SWIG_FLAGS -debug-tmsearch)
set(CMAKE_SWIG_FLAGS)
# Note: Cannot use -doxygen feature because all documentation is in C++ body files
# Note: Use -E option for seeing SWIG preprocessor output

# SWIG target (generate wrapper & R script then build [so,dll] library)
swig_add_library(
  r_build
  LANGUAGE   r
  TYPE       SHARED
  SOURCES    ${SRC}
)

if (APPLE)
  # To prevent 'import' from crashing under Mac (swig output suffix is .dynlib by default) 
  set_target_properties(r_build PROPERTIES SUFFIX ".so")
endif()

# Link to static library
target_link_libraries(r_build PUBLIC ${PROJECT_NAME}::static)

# Transmit the fact that we are linking to static ${PROJECT_NAME} library
set(COMP_FLAGS "-D${PROJECT_NAME_UP}_STATIC_DEFINE -Wno-unused-parameter -Wno-unused-variable -Wno-uninitialized -Wno-cast-function-type")

# Set some properties on SWIG target
set_target_properties(r_build PROPERTIES
  # Do not build R package by default
  EXCLUDE_FROM_ALL ON
  # Modify the generated library name
  OUTPUT_NAME ${PROJECT_NAME}
  # Compiler flags for the SWIG library
  COMPILE_FLAGS ${COMP_FLAGS}
)

# Find appropriate R.h
target_include_directories(r_build PRIVATE ${R_INCLUDE_DIRS})

# Link to R's libraries (needed for MSYS and MacOS)
target_link_libraries(r_build PRIVATE ${R_LIBRARIES})

########################################
# PACKAGING

# Generate NAMESPACE automatically for each configuration
# First step: replace variables (@VAR@)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/NAMESPACE.in 
  ${CMAKE_CURRENT_BINARY_DIR}/NAMESPACE.in
  @ONLY
)
# Second step: replace generator expression ($<>)
file(GENERATE
  OUTPUT ${R_PACKAGE_DESTINATION_FOLDER}/NAMESPACE
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/NAMESPACE.in
)

# Replace DESCRIPTION automatically for each configuration
# First step: replace variables (@VAR@)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION.in 
  ${CMAKE_CURRENT_BINARY_DIR}/DESCRIPTION.in
  @ONLY
)
# Second step: replace generator expression ($<>)
file(GENERATE
  OUTPUT ${R_PACKAGE_DESTINATION_FOLDER}/DESCRIPTION
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/DESCRIPTION.in
)

######################################
# INSTALLATION

# Add a custom target for R package installation
# TODO: Do also installation each time NAMESPACE.in or DESCRIPTION.in is modified
if (MINGW)
  set(ARCHIVE_FILE_NAME     "${PROJECT_NAME}_${PROJECT_VERSION}.zip")
  set(DST_ARCHIVE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}.zip")
else()
  if(APPLE)
    set(ARCHIVE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}.tgz")
    set(DST_ARCHIVE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}.tgz")
  else()
    set(ARCHIVE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}_R_x86_64-pc-linux-gnu.tar.gz")
    set(DST_ARCHIVE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}.tar.gz")
  endif()
endif()
add_custom_target(r_install
  COMMAND ${CMAKE_COMMAND} -E touch ${R_PACKAGE_SRC_FOLDER}/Makevars
  COMMAND ${CMAKE_COMMAND} -E touch ${R_PACKAGE_SRC_FOLDER}/Makevars.win
  COMMAND ${CMAKE_COMMAND} -E touch ${R_PACKAGE_SRC_FOLDER}/Makevars.ucrt
  COMMAND ${R_EXECUTABLE} CMD INSTALL --build --no-multiarch ${R_PACKAGE_DESTINATION_FOLDER}
  COMMAND ${CMAKE_COMMAND} -E copy ${ARCHIVE_FILE_NAME} ${R_PACKAGE_ROOT_FOLDER}/${DST_ARCHIVE_FILE_NAME}
  COMMENT "Installing R package"
  VERBATIM
)

# Add r_build target dependency to install target
add_dependencies(r_install r_build)


