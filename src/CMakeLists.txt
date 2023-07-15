cmake_minimum_required(VERSION 3.14)

project(gamo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(third_party/googletest-1.13.0)
include_directories(third_party/googletest-1.13.0/googletest/include)
include_directories(third_party/spdlog/include)

add_subdirectory(render)
add_subdirectory(utils)
add_subdirectory(actor)
add_subdirectory(director)
add_subdirectory(game)

add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME}
    render
    utils
)
