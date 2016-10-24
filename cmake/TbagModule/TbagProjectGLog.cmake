#/// @file   TbagProjectGLog.cmake
#/// @brief  Google-glog project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT GLog_FOUND)
    message (WARNING "Not found Google-glog.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${GLog_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${GLog_LIBRARIES})

