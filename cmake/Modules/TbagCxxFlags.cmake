#/// @file   TbagCxxFlags.cmake
#/// @brief  C/C++ Flag helper.
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
function (tbag_cxx_flags__whole_archive __result)
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

#/// Assign Position Independent Code.
macro (tbag_cxx_flags__set_position_independent_code)
    if (CMAKE_VERSION VERSION_LESS "3.0.2")
        set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    else ()
        set (CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC
    endif ()
endmacro ()

#/// Assign C++11 standard.
macro (tbag_cxx_flags__set_cpp_standard_11)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CUDA_FOUND)
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        else ()
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif ()
        if (CUDA_FOUND)
            set (CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11")
        endif ()
    else ()
        set (CMAKE_CXX_STANDARD     11) # C++ standard 11
        set (CMAKE_CXX_EXTENSIONS  OFF) # Don't change the -std=gnu++11 to -std=c++11
    endif ()
endmacro ()

#/// Assign default debug definition.
macro (tbag_cxx_flags__set_debug_define)
    # Build type: Debug, Release, RelWithDebInfo, MinSizeRel
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_definitions (-DDEBUG)
    else ()
        add_definitions (-DNDEBUG -DRELEASE)
    endif ()
endmacro ()

#/// include & link directory for source directory.
macro (tbag_cxx_flags__set_path_for_source_dir)
    list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")
endmacro ()

#/// include & link directory for source directory.
macro (tbag_cxx_flags__set_include_and_link_for_source_dir)
    include_directories (${PROJECT_SOURCE_DIR})
    link_directories    (${PROJECT_SOURCE_DIR})
endmacro ()

#/// Print all compile warning.
macro (tbag_cxx_flags__set_print_all_compile_warning)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endmacro ()

#/// Setup RPATH variables.
macro (tbag_cxx_flags__set_install_rpath)
    set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)
    set (CMAKE_INSTALL_RPATH     "${CMAKE_INSTALL_PREFIX}/lib")
    set (CMAKE_INSTALL_NAME_DIR  "${CMAKE_INSTALL_PREFIX}/lib")
endmacro ()

#/// Default tbag project setup.
macro (tbag_cxx_flags)
    tbag_cxx_flags__set_position_independent_code       ()
    tbag_cxx_flags__set_cpp_standard_11                 ()
    tbag_cxx_flags__set_debug_define                    ()
    tbag_cxx_flags__set_path_for_source_dir             ()
    tbag_cxx_flags__set_include_and_link_for_source_dir ()
    tbag_cxx_flags__set_print_all_compile_warning       ()
    #tbag_cxx_flags__set_install_rpath                  ()
endmacro ()

