#/// Write Binary to Array C++ header file.
#///
#/// @param OUTPUT_PATH [in] Output header file path.
#/// @param NAME        [in] Variable name.
#/// @param BINARY_PATH [in] Binary file path.

file (READ "${BINARY_PATH}" __content HEX)

string (LENGTH "${__content}" __content_length)
if (__content_length LESS 2)
    message (ERROR "The size of content length is small: (${__content_length})")
    return ()
endif ()

math (EXPR __is_odd_number "${__content_length} % 2")
if (__is_odd_number EQUAL 1)
    message (ERROR "The result must be an even number: (${__content_length})")
    return ()
endif ()

math (EXPR __size "${__content_length} / 2")
set (__index 0)

string (SUBSTRING "${__content}" ${__index} 2 __hex)
set (__array_list "0x${__hex}")
math (EXPR __index "${__index} + 2")

while (${__index} LESS ${__content_length})
    string (SUBSTRING "${__content}" ${__index} 2 __hex)
    list (APPEND __array_list ",0x${__hex}")
    math (EXPR __index "${__index} + 2")
endwhile ()

string (CONCAT __array ${__array_list})

file (WRITE ${OUTPUT_PATH} ""
        "\n#ifndef __INCLUDE__BINARY_TO_CPP_ARRAY__${NAME}_H__"
        "\n#define __INCLUDE__BINARY_TO_CPP_ARRAY__${NAME}_H__"
        "\n"
        "\ninline unsigned long __get_binary_to_cpp_array_size__${NAME}__()"
        "\n{"
        "\n    return ${__size};"
        "\n}"
        "\n"
        "\ninline unsigned char const * const __get_binary_to_cpp_array__${NAME}__()"
        "\n{"
        "\n    return { ${__array} };"
        "\n}"
        "\n"
        "\n#endif /* __INCLUDE__BINARY_TO_CPP_ARRAY__${NAME}_H__ */"
        "\n")
message (STATUS "Write Binary2CppArray: ${OUTPUT_PATH}")

