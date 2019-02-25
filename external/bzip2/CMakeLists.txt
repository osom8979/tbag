cmake_minimum_required (VERSION 2.8.8)
project(bzip2)
add_definitions(-D_FILE_OFFSET_BITS=64)
set (SRCS blocksort.c huffman.c crctable.c randtable.c compress.c decompress.c bzlib.c)
add_library(cmbzip2 ${SRCS})

install (TARGETS cmbzip2
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES bzlib.h
         DESTINATION include)

