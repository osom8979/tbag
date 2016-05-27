#/// @file   TbagInformation.cmake
#/// @brief  System information utilities.
#/// @author zer0
#/// @date   2016-05-26

#/// Print information.
#///
#/// @param ... [in] message arguments.
function (tbag_info__print)
    message ("## " ${ARGN})
endfunction ()

#/// Print variable.
#///
#/// @param __value [in] value name.
function (tbag_info__print_variable __value)
    tbag_info__print ("${__value}: " ${${__value}})
endfunction ()

#/// Print os information.
#///
#/// @remarks
#///  - ${WIN32}: Prior to 2.8.4 this included CygWin.
#///  - ${UNIX}: including Apple OS X and CygWin.
function (tbag_info__print_os)
    if (APPLE)
        tbag_info__print ("Apple systems.")
        return ()
    endif ()

    if (WIN32 AND CYGWIN)
        tbag_info__print ("Windows OS AND Cygwin.")
        return ()
    endif ()

    if (WIN32)
        tbag_info__print ("Windows OS.")
        return ()
    endif ()

    if (UNIX)
        tbag_info__print ("Unix-like OS's.")
        return ()
    endif ()

    tbag_info__print ("Unknown OS.")
endfunction ()

#/// Print compiler information.
function (tbag_info__print_compiler)
    # Compiler & IDE:
    # CLANG GNU
    # MINGW MSYS CYGWIN
    # BORLAND WATCOM
    # MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10
    message (WARNING "Not implement.")
endfunction ()

#/// Print your compiler.
function (tbag_info__print_compiler_id)
    tbag_info__print ("Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

#/// Preview information.
function (tbag_info__print_preview)
    message ("")
    tbag_info__print ("#################### PREVIEW INFORMATION ####################")
    tbag_info__print ("")

    tbag_info__print_os ()
    tbag_info__print_compiler_id ()
    tbag_info__print ("")

    tbag_info__print_variable (CMAKE_BUILD_TYPE)
    tbag_info__print_variable (BUILD_SHARED_LIBS)
    tbag_info__print ("")

    tbag_info__print_variable (CMAKE_CXX_FLAGS)
    tbag_info__print_variable (CMAKE_EXE_LINKER_FLAGS)
    tbag_info__print_variable (CMAKE_MODULE_LINKER_FLAGS)
    tbag_info__print_variable (CMAKE_SHARED_LINKER_FLAGS)
    tbag_info__print ("")

    tbag_info__print_variable (PROJECT_SOURCE_DIR)
    tbag_info__print_variable (PROJECT_BINARY_DIR)
    tbag_info__print_variable (CMAKE_INSTALL_PREFIX)
    message ("")
endfunction ()

