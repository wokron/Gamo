project(gamo_core_py)

include_directories("${CMAKE_SOURCE_DIR}/core/utils/include")

pybind11_add_module(core_example core_example.cpp)
target_link_libraries(core_example PUBLIC utils)
