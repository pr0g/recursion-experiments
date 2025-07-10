cmake_minimum_required(VERSION 3.28)

project(third-party)

include(ExternalProject)

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig)
  if(NOT CMAKE_BUILD_TYPE)
    # set Debug as default build type if one is not provided (match CMake
    # default behavior)
    set(CMAKE_BUILD_TYPE
        Debug
        CACHE STRING "" FORCE)
  endif()
  # give different build types their own folder with single config generators
  set(build_type_dir ${CMAKE_BUILD_TYPE})
  # pass through build type arg to ExternalProject_Add command
  set(build_type_arg -DCMAKE_BUILD_TYPE=$<CONFIG>)
endif()

if(SUPERBUILD AND NOT PROJECT_IS_TOP_LEVEL)
  set(THIRD_PARTY_BINARY_DIR
      "${CMAKE_SOURCE_DIR}/build-third-party"
      CACHE STRING "Third-party build folder")
  if(NOT IS_ABSOLUTE ${THIRD_PARTY_BINARY_DIR})
    set(THIRD_PARTY_BINARY_DIR "${CMAKE_SOURCE_DIR}/${THIRD_PARTY_BINARY_DIR}")
  endif()
  set(PREFIX_DIR ${THIRD_PARTY_BINARY_DIR})
else()
  set(PREFIX_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

ExternalProject_Add(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
  GIT_SHALLOW TRUE
  PREFIX ${PREFIX_DIR}/SDL3
  BINARY_DIR ${PREFIX_DIR}/SDL3/build/${build_type_dir}
  INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install
  CMAKE_ARGS ${build_type_arg} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)

if(SUPERBUILD AND NOT PROJECT_IS_TOP_LEVEL)
  ExternalProject_Add(
    ${CMAKE_PROJECT_NAME}_superbuild
    DEPENDS SDL3
    SOURCE_DIR ${CMAKE_SOURCE_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -DSUPERBUILD=OFF
               ${build_type_arg})
endif()
