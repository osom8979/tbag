#/// @file   TbagFlags.cmake
#/// @brief  Flag helper.
#/// @author zer0
#/// @date   2016-05-26

#set (BUILD_SHARED_LIBS        ON) # User's variable.
#set (CMAKE_BUILD_TYPE    Release) # User's variable.
#set (CMAKE_MACOSX_RPATH        1) # CMP0042

#set (CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}")
#set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
#set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

#/// Assign whole-archive flags.
#///
#/// @param __result [out] value name of output list.
#/// @param ...      [in]  flag list.
function (tbag_flags__whole_archive __result)
    set (${__result})

    # Turn on.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND ${__result} "-Wl,-force_load")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND ${__result} "-Wl,--whole-archive")
    endif ()

    # Append arguments.
    foreach (__flag_cursor ${ARGN})
        list (APPEND ${__result} ${__flag_cursor})
    endforeach ()

    # Turn off.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND ${__result} "-Wl,--no-whole-archive")
    endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Append flags.
#///
#/// @param __value [in] Append value name.
#/// @param ...     [in] List of flags.
macro (tbag_flags__append __value)
    foreach (__flag_cursor ${ARGN})
        set (${__value} "${${__value}} ${__flag_cursor}")
    endforeach ()
endmacro ()

#/// Assign Position Independent Code.
macro (tbag_flags__set_position_independent_code)
    #if (CMAKE_VERSION VERSION_LESS "3.0.2")
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    #endif ()

    # Don't use this variables:
    set (CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC
endmacro ()

#/// Assign C++11 standard.
macro (tbag_flags__set_cpp_standard_11)
    #if (CMAKE_VERSION VERSION_LESS "3.1")
    ##if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CUDA_FOUND)
    ##    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    ##else ()
    ##    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ##endif ()
    set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    #endif ()

    # Don't use this variables:
    #set (CMAKE_CXX_STANDARD     11) # C++ standard 11
    #set (CMAKE_CXX_EXTENSIONS  OFF) # Don't change the -std=gnu++11 to -std=c++11
endmacro ()

#/// Assign default debug definition.
macro (tbag_flags__set_debug_define)
    # Build type: Debug, Release, RelWithDebInfo, MinSizeRel
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_definitions (-DDEBUG)
    else ()
        add_definitions (-DNDEBUG -DRELEASE)
    endif ()
endmacro ()

#/// Print all compile warning.
macro (tbag_flags__set_print_all_compile_warning)
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endmacro ()

#/// Setup RPATH variables.
macro (tbag_flags__set_install_rpath)
    set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)
    set (CMAKE_INSTALL_RPATH     "${CMAKE_INSTALL_PREFIX}/lib")
    set (CMAKE_INSTALL_NAME_DIR  "${CMAKE_INSTALL_PREFIX}/lib")
endmacro ()

#/// Default tbag project setup.
macro (tbag_flags__default_setup)
    tbag_flags__set_position_independent_code  ()
    tbag_flags__set_cpp_standard_11            ()
    tbag_flags__set_debug_define               ()
    #tbag_flags__set_print_all_compile_warning ()
    #tbag_flags__set_install_rpath             ()
endmacro ()

