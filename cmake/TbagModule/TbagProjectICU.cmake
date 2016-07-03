#/// @file   TbagProjectICU.cmake
#/// @brief  ICU project module.
#/// @author zer0
#/// @date   2016-07-03

if (NOT ICU_FOUND)
    message (WARNING "Not found ICU.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${ICU_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${ICU_LIBRARIES})

