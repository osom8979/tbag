#/// Write Text to String C++11 header file.
#///
#/// @param OUTPUT_PATH [in] Output header file path.
#/// @param NAME        [in] Variable name.
#/// @param SOURCE_PATH [in] Source file path.
#/// @param HEX_MODE    [in] HEX Mode ON/OFF (Default: OFF)

if (HEX_MODE)
    file (READ "${SOURCE_PATH}" __content HEX)
else ()
    file (READ "${SOURCE_PATH}" __content)
endif ()

# MSVC Specific:
# The maximum length of a string literal is 16,384 (16K) bytes.
# http://msdn.microsoft.com/en-us/library/69ze775t%28v=vs.110%29.aspx
set (__substring_size 16384)
set (__index 0)

string (LENGTH "${__content}" __content_length)

set (__header_prefix
        "\n#ifndef __INCLUDE__DATA_TO_STRING__${NAME}_H__"
        "\n#define __INCLUDE__DATA_TO_STRING__${NAME}_H__"
        "\n"
        "\nchar const * __data_to_string__${NAME}__[] = {")
set (__header_suffix
        "\n        0"
        "\n    }\;"
        "\n"
        "\n#endif /* __INCLUDE__DATA_TO_STRING__${NAME}_H__ */"
        "\n")

get_filename_component (OUTPUT_DIR ${OUTPUT_PATH} DIRECTORY)
if (NOT IS_DIRECTORY "${OUTPUT_DIR}")
    file (MAKE_DIRECTORY "${OUTPUT_DIR}")
endif ()

file (WRITE ${OUTPUT_PATH} ${__header_prefix})
while (${__index} LESS ${__content_length})
    string (SUBSTRING "${__content}" ${__index} ${__substring_size} __content_cursor)
    file (APPEND ${OUTPUT_PATH} "\n        R\"__(${__content_cursor})__\",")
    math (EXPR __index "${__index} + ${__substring_size}")
endwhile ()
file (APPEND ${OUTPUT_PATH} ${__header_suffix})

message (STATUS "Write Data2String: ${OUTPUT_PATH}")

