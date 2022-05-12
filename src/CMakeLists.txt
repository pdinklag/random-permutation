add_library(random-permutation random_permutation.cpp)
target_include_directories(random-permutation PUBLIC ${SOURCE_DIR}/include)

add_executable(generate generate.cpp)
target_link_libraries(generate tlx-cmdline-parser random-permutation)
