cmake_minimum_required(VERSION 3.14)

project(gamo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
add_subdirectory(src)

# python bind target
# add_subdirectory(script/gamo/core)
