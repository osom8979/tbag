#/// Write Binary to String C++11 header file.
#///
#/// @param OUTPUT_PATH [in] Output header file path.
#/// @param NAME        [in] Variable name.
#/// @param DELIMITER   [in] Delimiter of C++11 string literal.
#/// @param BINARY_PATH [in] Binary file path.

file (READ "${BINARY_PATH}" __content HEX)
file (WRITE ${OUTPUT_PATH} ""
        "\n#ifndef __INCLUDE__BINARY_TO_CPP11_STRING__${NAME}_H__"
        "\n#define __INCLUDE__BINARY_TO_CPP11_STRING__${NAME}_H__"
        "\n"
        "\ninline char const * const __get_binary_to_cpp11_string__${NAME}__()"
        "\n{"
        "\n    return R\"${DELIMITER}(${__content})${DELIMITER}\";"
        "\n}"
        "\n"
        "\n#endif /* __INCLUDE__BINARY_TO_CPP11_STRING__${NAME}_H__ */"
        "\n")
message (STATUS "Write Binary2String: ${OUTPUT_PATH}")

