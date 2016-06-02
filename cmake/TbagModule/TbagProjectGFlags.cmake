#/// @file   TbagProjectGFlags.cmake
#/// @brief  Google-gflags project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT GFlags_FOUND)
    find_package (GFlags)
endif ()

if (GFlags_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GFLAGS)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${GFlags_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${GFlags_LIBRARIES})

    if (WIN32 AND NOT CYGWIN)
        list (APPEND TBAG_PROJECT_LDFLAGS "shlwapi.lib")
    endif ()
endif ()

