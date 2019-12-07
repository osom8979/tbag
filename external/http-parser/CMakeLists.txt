cmake_minimum_required (VERSION 2.8.8)

project (demangle C)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

add_library (http_parser http_parser.c)
target_compile_definitions (http_parser PRIVATE -DDISABLE_TRIPLE_SLASH_IS_FAIL)
target_include_directories (http_parser PRIVATE "${CMAKE_SOURCE_DIR}")

install (TARGETS http_parser
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES http_parser.h
         DESTINATION include)

