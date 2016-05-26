#/// @file   TbagCMakeTparty.cmake
#/// @brief  Default tbag with tpary cmake configure.
#/// @author zer0
#/// @date   2016-05-26

include (TbagFilesystem) # Use the PATH_SEPARATOR variable.

macro (tbag_tparty_init)
    # 3rd-party library prefix.
    set (THIRD_HOME   "$ENV{TPARTY_HOME}" CACHE STRING "3rd-party library prefix")

    if (IS_DIRECTORY ${THIRD_HOME})
        set (THIRD_PREFIX "${TPARTY_HOME}/local")

        # Third-party libraries.
        set (THIRD_BIN "${THIRD_PREFIX}/bin")
        set (THIRD_INC "${THIRD_PREFIX}/include")
        set (THIRD_LIB "${THIRD_PREFIX}/lib")

        if (IS_DIRECTORY ${THIRD_BIN})
            set (ENV{PATH} "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
        endif ()

        if (IS_DIRECTORY ${THIRD_INC})
            set (ENV{CPATH} "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
            include_directories (${THIRD_INC})
        endif ()

        if (IS_DIRECTORY ${THIRD_LIB})
            set (ENV{LIBRARY_PATH} "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
            link_directories (${THIRD_LIB})
        endif ()

        list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
        list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")
    endif ()
endmacro ()

