cmake_minimum_required (VERSION 2.8.8)

project (lmdb)
add_library (lmdb mdb.c midl.c)

install (TARGETS lmdb
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES lmdb.h lmdb_export.h midl.h
         DESTINATION include)

