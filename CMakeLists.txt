cmake_minimum_required(VERSION 3.14)

project(gamo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# config.h.in is used to define macros related to project
configure_file(config.h.in ${CMAKE_BINARY_DIR}/config.h)
include_directories(${CMAKE_BINARY_DIR})

enable_testing()

# >>> third party libs >>>
# google test
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
add_subdirectory(third_party/googletest)

# spdlog
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE third_party/spdlog/include)

# eventpp
add_library(eventpp INTERFACE)
target_include_directories(eventpp INTERFACE third_party/eventpp/include)

# box2d
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(third_party/box2d)

# pybind11
add_subdirectory(third_party/pybind11)
include_directories(third_party/pybind11/include)

# clipper2
set(CLIPPER2_EXAMPLES OFF)
set(CLIPPER2_TESTS OFF)
add_subdirectory(third_party/clipper2/CPP)

# ConvexDecomposition
add_library(convexdecomposition INTERFACE)
target_include_directories(convexdecomposition INTERFACE third_party/convexdecomposition/src)
# <<< third party libs <<<

# cpp core
add_subdirectory(core)

# python bind target
# add_subdirectory(py/gamo/core/_core)
