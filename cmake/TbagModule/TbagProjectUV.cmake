#/// @file   TbagProjectUV.cmake
#/// @brief  libuv project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT UV_FOUND)
    find_package (UV)
endif ()

if (UV_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_LIBUV)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${UV_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${UV_LIBRARIES})
endif ()

