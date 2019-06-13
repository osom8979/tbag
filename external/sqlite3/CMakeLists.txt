cmake_minimum_required (VERSION 2.8.8)

project (sqlite3)
add_library (sqlite3 sqlite3.c)

install (TARGETS sqlite3
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES sqlite3.h sqlite3ext.h
         DESTINATION include)

