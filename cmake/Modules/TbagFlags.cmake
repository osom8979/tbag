#/// @file   TbagFlags.cmake
#/// @brief  Flag helper.
#/// @author zer0
#/// @date   2016-05-26

set (TBAG_FLAG_CLANG_WHOLE_ARCHIVE    "-Wl,-force_load")
set (TBAG_FLAG_GNU_ON_WHOLE_ARCHIVE   "-Wl,--whole-archive")
set (TBAG_FLAG_GNU_OFF_WHOLE_ARCHIVE  "-Wl,--no-whole-archive")

#/// Assign whole-archive flags.
#///
#/// @param __result [out] value name of output list.
#/// @param ...      [in]  flag list.
function (tbag_flags__whole_archive __result)
    set (${__result})

    # Turn on.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND ${__result} ${TBAG_FLAG_CLANG_WHOLE_ARCHIVE})
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND ${__result} ${TBAG_FLAG_GNU_ON_WHOLE_ARCHIVE})
    endif ()

    # Append arguments.
    foreach (__flag_cursor ${ARGN})
        list (APPEND ${__result} ${__flag_cursor})
    endforeach ()

    # Turn off.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND ${__result} ${TBAG_FLAG_GNU_OFF_WHOLE_ARCHIVE})
    endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Assign default debug definition.
function (tbag_flags__add_debug_define)
    # Build type: Debug, Release, RelWithDebInfo, MinSizeRel
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_definitions (-DDEBUG)
    else ()
        add_definitions (-DNDEBUG -DRELEASE)
    endif ()
endfunction ()

#/// Assign default debug definition.
function (tbag_flags__include_and_link_for_source_dir)
    include_directories (${PROJECT_SOURCE_DIR})
    link_directories    (${PROJECT_SOURCE_DIR})
endfunction ()

