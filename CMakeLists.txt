##########################################################################################
#
#                      SWIGEX0 C++ Library and Wrappers
#
#                   Author: Fabien Ors (MINES Paris - PSL) 
#                   License: MIT
#                   Date: June 2023
#
#
# 1. Configure project :
#
#    C/C++ only:       cmake -Bbuild -S.
#    Python:           cmake -Bbuild -S. -DBUILD_PYTHON=ON
#    R:                cmake -Bbuild -S. -DBUILD_R=ON
#
#
# 2a. Build static library (and install Python package)
# 
#   GCC / MinGW =>     cmake --build build --target python_install
#   MSVC        =>     cmake --build build --target python_install --config Release
#
#
# 2b. Build and install shared library (and don't build Python package)
#
#   GCC / MinGW =>     cmake --build build --target install
#   MSVC        =>     cmake --build build --target install --config Release
#
#
# 3. Execute non-regression tests (C++, Python and R)
#
#   GCC / MinGW =>     cmake --build build --target check
#   MSVC        =>     cmake --build build --target check --config Release
#
#
# Note: See shortcuts for 'make' users in Makefile file
#

# For recent UseSWIG
cmake_minimum_required(VERSION 3.20)

# arm64 (Mac OS 11+) (put it before "project" instruction)
# TOO : this seems to have no impact!
#set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build arm64 architecture for Mac OS only" FORCE)

# Define project here
project(swigex0
        VERSION      0.1.9
        DESCRIPTION  "C++ library and SWIG wrappers (R & Python)"
        HOMEPAGE_URL "https://github.com/fabien-ors/swigex0"
        LANGUAGES    C CXX) # Enable C language for third party libraries

####################################################
## CONFIGURATION

# Get the project date
string(TIMESTAMP ${PROJECT_NAME}_DATE "%Y/%m/%d - %H:%M")

# Convert project name to uppercase
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UP)

# Fix version.h automatically
configure_file(version.h.in version.h)

# Detect presence of multi configuration generators
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

# Activate verbose mode
#set(CMAKE_VERBOSE_MAKEFILE ON)
# Debug find package instruction
#set(CMAKE_FIND_DEBUG_MODE TRUE)

# Options
option(BUILD_PYTHON "Build Python package" OFF)
option(BUILD_R      "Build R package"      OFF)
if (MINGW)
  set(BUILD_PYTHON OFF)
endif()
if (MSVC)
  set(BUILD_R OFF)
endif()
message(STATUS "BUILD_PYTHON=" ${BUILD_PYTHON})
message(STATUS "BUILD_R="      ${BUILD_R})

####################################################
## BUILD & INSTALL

include(cmake/version.cmake)
include(cmake/cpp.cmake)
include(cmake/install.cmake)

####################################################
## PACKAGES

add_subdirectory(python)
add_subdirectory(r)

####################################################
## TESTING

# Add non-regression test target
include(CTest)
enable_testing()

add_subdirectory(tests)
