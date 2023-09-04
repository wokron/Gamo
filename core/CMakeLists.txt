project(gamo_core)

# sub modules
add_subdirectory(render)
add_subdirectory(utils)
add_subdirectory(actor)
add_subdirectory(director)
add_subdirectory(game)
add_subdirectory(input)
add_subdirectory(event)
add_subdirectory(physics)
add_subdirectory(resource)
add_subdirectory(algo)

add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME}
    render
    utils
)
