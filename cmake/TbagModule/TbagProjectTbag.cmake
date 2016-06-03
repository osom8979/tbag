#/// @file   TbagProjectTbag.cmake
#/// @brief  libtbag project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT Tbag_FOUND)
    find_package (Tbag)
endif ()

if (Tbag_FOUND)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Tbag_INCLUDE_DIRS})
endif ()

