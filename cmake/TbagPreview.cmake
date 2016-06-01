#/// @file   TbagPreview.cmake
#/// @brief  Preview cmake variables.
#/// @author zer0
#/// @date   2016-06-01

#/// Print information.
#///
#/// @param ... [in] message arguments.
function (tbag_preview__print)
    message ("## " ${ARGN})
endfunction ()

#/// Print variable.
#///
#/// @param __value [in] value name.
function (tbag_preview__print_variable __value)
    tbag_preview__print ("${__value}: " ${${__value}})
endfunction ()

#/// Print os information.
#///
#/// @remarks
#///  - ${WIN32}: Prior to 2.8.4 this included CygWin.
#///  - ${UNIX}: including Apple OS X and CygWin.
function (tbag_preview__print_os)
    if (WIN32)
        if (CYGWIN)
            tbag_preview__print ("Windows OS and Cygwin.")
        elseif (MSYS)
            tbag_preview__print ("Windows OS and MSYS.")
        else ()
            tbag_preview__print ("Windows OS.")
        endif ()
    elseif (UNIX)
        if (APPLE)
            tbag_preview__print ("Apple systems.")
        else ()
            tbag_preview__print ("Unix-like OS's.")
        endif ()
    else ()
        tbag_preview__print ("Unknown OS.")
    endif ()
endfunction ()

#/// Print compiler information.
function (tbag_preview__print_compiler)
    # Compiler & IDE:
    # CLANG GNU
    # MINGW MSYS CYGWIN
    # BORLAND WATCOM
    # MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10
    message (WARNING "Not implement.")
endfunction ()

#/// Print your compiler.
function (tbag_preview__print_compiler_id)
    tbag_preview__print ("Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

#/// Preview information.
function (tbag_preview)
    message ("")
    tbag_preview__print ("#################### PREVIEW ####################")
    tbag_preview__print ("")

    tbag_preview__print_os ()
    tbag_preview__print_compiler_id ()
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_BUILD_TYPE)
    tbag_preview__print_variable (BUILD_SHARED_LIBS)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_CXX_FLAGS)
    tbag_preview__print_variable (CMAKE_EXE_LINKER_FLAGS)
    tbag_preview__print_variable (CMAKE_MODULE_LINKER_FLAGS)
    tbag_preview__print_variable (CMAKE_SHARED_LINKER_FLAGS)
    tbag_preview__print ("")

    tbag_preview__print_variable (PROJECT_SOURCE_DIR)
    tbag_preview__print_variable (PROJECT_BINARY_DIR)
    tbag_preview__print_variable (CMAKE_INSTALL_PREFIX)
    message ("")
endfunction ()

