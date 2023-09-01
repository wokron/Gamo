cmake_minimum_required(VERSION 3.14)

project(gamo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# config.h.in is used to define macros related to project
configure_file(config.h.in ${CMAKE_BINARY_DIR}/config.h)
include_directories(${CMAKE_BINARY_DIR})

enable_testing()

# third party libs
add_subdirectory(third_party/googletest)
include_directories(third_party/googletest/googletest/include)
include_directories(third_party/spdlog/include)
include_directories(third_party/eventpp/include)
add_subdirectory(third_party/box2d)
include_directories(third_party/box2d/include)
add_subdirectory(third_party/pybind11)
include_directories(third_party/pybind11/include)

# cpp core
add_subdirectory(core)

# python bind target
# add_subdirectory(py/gamo/core/_core)
