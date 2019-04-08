## Find the Live555 (OpenRTSP) library.
#
# The following variables are optionally searched for defaults
#  Live555_ROOT_INCLUDE_PATHS
#  Live555_ROOT_LIBRARY_PATHS
#  Live555_ROOT_INCLUDE_PATHS_ONLY
#  Live555_ROOT_LIBRARY_PATHS_ONLY
#
# The following are set after configuration is done:
#  Live555_FOUND
#  Live555_INCLUDE_DIRS
#  Live555_LIBRARIES

if (Live555_FOUND)
    return ()
endif ()

if (DEFINED Live555_ROOT_INCLUDE_PATHS)
    if (NOT DEFINED BasicUsageEnvironment_ROOT_INCLUDE_PATHS)
        set (BasicUsageEnvironment_ROOT_INCLUDE_PATHS ${Live555_ROOT_INCLUDE_PATHS})
    endif ()
    if (NOT DEFINED UsageEnvironment_ROOT_INCLUDE_PATHS)
        set (UsageEnvironment_ROOT_INCLUDE_PATHS ${Live555_ROOT_INCLUDE_PATHS})
    endif ()
    if (NOT DEFINED Groupsock_ROOT_INCLUDE_PATHS)
        set (Groupsock_ROOT_INCLUDE_PATHS ${Live555_ROOT_INCLUDE_PATHS})
    endif ()
    if (NOT DEFINED LiveMedia_ROOT_INCLUDE_PATHS)
        set (LiveMedia_ROOT_INCLUDE_PATHS ${Live555_ROOT_INCLUDE_PATHS})
    endif ()
endif ()

if (DEFINED Live555_ROOT_LIBRARY_PATHS)
    if (NOT DEFINED BasicUsageEnvironment_ROOT_LIBRARY_PATHS)
        set (BasicUsageEnvironment_ROOT_LIBRARY_PATHS ${Live555_ROOT_LIBRARY_PATHS})
    endif ()
    if (NOT DEFINED UsageEnvironment_ROOT_LIBRARY_PATHS)
        set (UsageEnvironment_ROOT_LIBRARY_PATHS ${Live555_ROOT_LIBRARY_PATHS})
    endif ()
    if (NOT DEFINED Groupsock_ROOT_LIBRARY_PATHS)
        set (Groupsock_ROOT_LIBRARY_PATHS ${Live555_ROOT_LIBRARY_PATHS})
    endif ()
    if (NOT DEFINED LiveMedia_ROOT_LIBRARY_PATHS)
        set (LiveMedia_ROOT_LIBRARY_PATHS ${Live555_ROOT_LIBRARY_PATHS})
    endif ()
endif ()

if (DEFINED Live555_ROOT_INCLUDE_PATHS_ONLY)
    if (NOT DEFINED BasicUsageEnvironment_ROOT_INCLUDE_PATHS_ONLY)
        set (BasicUsageEnvironment_ROOT_INCLUDE_PATHS_ONLY ${Live555_ROOT_INCLUDE_PATHS_ONLY})
    endif ()
    if (NOT DEFINED UsageEnvironment_ROOT_INCLUDE_PATHS_ONLY)
        set (UsageEnvironment_ROOT_INCLUDE_PATHS_ONLY ${Live555_ROOT_INCLUDE_PATHS_ONLY})
    endif ()
    if (NOT DEFINED Groupsock_ROOT_INCLUDE_PATHS_ONLY)
        set (Groupsock_ROOT_INCLUDE_PATHS_ONLY ${Live555_ROOT_INCLUDE_PATHS_ONLY})
    endif ()
    if (NOT DEFINED LiveMedia_ROOT_INCLUDE_PATHS_ONLY)
        set (LiveMedia_ROOT_INCLUDE_PATHS_ONLY ${Live555_ROOT_INCLUDE_PATHS_ONLY})
    endif ()
endif ()

if (DEFINED Live555_ROOT_LIBRARY_PATHS_ONLY)
    if (NOT DEFINED BasicUsageEnvironment_ROOT_LIBRARY_PATHS_ONLY)
        set (BasicUsageEnvironment_ROOT_LIBRARY_PATHS_ONLY ${Live555_ROOT_LIBRARY_PATHS_ONLY})
    endif ()
    if (NOT DEFINED UsageEnvironment_ROOT_LIBRARY_PATHS_ONLY)
        set (UsageEnvironment_ROOT_LIBRARY_PATHS_ONLY ${Live555_ROOT_LIBRARY_PATHS_ONLY})
    endif ()
    if (NOT DEFINED Groupsock_ROOT_LIBRARY_PATHS_ONLY)
        set (Groupsock_ROOT_LIBRARY_PATHS_ONLY ${Live555_ROOT_LIBRARY_PATHS_ONLY})
    endif ()
    if (NOT DEFINED LiveMedia_ROOT_LIBRARY_PATHS_ONLY)
        set (LiveMedia_ROOT_LIBRARY_PATHS_ONLY ${Live555_ROOT_LIBRARY_PATHS_ONLY})
    endif ()
endif ()

include (TbagSimpleFindLibrary)
tbag_simple_find_library (BasicUsageEnvironment BasicUsageEnvironment/BasicUsageEnvironment.hh BasicUsageEnvironment)
tbag_simple_find_library (UsageEnvironment UsageEnvironment/UsageEnvironment.hh UsageEnvironment)
tbag_simple_find_library (Groupsock groupsock/Groupsock.hh groupsock)
tbag_simple_find_library (LiveMedia liveMedia/liveMedia.hh liveMedia)

if (BasicUsageEnvironment_FOUND OR UsageEnvironment_FOUND OR Groupsock_FOUND OR LiveMedia_FOUND)
    set (Live555_FOUND ON)
else ()
    set (Live555_FOUND OFF)
endif ()

set (Live555_INCLUDE_DIRS)
set (Live555_LIBRARIES)
if (BasicUsageEnvironment_FOUND)
    list (APPEND Live555_INCLUDE_DIRS ${BasicUsageEnvironment_INCLUDE_DIRS}/BasicUsageEnvironment)
    list (APPEND Live555_LIBRARIES ${BasicUsageEnvironment_LIBRARIES})
endif ()
if (UsageEnvironment_FOUND)
    list (APPEND Live555_INCLUDE_DIRS ${UsageEnvironment_INCLUDE_DIRS}/UsageEnvironment)
    list (APPEND Live555_LIBRARIES ${UsageEnvironment_LIBRARIES})
endif ()
if (Groupsock_FOUND)
    list (APPEND Live555_INCLUDE_DIRS ${Groupsock_INCLUDE_DIRS}/groupsock)
    list (APPEND Live555_LIBRARIES ${Groupsock_LIBRARIES})
endif ()
if (LiveMedia_FOUND)
    list (APPEND Live555_INCLUDE_DIRS ${LiveMedia_INCLUDE_DIRS}/liveMedia)
    list (APPEND Live555_LIBRARIES ${LiveMedia_LIBRARIES})
endif ()

if (Live555_INCLUDE_DIRS)
    list (REMOVE_DUPLICATES Live555_INCLUDE_DIRS)
endif ()

mark_as_advanced (Live555_FOUND
                  Live555_LIBRARIES
                  Live555_INCLUDE_DIRS)

