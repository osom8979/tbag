#/// Remove content matching regex. And removes blank lines.
#///
#/// @param OUTPUT_PATH [in] Output file path.
#/// @param SOURCE_PATH [in] Source file path.
#/// @param REGEX       [in] Remove regular expression.

get_filename_component (OUTPUT_DIR "${OUTPUT_PATH}" DIRECTORY)
if (NOT IS_DIRECTORY "${OUTPUT_DIR}")
    file (MAKE_DIRECTORY "${OUTPUT_DIR}")
endif ()

file (STRINGS "${SOURCE_PATH}" __content ENCODING UTF-8)
foreach (__cursor ${__content})
    if (NOT __cursor MATCHES ${REGEX})
        file (APPEND "${OUTPUT_PATH}" "${__cursor}" "\n")
    endif ()
endforeach ()

message (STATUS "Write StripRegex: ${OUTPUT_PATH}")

