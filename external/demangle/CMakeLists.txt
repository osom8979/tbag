cmake_minimum_required (VERSION 2.8.8)

project (demangle CXX)

add_library (demangle demangle.cpp)

install (TARGETS demangle
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (FILES demangle.h
         DESTINATION include)

