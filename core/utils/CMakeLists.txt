project(utils)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_basic_type test/test_basic_type.cpp)
target_link_libraries(test_basic_type gtest gtest_main utils)
add_test(NAME test_basic_type COMMAND test_basic_type)

add_executable(test_singleton_template test/test_singleton_template.cpp)
target_link_libraries(test_singleton_template gtest gtest_main utils)
add_test(NAME test_singleton_template COMMAND test_singleton_template)

add_executable(test_prototype_template test/test_prototype_template.cpp)
target_link_libraries(test_prototype_template gtest gtest_main utils)
add_test(NAME test_prototype_template COMMAND test_prototype_template)
