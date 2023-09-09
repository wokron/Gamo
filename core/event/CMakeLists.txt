project(event)

# for library
aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC eventpp utils
)

# for test
add_executable(test_event_dispatcher test/test_event_dispatcher.cpp)
target_link_libraries(test_event_dispatcher gtest gtest_main event)
add_test(NAME test_event_dispatcher COMMAND test_event_dispatcher)