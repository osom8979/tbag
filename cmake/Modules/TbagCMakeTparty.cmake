#/// @file   TbagCMakeTparty.cmake
#/// @brief  Default tbag with tpary cmake configure.
#/// @author zer0
#/// @date   2016-05-26

include (TbagFilesystem) # For the PATH_SEPARATOR variable.

#/// Initialize third-party library variables.
macro (tbag_tparty_init)
    # Third-party library directory.
    set (THIRD_PREFIX "$ENV{TPARTY_HOME}/local" CACHE STRING "3rd-party library prefix")

    set (THIRD_BIN "${THIRD_PREFIX}/bin")
    set (THIRD_INC "${THIRD_PREFIX}/include")
    set (THIRD_LIB "${THIRD_PREFIX}/lib")

    if (IS_DIRECTORY ${THIRD_BIN})
        set (ENV{PATH} "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
    endif ()
    if (IS_DIRECTORY ${THIRD_INC})
        set (ENV{CPATH} "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
    endif ()
    if (IS_DIRECTORY ${THIRD_LIB})
        set (ENV{LIBRARY_PATH} "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
    endif ()

    list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")

    include_directories (${THIRD_INC})
    link_directories (${THIRD_LIB})
endmacro ()

