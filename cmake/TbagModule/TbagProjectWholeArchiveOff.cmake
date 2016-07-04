#/// @file   TbagProjectWholeArchiveOff.cmake
#/// @brief  Disable whole-archive project module.
#/// @author zer0
#/// @date   2016-07-04

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,--no-whole-archive")
endif ()

