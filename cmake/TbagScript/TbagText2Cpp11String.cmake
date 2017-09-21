#/// Write Text to String C++11 header file.
#///
#/// @param OUTPUT_PATH [in] Output header file path.
#/// @param NAME        [in] Variable name.
#/// @param DELIMITER   [in] Delimiter of C++11 string literal.
#/// @param TEXT_PATH   [in] Text file path.

file (READ "${TEXT_PATH}" __content)
file (WRITE ${OUTPUT_PATH} ""
        "\n#ifndef __INCLUDE__TEXT_TO_CPP11_STRING__${NAME}_H__"
        "\n#define __INCLUDE__TEXT_TO_CPP11_STRING__${NAME}_H__"
        "\n"
        "\ninline char const * const __get_text_to_cpp11_string__${NAME}__()"
        "\n{"
        "\n    return R\"${DELIMITER}(${__content})${DELIMITER}\";"
        "\n}"
        "\n"
        "\n#endif /* __INCLUDE__TEXT_TO_CPP11_STRING__${NAME}_H__ */"
        "\n")
message (STATUS "Write Text2String: ${OUTPUT_PATH}")

