project(physics)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    box2d
)

# for test
# add_executable(test_input test/test_input.cpp)
# target_link_libraries(test_input gtest gtest_main SDL2 SDL2_image game render input)
# add_test(NAME test_input COMMAND test_input)
