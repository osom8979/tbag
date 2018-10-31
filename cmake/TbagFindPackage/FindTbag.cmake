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
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set (__build_type_lower debug)
    else ()
        string (TOLOWER ${CMAKE_BUILD_TYPE} __build_type_lower)
    endif ()

    set (Tbag_ROOT_INCLUDE_PATHS "$ENV{TBAG_HOME}")
    set (Tbag_ROOT_LIBRARY_PATHS "$ENV{TBAG_HOME}/build"
                                 "$ENV{TBAG_HOME}/build-${__build_type_lower}"
                                 "$ENV{TBAG_HOME}/cmake-build-${__build_type_lower}")
endif ()

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

if (Tbag_FOUND)
    message (STATUS "Found Tbag: ${Tbag_LIBRARIES}")

    if (UNIX AND APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        # If you're building a 64 bit application on OSX which links directly or indirectly against LuaJIT,
        # you need to link your main executable with these flags:
        # Reference (Embedding LuaJIT): http://luajit.org/install.html
        set (__update_exe_flags -Wl,-pagezero_size,10000 -Wl,-image_base,100000000)
        message (STATUS "Update EXE linker flags (Embedding LuaJIT): ${__update_exe_flags}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${__update_exe_flags}")
        unset (__update_exe_flags)
    endif()
endif ()

