project(utils)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_basic_type test/test_basic_type.cpp)
target_link_libraries(test_basic_type gtest gtest_main utils)
add_test(NAME test_basic_type COMMAND test_basic_type)
