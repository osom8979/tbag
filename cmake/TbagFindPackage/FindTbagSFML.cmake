#/// @file   FindTbagSFML.cmake
#/// @brief  Find the SFML library.
#/// @author zer0
#/// @date   2016-08-29
#///
#/// @remarks
#///  The following variables are optionally searched for defaults:
#///    - ${SFML_ROOT}

cmake_policy (PUSH)

# Policy CMP0074 is not set: find_package uses <PackageName>_ROOT variables.
if (POLICY CMP0074)
    cmake_policy (SET CMP0074 NEW)
endif ()

if (NOT DEFINED SFML_ROOT)
    if (IS_DIRECTORY "${THIRD_PREFIX}")
        set (SFML_ROOT "${THIRD_PREFIX}")
    else ()
        set (SFML_ROOT "/usr/local")
    endif ()
endif ()

set (__tbag_sfml_module_dir "${SFML_ROOT}/share/SFML/cmake/Modules")
if (NOT IS_DIRECTORY "${__tbag_sfml_module_dir}")
    if (IS_DIRECTORY "/usr/local/share/SFML/cmake/Modules")
        set (__tbag_sfml_module_dir "/usr/local/share/SFML/cmake/Modules")
    else ()
        #message (WARNING "Not found SFML Modules directory: ${__tbag_sfml_module_dir}")
    endif ()
endif ()

if (NOT SFML_FOUND)
    list (FIND "${CMAKE_MODULE_PATH}" "${__tbag_sfml_module_dir}" __sfml_module_index)
    if (__sfml_module_index EQUAL -1)
        list (APPEND CMAKE_MODULE_PATH "${__tbag_sfml_module_dir}")
    endif ()

    if (TbagSFML_FIND_QUIETLY)
        set (__tbag_sfml_quiet QUIET)
    else ()
        set (__tbag_sfml_quiet)
    endif ()

    find_package (SFML 2 ${__tbag_sfml_quiet} COMPONENTS system window graphics audio)
endif ()

if (SFML_FOUND)
    message (STATUS "Found SFML: ${SFML_LIBRARIES}")
endif ()

cmake_policy (POP)

