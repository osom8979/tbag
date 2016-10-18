#/// @file   TbagProjectWholeArchiveOn.cmake
#/// @brief  Enable whole-archive project module.
#/// @author zer0
#/// @date   2016-07-04

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-force_load") # Single file only.
    list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-all_load")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,--whole-archive")
endif ()

