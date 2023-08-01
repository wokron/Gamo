project(resource)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    event
)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_resource_base test/test_resource_base.cpp)
target_link_libraries(test_resource_base gtest gtest_main resource)
add_test(NAME test_resource_base COMMAND test_resource_base)
