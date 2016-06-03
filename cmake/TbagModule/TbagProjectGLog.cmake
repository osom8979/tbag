#/// @file   TbagProjectGLog.cmake
#/// @brief  Google-glog project module.
#/// @author zer0
#/// @date   2016-06-02

if (WIN32)
    message (WARNING "Not supported Windows OS.")
endif ()

if (NOT GLog_FOUND)
    find_package (GLog)
endif ()

if (GLog_FOUND)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${GLog_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${GLog_LIBRARIES})
endif ()

