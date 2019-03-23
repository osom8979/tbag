cmake_minimum_required(VERSION 2.8.12)

project(pugixml)

set(HEADERS src/pugixml.hpp src/pugiconfig.hpp)
set(SOURCES src/pugixml.cpp)

add_library(pugixml STATIC ${HEADERS} ${SOURCES})

## Export symbols for shared library builds
#if(BUILD_SHARED_LIBS AND MSVC)
#    target_compile_definitions(pugixml PRIVATE "PUGIXML_API=__declspec(dllexport)")
#endif()

# Enable C++11 long long for compilers that are capable of it
if(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} STRLESS 3.1 AND ";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_long_long_type;")
    target_compile_features(pugixml PUBLIC cxx_long_long_type)
endif()

#set_target_properties(pugixml PROPERTIES VERSION 1.9 SOVERSION 1)
#get_target_property(PUGIXML_VERSION_STRING pugixml VERSION)

