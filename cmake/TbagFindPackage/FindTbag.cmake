## Find the libtbag library.
#
# The following variables are optionally searched for defaults
#  Tbag_ROOT
#
# The following are set after configuration is done:
#  Tbag_FOUND
#  Tbag_INCLUDE_DIRS
#  Tbag_LIBRARIES
#  Tbag_IS_STATIC

set (__headers "libtbag/config.h")
set (__libs    "tbag")

if (IS_DIRECTORY "$ENV{TBAG_HOME}")
    set (Tbag_ROOT_INCLUDE_PATHS "$ENV{TBAG_HOME}")
    set (Tbag_ROOT_LIBRARY_PATHS "$ENV{TBAG_HOME}/build")
endif ()

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

set (Tbag_IS_STATIC OFF)
if (Tbag_FOUND)
    foreach (__lib_cursor ${Tbag_LIBRARIES})
        if (${__lib_cursor} MATCHES ".*${CMAKE_STATIC_LIBRARY_SUFFIX}$")
            set (Tbag_IS_STATIC ON)
            break ()
        endif ()
    endforeach ()
endif ()

