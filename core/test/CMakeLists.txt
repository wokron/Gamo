project(gamo_test)

# actor
add_executable(test_actor test_actor.cpp)
target_link_libraries(test_actor gtest gtest_main gamo)
add_test(NAME test_actor COMMAND test_actor)

add_executable(test_characteristic test_characteristic.cpp)
target_link_libraries(test_characteristic gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_characteristic COMMAND test_characteristic)

add_executable(test_behavior test_behavior.cpp)
target_link_libraries(test_behavior gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_behavior COMMAND test_behavior)

add_executable(test_physics test_physics.cpp)
target_link_libraries(test_physics gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_physics COMMAND test_physics)

add_executable(test_transform test_transform.cpp)
target_link_libraries(test_transform gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_transform COMMAND test_transform)

add_executable(test_tilemap test_tilemap.cpp)
target_link_libraries(test_tilemap gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_tilemap COMMAND test_tilemap)

add_executable(test_clone test_clone.cpp)
target_link_libraries(test_clone gtest gtest_main SDL2 SDL2_image gtest gtest_main gamo)
add_test(NAME test_clone COMMAND test_clone)

# algo
add_executable(test_polygon_algo test_polygon_algo.cpp)
target_link_libraries(test_polygon_algo PRIVATE gamo gtest gtest_main)
add_test(NAME test_polygon_algo COMMAND test_polygon_algo)

# director
add_executable(test_render_director test_render_director.cpp)
target_link_libraries(test_render_director gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_render_director COMMAND test_render_director)

# event
add_executable(test_event_dispatcher test_event_dispatcher.cpp)
target_link_libraries(test_event_dispatcher gtest gtest_main gamo)
add_test(NAME test_event_dispatcher COMMAND test_event_dispatcher)

# game
add_executable(test_play test_play.cpp)
target_link_libraries(test_play gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_play COMMAND test_play)

add_executable(test_scene test_scene.cpp)
target_link_libraries(test_scene gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_scene COMMAND test_scene)

add_executable(test_physics_in_play test_physics_in_play.cpp)
target_link_libraries(test_physics_in_play gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_physics_in_play COMMAND test_physics_in_play)

add_executable(test_mem_free test_mem_free.cpp)
target_link_libraries(test_mem_free gtest gtest_main gamo)
add_test(NAME test_mem_free COMMAND test_mem_free)

# input
add_executable(test_input test_input.cpp)
target_link_libraries(test_input gtest gtest_main SDL2 SDL2_image gamo spdlog)
add_test(NAME test_input COMMAND test_input)

# physics
add_executable(test_shape test_shape.cpp)
target_link_libraries(test_shape gtest gtest_main gamo)
add_test(NAME test_shape COMMAND test_shape)

# render
add_executable(test_texture test_texture.cpp)
target_link_libraries(test_texture gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_texture COMMAND test_texture)

add_executable(test_sprite test_sprite.cpp)
target_link_libraries(test_sprite gtest gtest_main SDL2 SDL2_image gamo)
add_test(NAME test_sprite COMMAND test_sprite)

# resource
add_executable(test_resource_base test_resource_base.cpp)
target_link_libraries(test_resource_base gtest gtest_main gamo)
add_test(NAME test_resource_base COMMAND test_resource_base)

# utils
add_executable(test_basic_type test_basic_type.cpp)
target_link_libraries(test_basic_type gtest gtest_main gamo)
add_test(NAME test_basic_type COMMAND test_basic_type)

add_executable(test_singleton_template test_singleton_template.cpp)
target_link_libraries(test_singleton_template gtest gtest_main gamo)
add_test(NAME test_singleton_template COMMAND test_singleton_template)

add_executable(test_prototype_template test_prototype_template.cpp)
target_link_libraries(test_prototype_template gtest gtest_main gamo)
add_test(NAME test_prototype_template COMMAND test_prototype_template)
