#/// @file   TbagProjectPCH.cmake
#/// @brief  PCH module.
#/// @author zer0
#/// @date   2016-08-20
#/// @remarks
#///  Precompiled Header.

if (NOT EXISTS ${TbagProjectPCH_0})
    message (FATAL_ERROR "Not found original header file path (parameter 01).")
endif ()

get_filename_component (TbagProjectPCH_ORIGINAL_NAME "${TbagProjectPCH_0}" NAME)
string (REPLACE "." "-" TbagProjectPCH_NAME "${TBAG_PROJECT_CONST_NAME}-${TbagProjectPCH_ORIGINAL_NAME}-pch")
set (TbagProjectPCH_COPY_PATH ${CMAKE_BINARY_DIR}/${TbagProjectPCH_ORIGINAL_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GCC")
    set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.gch")
else ()
    set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.pch")
endif ()

## PCH custom target.
add_custom_target (${TbagProjectPCH_NAME} ALL DEPENDS ${TbagProjectPCH_RESULT_PATH})

add_custom_command (
        OUTPUT ${TbagProjectPCH_COPY_PATH}
        COMMAND ${CMAKE_COMMAND} -E copy "${TbagProjectPCH_0}" "${TbagProjectPCH_COPY_PATH}"
        DEPENDS "${TbagProjectPCH_0}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command (
        OUTPUT ${TbagProjectPCH_RESULT_PATH}
        COMMAND ${CMAKE_CXX_COMPILER} -x c++-header ${TbagProjectPCH_COPY_PATH} -std=c++11 -o ${TbagProjectPCH_RESULT_PATH}
        DEPENDS "${TbagProjectPCH_COPY_PATH}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

list (APPEND TBAG_PROJECT_DEPENDENCIES ${TbagProjectPCH_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_BINARY_DIR})
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list (APPEND TBAG_PROJECT_CXXFLAGS -include ${TbagProjectPCH_COPY_PATH})
endif ()

