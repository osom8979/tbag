## Find the libtbag-ui library.
#
# The following variables are optionally searched for defaults
#  Tbui_ROOT
#
# The following are set after configuration is done:
#  Tbui_FOUND
#  Tbui_INCLUDE_DIRS
#  Tbui_LIBRARIES

if (Tbui_FOUND)
    return ()
endif ()

set (__headers "libtbui/libtbui.h")
set (__libs    "tbui")

if (IS_DIRECTORY "$ENV{TBAG_HOME}")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set (__build_type_lower debug)
    else ()
        string (TOLOWER ${CMAKE_BUILD_TYPE} __build_type_lower)
    endif ()

    set (Tbui_ROOT_INCLUDE_PATHS "$ENV{TBAG_HOME}")
    set (Tbui_ROOT_LIBRARY_PATHS "$ENV{TBAG_HOME}/build"
                                 "$ENV{TBAG_HOME}/build-${__build_type_lower}"
                                 "$ENV{TBAG_HOME}/cmake-build-${__build_type_lower}")
endif ()

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

if (Tbui_FOUND)
    message (STATUS "Found Tbui: ${Tbui_LIBRARIES}")
endif ()

