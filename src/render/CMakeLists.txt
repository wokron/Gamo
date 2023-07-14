project(render)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    utils SDL2 SDL2_image
)

# for test
# test texture
add_executable(test_texture test/test_texture.cpp)
target_link_libraries(test_texture gtest gtest_main SDL2 SDL2_image render)
add_test(NAME test_texture COMMAND test_texture)

# test sprite
add_executable(test_sprite test/test_sprite.cpp)
target_link_libraries(test_sprite gtest gtest_main SDL2 SDL2_image render)
add_test(NAME test_sprite COMMAND test_sprite)
