project(gamo_core_py)

include_directories("${CMAKE_SOURCE_DIR}/core")

aux_source_directory(. DIR_SRCS)

pybind11_add_module(${PROJECT_NAME} ${DIR_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC
    actor
    director
    event
    game
    input
    physics
    render
    utils
)
