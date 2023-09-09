project(game)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC utils director render physics input resource spdlog
)

# for test
add_executable(test_play test/test_play.cpp)
target_link_libraries(test_play gtest gtest_main SDL2 SDL2_image game director actor)
add_test(NAME test_play COMMAND test_play)

add_executable(test_scene test/test_scene.cpp)
target_link_libraries(test_scene gtest gtest_main SDL2 SDL2_image game director actor)
add_test(NAME test_scene COMMAND test_scene)

add_executable(test_physics_in_play test/test_physics_in_play.cpp)
target_link_libraries(test_physics_in_play gtest gtest_main SDL2 SDL2_image game director actor input)
add_test(NAME test_physics_in_play COMMAND test_physics_in_play)

add_executable(test_mem_free test/test_mem_free.cpp)
target_link_libraries(test_mem_free gtest gtest_main game director actor input)
add_test(NAME test_mem_free COMMAND test_mem_free)
