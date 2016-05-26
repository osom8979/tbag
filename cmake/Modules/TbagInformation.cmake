#/// @file   TbagInformation.cmake
#/// @brief  System information utilities.
#/// @author zer0
#/// @date   2016-05-26

#/// Print os information.
function (tbag_print_os)
    if (UNIX)
        # including Apple OS X and CygWin.
        message (STATUS "Unix-like OS's.")
    endif ()
    if (WIN32)
        # Prior to 2.8.4 this included CygWin.
        message (STATUS "Windows OS.")
    endif ()
    if (APPLE)
        message (STATUS "Apple systems.")
    endif ()
endfunction ()

#/// Print compiler information.
function (tbag_print_compiler)
    # Compiler & IDE:
    # CLANG GNU
    # MINGW MSYS CYGWIN
    # BORLAND WATCOM
    # MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10
    message (WARNING "Not implement.")
endfunction ()

#/// Print your compiler.
function (tbag_print_compiler_id)
    message (STATUS "Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

