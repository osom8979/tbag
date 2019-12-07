cmake_minimum_required (VERSION 2.8.8)

project (minizip C)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

add_library (minizip ioapi.c unzip.c zip.c)
add_dependencies (minizip zlib)
target_include_directories (minizip PRIVATE ${CMAKE_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})
target_link_libraries (minizip PRIVATE ${ZLIB_STATIC_LIB})

install (TARGETS minizip
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES ioapi.h iowin32.h unzip.h zip.h
         DESTINATION include)


