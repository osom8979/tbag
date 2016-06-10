#/// @file   TbagProjectTbag.cmake
#/// @brief  libtbag project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT Tbag_FOUND)
    message (WARNING "Not found Tbag.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Tbag_INCLUDE_DIRS})
#list(APPEND TBAG_PROJECT_LDFLAGS      ${Tbag_LIBRARIES})

