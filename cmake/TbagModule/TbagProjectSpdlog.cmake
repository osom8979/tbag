#/// @file   TbagProjectSpdlog.cmake
#/// @brief  speed-log project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT Spdlog_FOUND)
    message (WARNING "Not found speed-log.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Spdlog_INCLUDE_DIRS})

