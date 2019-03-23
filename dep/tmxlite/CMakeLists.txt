cmake_minimum_required(VERSION 3.0)
project(tmxlite)
SET(PROJECT_NAME tmxlite)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG_ -DTMXLITE_STATIC")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG -DTMXLITE_STATIC")
SET(CMAKE_DEBUG_POSTFIX -s-d)
SET(CMAKE_RELEASE_POSTFIX -s)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_SOURCE_DIR}/dep/miniz
                    ${CMAKE_SOURCE_DIR}/dep/pugixml/src)

#includes the list of source files in the src directory
SET(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_SRC ${PROJECT_DIR}/FreeFuncs.cpp
                ${PROJECT_DIR}/ImageLayer.cpp
                ${PROJECT_DIR}/Map.cpp
                ${PROJECT_DIR}/Object.cpp
                ${PROJECT_DIR}/ObjectGroup.cpp
                ${PROJECT_DIR}/Property.cpp
                ${PROJECT_DIR}/TileLayer.cpp
                ${PROJECT_DIR}/Tileset.cpp)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})

