#/// @file   TbagDebug.cmake
#/// @brief  Debugging utilities.
#/// @author zer0
#/// @date   2016-06-01

#/// Logging-like debugging message function.
#///
#/// @param __tag [in] tag name.
#/// @param ...   [in] message arguments.
#///
#/// @remarks
#///  Check the global variables:
#///  - TBAG_DEBUG_TAG: Regex conditions of print.
function (tbag_debug __tag)
    if (TBAG_DEBUG_TAG AND ("${__tag}" MATCHES "${TBAG_DEBUG_TAG}"))
        message (STATUS "@ [${__tag}] " ${ARGN})
    endif ()
endfunction ()

#/// Print variable.
#///
#/// @param __tag      [in] tag name.
#/// @param __variable [in] Variable name.
function (tbag_debug_variable __tag __variable)
    tbag_debug ("${__tag}" "${__variable}: ${${__variable}}")
endfunction ()

#/// Print debug message with list.
#///
#/// @param __tag [in] tag name.
#/// @param ...   [in] list arguments.
function (tbag_debug_list __tag)
    set (__list_index 0)
    foreach (__list_cursor ${ARGN})
        tbag_debug ("${__tag}" "#${__list_index} ${__list_cursor}")
        math (EXPR __list_index "${__list_index} + 1")
    endforeach ()
endfunction ()

