cmake_minimum_required(VERSION 2.8)
project(fccedm CXX)

set(FCCEDM_MAJOR_VERSION 1)
set(FCCEDM_MINOR_VERSION 0)
set(FCCEDM_PATCH_VERSION 0)
set(FCCEDM_VERSION
  ${FCCEDM_MAJOR_VERSION}.${FCCEDM_MINOR_VERSION}.${FCCEDM_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
#if(WIN32 AND NOT CYGWIN)
#  set(DEF_INSTALL_CMAKE_DIR CMake)
#else()
#  set(DEF_INSTALL_CMAKE_DIR lib/CMake/fccedm)
#endif()
set(DEF_INSTALL_CMAKE_DIR cmake)
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Set up C++11
set(CPP11FLAGS "-std=c++11")
if (${APPLE})
    set(CPP11FLAGS "-std=c++11\ -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPP11FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")


# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{ALBERS})
find_package(alberscore)
find_package(ROOT)

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   
  "${PROJECT_BINARY_DIR}"
  "${ROOT_INCLUDE_DIR}"
  "${ALBERSCORE_INCLUDE_DIRS}"
  )  

# Add sub-directories
add_subdirectory(datamodel)
add_subdirectory(utilities)
add_subdirectory(examples)

# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(TARGETS datamodel utilities fccedm-write fccedm-read fccedm-simplewrite
  FILE "${PROJECT_BINARY_DIR}/fccedmTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE fccedm)

# Create the fccedmConfig.cmake and fccedmConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(fccedmConfig.cmake.in
  "${PROJECT_BINARY_DIR}/fccedmConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${FCCEDM_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(fccedmConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/fccedmConfig.cmake" @ONLY)
# ... for both
configure_file(fccedmConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/fccedmConfigVersion.cmake" @ONLY)

# Install the fccedmConfig.cmake and fccedmConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/fccedmConfig.cmake"
  "${PROJECT_BINARY_DIR}/fccedmConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT fccedmTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
