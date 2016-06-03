#/// @file   TbagProjectSpdlog.cmake
#/// @brief  speed-log project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT Spdlog_FOUND)
    find_package (Spdlog)
endif ()

if (Spdlog_FOUND)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Spdlog_INCLUDE_DIRS})
endif ()

