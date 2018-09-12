#/// @file   TbagPreview.cmake
#/// @brief  Preview cmake variables.
#/// @author zer0
#/// @date   2016-06-01

set (TBAG_PREVIEW_FILE "${CMAKE_BINARY_DIR}/Preview.txt" CACHE STRING "Preview file path.")
option (TBAG_SAVE_PREVIEW_FILE "Save preview file?" TRUE)

#/// Print information.
#///
#/// @param ... [in] message arguments.
function (tbag_preview__print)
    set (__msg "## " ${ARGN})
    message (${__msg})
    if (TBAG_SAVE_PREVIEW_FILE)
        file (APPEND "${TBAG_PREVIEW_FILE}" ${__msg} "\n")
    endif ()
endfunction ()

#/// Print variable.
#///
#/// @param __value [in] value name.
function (tbag_preview__print_variable __value)
    tbag_preview__print ("${__value}: " ${${__value}})
endfunction ()

#/// Print variable with list.
#///
#/// @param __value [in] value name.
function (tbag_preview__print_variable_list __value)
    set (__list_string)
    set (__list_index 0)

    foreach (__list_cursor ${${__value}})
        if (__list_index GREATER 0)
            set (__list_string "${__list_string}, ")
        endif ()
        set (__list_string "${__list_string}${__list_cursor}")
        math (EXPR __list_index "${__list_index} + 1")
    endforeach ()

    tbag_preview__print ("${__value}: ${__list_string}")
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
    ## Variables that Describe the System:
    # APPLE BORLAND CYGWIN ENV
    # MSVC MSVC_IDE MSVC10 MSVC11 MSVC12 MSVC60 MSVC70 MSVC71 MSVC80 MSVC90 MSVC_VERSION
    # UNIX WIN32 XCODE_VERSION
    message (WARNING "Not implement.")
endfunction ()

#/// Preview information.
function (tbag_preview)
    if (TBAG_SAVE_PREVIEW_FILE)
        file (REMOVE "${TBAG_PREVIEW_FILE}")
    endif ()

    message ("")
    tbag_preview__print ("#################### PREVIEW ####################")
    tbag_preview__print ("")

    tbag_preview__print_os ()
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_GENERATOR)
    tbag_preview__print ("")

    tbag_preview__print_variable (CCACHE_FOUND)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_HOST_SYSTEM_NAME)
    tbag_preview__print_variable (CMAKE_HOST_SYSTEM_VERSION)
    tbag_preview__print_variable (CMAKE_HOST_SYSTEM_PROCESSOR)
    tbag_preview__print ("")

    if (CMAKE_CROSSCOMPILING)
        tbag_preview__print_variable (CMAKE_SYSTEM_NAME)
        tbag_preview__print_variable (CMAKE_SYSTEM_VERSION)
        tbag_preview__print_variable (CMAKE_SYSTEM_PROCESSOR)
        tbag_preview__print ("")
    endif ()

    tbag_preview__print_variable (CMAKE_VERSION)
    tbag_preview__print_variable (CMAKE_GENERATOR)
    tbag_preview__print_variable (CMAKE_BUILD_TOOL)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_C_COMPILER_ID)
    tbag_preview__print_variable (CMAKE_C_COMPILER)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_CXX_COMPILER_ID)
    tbag_preview__print_variable (CMAKE_CXX_COMPILER)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_BUILD_TYPE)
    tbag_preview__print_variable (BUILD_SHARED_LIBS)
    tbag_preview__print ("")

    tbag_preview__print_variable (PROJECT_SOURCE_DIR)
    tbag_preview__print_variable (PROJECT_BINARY_DIR)
    tbag_preview__print_variable (CMAKE_INSTALL_PREFIX)
    tbag_preview__print ("")

    tbag_preview__print_variable (MAIN_NAME)
    tbag_preview__print_variable (MAIN_AUTHOR)
    tbag_preview__print_variable (MAIN_EMAIL)
    tbag_preview__print_variable (MAIN_BRIEF)
    tbag_preview__print_variable (VERSION)
    tbag_preview__print_variable (VERSION_TWEAK)
    tbag_preview__print ("PACKET VERSION: ${VERSION_PACKET_MAJOR}.${VERSION_PACKET_MINOR}")
    tbag_preview__print ("RELEASE VERSION: ${VERSION_RELEASE}")
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_C_FLAGS)
    tbag_preview__print_variable (CMAKE_C_FLAGS_DEBUG)
    tbag_preview__print_variable (CMAKE_C_FLAGS_RELEASE)
    tbag_preview__print ("")

    tbag_preview__print_variable (CMAKE_CXX_FLAGS)
    tbag_preview__print_variable (CMAKE_CXX_FLAGS_DEBUG)
    tbag_preview__print_variable (CMAKE_CXX_FLAGS_RELEASE)
    tbag_preview__print ("")

    #tbag_preview__print_variable (CMAKE_EXE_LINKER_FLAGS)
    #tbag_preview__print_variable (CMAKE_EXE_LINKER_FLAGS_DEBUG)
    #tbag_preview__print_variable (CMAKE_EXE_LINKER_FLAGS_RELEASE)

    #tbag_preview__print_variable (CMAKE_MODULE_LINKER_FLAGS)
    #tbag_preview__print_variable (CMAKE_MODULE_LINKER_FLAGS_DEBUG)
    #tbag_preview__print_variable (CMAKE_MODULE_LINKER_FLAGS_RELEASE)

    #tbag_preview__print_variable (CMAKE_SHARED_LINKER_FLAGS)
    #tbag_preview__print_variable (CMAKE_SHARED_LINKER_FLAGS_DEBUG)
    #tbag_preview__print_variable (CMAKE_SHARED_LINKER_FLAGS_RELEASE)

    #tbag_preview__print_variable (CMAKE_C_FLAGS_DEBUG_INIT)
    #tbag_preview__print_variable (CMAKE_C_FLAGS_MINSIZEREL_INIT)
    #tbag_preview__print_variable (CMAKE_C_FLAGS_RELEASE_INIT)
    #tbag_preview__print_variable (CMAKE_C_FLAGS_RELWITHDEBINFO_INITT)

    #tbag_preview__print_variable (CMAKE_CXX_FLAGS_DEBUG_INIT)
    #tbag_preview__print_variable (CMAKE_CXX_FLAGS_MINSIZEREL_INIT)
    #tbag_preview__print_variable (CMAKE_CXX_FLAGS_RELEASE_INIT)
    #tbag_preview__print_variable (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INITT)

    if (CMAKE_USE_SPROC_INIT)
        tbag_preview__print_variable (CMAKE_USE_SPROC_INIT)
    endif ()
    if (CMAKE_USE_WIN32_THREADS_INIT)
        tbag_preview__print_variable (CMAKE_USE_WIN32_THREADS_INIT)
    endif ()
    if (CMAKE_USE_PTHREADS_INIT)
        tbag_preview__print_variable (CMAKE_USE_PTHREADS_INIT)
    endif ()
    if (CMAKE_HP_PTHREADS_INIT)
        tbag_preview__print_variable (CMAKE_HP_PTHREADS_INIT)
    endif ()
    tbag_preview__print ("")

    get_property (TBAG_FIND_SUBPROJECT_STATIC_LIBS GLOBAL PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_STATIC_LIBS)
    get_property (TBAG_FIND_SUBPROJECT_SHARED_LIBS GLOBAL PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_SHARED_LIBS)
    get_property (TBAG_FIND_SUBPROJECT_MODULE_LIBS GLOBAL PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_MODULE_LIBS)
    get_property (TBAG_FIND_SUBPROJECT_EXES        GLOBAL PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_EXES)

    tbag_preview__print_variable_list (TBAG_FIND_SUBPROJECT_STATIC_LIBS)
    tbag_preview__print_variable_list (TBAG_FIND_SUBPROJECT_SHARED_LIBS)
    tbag_preview__print_variable_list (TBAG_FIND_SUBPROJECT_MODULE_LIBS)
    tbag_preview__print_variable_list (TBAG_FIND_SUBPROJECT_EXES)

    tbag_preview__print ("")

    if (TBAG_DEBUG_TAG)
        tbag_preview__print ("ENABLE TBAG_DEBUG_TAG: ${TBAG_DEBUG_TAG}")
    else ()
        tbag_preview__print ("DISABLE TBAG_DEBUG_TAG")
    endif ()

    tbag_preview__print ("")
    tbag_preview__print ("#################################################")
endfunction ()

