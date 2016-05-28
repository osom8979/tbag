#/// @file   TbagDebug.cmake
#/// @brief  Debugging helper.
#/// @author zer0
#/// @date   2016-05-25

#/// Print debug message.
#///
#/// @param ... [in] message arguments.
function (tbag_debug__message)
    message (STATUS "@ " ${ARGN})
endfunction ()

#/// Print debug message with list.
#///
#/// @param ... [in] message arguments.
function (tbag_debug__message_with_list)
    foreach (__list_cursor ${ARGN})
        tbag_debug__message (${__list_cursor})
    endforeach ()
endfunction ()

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
        tbag_debug__message ("[${__tag}] ${ARGN}")
    endif ()
endfunction ()

#/// Print variable.
#///
#/// @param __tag      [in] tag name.
#/// @param __variable [in] Variable name.
function (tbag_debug__variable __tag __variable)
    tbag_debug ("${__tag}" "${__variable}: ${${__variable}}")
endfunction ()

