#/// @file   TbagProjectPCH.cmake
#/// @brief  PCH module.
#/// @author zer0
#/// @date   2016-08-20
#/// @remarks
#///  Precompiled Header.

if (NOT EXISTS ${TbagProjectPCH_0})
    message (FATAL_ERROR "Not found original header file path (parameter 01).")
endif ()

if (NOT DEFINED TbagProjectPCH_NAME)
    get_filename_component (TbagProjectPCH_ORIGINAL_NAME "${TbagProjectPCH_0}" NAME)
    string (REPLACE "." "-" TbagProjectPCH_NAME "${TbagProjectPCH_ORIGINAL_NAME}-pch")
    set (TbagProjectPCH_COPY_PATH ${CMAKE_BINARY_DIR}/${TbagProjectPCH_ORIGINAL_NAME})

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.gch")
    else ()
        set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.pch")
    endif ()

    ## Extension CXX flags.
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_DEBUG})
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_RELEASE})
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
        set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
        set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_MINSIZEREL})
    endif()

    string (REPLACE " " ";" TbagProjectPCH_CXX_FLAGS "${CMAKE_CXX_FLAGS};${TbagProjectPCH_CXX_FLAGS_EX}")

    ## Definitions.
    foreach (__define_cursor ${TBAG_PROJECT_DEFINITIONS})
        if ("${__define_cursor}" MATCHES "^-D")
            list (APPEND TbagProjectPCH_CXX_FLAGS "${__define_cursor}")
        else ()
            list (APPEND TbagProjectPCH_CXX_FLAGS "-D${__define_cursor}")
        endif ()
    endforeach ()

    ## Include directories.
    foreach (__include_cursor ${TBAG_PROJECT_INCLUDE_DIRS})
        if ("${__include_cursor}" MATCHES "^-I")
            list (APPEND TbagProjectPCH_CXX_FLAGS "${__include_cursor}")
        else ()
            list (APPEND TbagProjectPCH_CXX_FLAGS "-I${__include_cursor}")
        endif ()
    endforeach ()

    ## PCH custom target.
    add_custom_target (${TbagProjectPCH_NAME} ALL DEPENDS ${TbagProjectPCH_RESULT_PATH})

    add_custom_command (
            OUTPUT ${TbagProjectPCH_COPY_PATH}
            COMMAND ${CMAKE_COMMAND} -E copy "${TbagProjectPCH_0}" "${TbagProjectPCH_COPY_PATH}"
            DEPENDS "${TbagProjectPCH_0}"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    add_custom_command (
            OUTPUT ${TbagProjectPCH_RESULT_PATH}
            COMMAND ${CMAKE_CXX_COMPILER} ${TbagProjectPCH_CXX_FLAGS}
                                          -x c++-header ${TbagProjectPCH_COPY_PATH}
                                          -o ${TbagProjectPCH_RESULT_PATH}
            DEPENDS "${TbagProjectPCH_COPY_PATH}"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

list (APPEND TBAG_PROJECT_DEPENDENCIES ${TbagProjectPCH_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_BINARY_DIR})
list (APPEND TBAG_PROJECT_CXXFLAGS -include ${TbagProjectPCH_COPY_PATH})

