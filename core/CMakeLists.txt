project(gamo)

file(GLOB_RECURSE DIR_SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} ${DIR_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC box2d Clipper2 SDL2 SDL2_image convexdecomposition eventpp spdlog)

add_subdirectory(test)
