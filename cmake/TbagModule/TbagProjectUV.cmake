#/// @file   TbagProjectUV.cmake
#/// @brief  libuv project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT UV_FOUND)
    message (WARNING "Not found libuv.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${UV_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${UV_LIBRARIES})

