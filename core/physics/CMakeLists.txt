project(physics)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    box2d utils algo
)

# for test
add_executable(test_shape test/test_shape.cpp)
target_link_libraries(test_shape gtest gtest_main physics)
add_test(NAME test_shape COMMAND test_shape)
