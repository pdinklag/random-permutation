add_executable(generate generate.cpp)
target_include_directories(generate PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(generate tlx-cmdline-parser)
