project(director)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    utils actor
)

# # for test
add_executable(test_render_director test/test_render_director.cpp)
target_link_libraries(test_render_director gtest gtest_main director SDL2 SDL2_image render actor game)
add_test(NAME test_render_director COMMAND test_render_director)
