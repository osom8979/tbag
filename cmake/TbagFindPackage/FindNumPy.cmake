## Find the Python NumPy library.
#
# The following are set after configuration is done:
#  NumPy_FOUND
#  NumPy_INCLUDE_DIRS
#  NumPy_VERSION

if (NumPy_FOUND)
    return ()
endif ()

if (Python3_Interpreter_FOUND)
    set (__python_executable ${Python3_EXECUTABLE})
elseif (Python_Interpreter_FOUND)
    set (__python_executable ${Python_EXECUTABLE})
elseif (PYTHONINTERP_FOUND)
    set (__python_executable ${PYTHON_EXECUTABLE})  ## Legacy
else ()
    set (__python_executable python)
    message (WARNING "FindNumpy: Not found Python interpreter.")
endif ()

execute_process(COMMAND "${__python_executable}" "-c" "import numpy as n; print(n.__version__); print(n.get_include());"
                RESULT_VARIABLE __result
                OUTPUT_VARIABLE __output
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if (__result MATCHES 0)
    string (REGEX REPLACE ";" "\\\\;" __values ${__output})
    string (REGEX REPLACE "\r?\n" ";" __values ${__values})
    list (GET __values 0 NumPy_VERSION)
    list (GET __values 1 NumPy_INCLUDE_DIRS)

    string (REGEX MATCH "^([0-9])+\\.([0-9])+\\.([0-9])+" __ver_check "${NumPy_VERSION}")
    if (NOT "${__ver_check}" STREQUAL "")
        set (NumPy_VERSION_MAJOR ${CMAKE_MATCH_1})
        set (NumPy_VERSION_MINOR ${CMAKE_MATCH_2})
        set (NumPy_VERSION_PATCH ${CMAKE_MATCH_3})
        math (EXPR NumPy_VERSION_DECIMAL "(${NumPy_VERSION_MAJOR} * 10000) + (${NumPy_VERSION_MINOR} * 100) + ${NumPy_VERSION_PATCH}")
        string (REGEX REPLACE "\\\\" "/"  NumPy_INCLUDE_DIRS ${NumPy_INCLUDE_DIRS})
    else ()
        unset (NumPy_VERSION)
        unset (NumPy_INCLUDE_DIRS)
        message (STATUS "Requested NumPy version and include path, but got instead: ${__output}")
    endif ()
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (NumPy
                                   REQUIRED_VARS NumPy_INCLUDE_DIRS NumPy_VERSION
                                   VERSION_VAR   NumPy_VERSION)

# If the variables <var1> to <varN> are all valid,
# then <UPPERCASED_NAME>_FOUND will be set to TRUE.
set (NumPy_FOUND "${NUMPY_FOUND}")

if (NumPy_FOUND AND NOT NumPy_FIND_QUIETLY)
    message (STATUS "NumPy ver. ${NumPy_VERSION} found (include: ${NumPy_INCLUDE_DIRS})")
endif()

