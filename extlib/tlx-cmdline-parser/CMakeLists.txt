set(TLX_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tlx)
add_library(tlx-cmdline-parser ${TLX_SRC_DIR}/cmdline_parser.cpp ${TLX_SRC_DIR}/string/parse_si_iec_units.cpp)
target_include_directories(tlx-cmdline-parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
