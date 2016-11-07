## Find the libtbag library.
#
# The following variables are optionally searched for defaults
#  Tbag_ROOT
#
# The following are set after configuration is done:
#  Tbag_FOUND
#  Tbag_INCLUDE_DIRS
#  Tbag_LIBRARIES

if (Tbag_FOUND)
    return ()
endif ()

set (__headers "libtbag/config.h")
set (__libs    "tbag")

if (IS_DIRECTORY "$ENV{TBAG_HOME}")
    set (Tbag_ROOT_INCLUDE_PATHS "$ENV{TBAG_HOME}")
    set (Tbag_ROOT_LIBRARY_PATHS "$ENV{TBAG_HOME}/build")
endif ()

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

