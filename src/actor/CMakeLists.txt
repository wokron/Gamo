project(actor)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    utils render event director
)

# for test
add_executable(test_actor test/test_actor.cpp)
target_link_libraries(test_actor gtest gtest_main SDL2 SDL2_image actor)
add_test(NAME test_actor COMMAND test_actor)

add_executable(test_characteristic test/test_characteristic.cpp)
target_link_libraries(test_characteristic gtest gtest_main SDL2 SDL2_image actor)
add_test(NAME test_characteristic COMMAND test_characteristic)

add_executable(test_behavior test/test_behavior.cpp)
target_link_libraries(test_behavior gtest gtest_main SDL2 SDL2_image actor event)
add_test(NAME test_behavior COMMAND test_behavior)
