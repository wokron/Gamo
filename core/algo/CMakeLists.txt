project(algo)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE utils convexdecomposition Clipper2)

add_executable(test_polygon_algo test/test_polygon_algo.cpp)
target_link_libraries(test_polygon_algo PRIVATE algo gtest gtest_main)
add_test(NAME test_polygon_algo COMMAND test_polygon_algo)
