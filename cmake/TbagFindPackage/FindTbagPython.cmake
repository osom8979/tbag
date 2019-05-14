## Find the python library.
#
# The following variables are optionally searched for defaults
#  TbagPython_PYENV_NAME
#  TbagPython_FIND_VERSION

macro (tabg_python__find_development_in_interpreter)
    execute_process (COMMAND "${Python_EXECUTABLE}" "-c"
                             "import distutils.sysconfig as s; print(s.get_python_inc())"
            RESULT_VARIABLE __result
            OUTPUT_VARIABLE __output
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (__result MATCHES 0)
        set (Python_INCLUDE_DIRS ${__output})
    else ()
        message (FATAL "Python script error (INC): ${__result}")
    endif ()

    execute_process (COMMAND "${Python_EXECUTABLE}" "-c"
                             "import distutils.sysconfig as s; print(s.get_config_var('LIBDIR'))"
            RESULT_VARIABLE __result
            OUTPUT_VARIABLE __output
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (__result MATCHES 0)
        set (Python_LIBRARY_DIRS ${__output})
    else ()
        message (FATAL "Python script error (LIBDIR): ${__result}")
    endif ()

    execute_process (COMMAND "${Python_EXECUTABLE}" "-c"
                             "import distutils.sysconfig as s; print(s.get_config_var('PYTHONFRAMEWORKPREFIX'))"
            RESULT_VARIABLE __result
            OUTPUT_VARIABLE __output
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (__result MATCHES 0)
        set (Python_PYTHONFRAMEWORKPREFIX ${__output})
    else ()
        message (FATAL "Python script error (base): ${__result}")
    endif ()

    execute_process (COMMAND "${Python_EXECUTABLE}" "-c"
                             "import distutils.sysconfig as s; print(s.get_config_var('LDLIBRARY'))"
            RESULT_VARIABLE __result
            OUTPUT_VARIABLE __output
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (__result MATCHES 0)
        if (Python_PYTHONFRAMEWORKPREFIX)
            set (Python_LIBRARIES ${Python_PYTHONFRAMEWORKPREFIX}/${__output})
        else ()
            set (Python_LIBRARIES ${Python_LIBRARY_DIRS}/${__output})
        endif ()
    else ()
        message (FATAL "Python script error (LDLIBRARY): ${__result}")
    endif ()

    execute_process (COMMAND "${Python_EXECUTABLE}" "-c"
                             "import platform as p; print(p.python_version())"
            RESULT_VARIABLE __result
            OUTPUT_VARIABLE __output
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (__result MATCHES 0)
        set (Python_VERSION_STRING ${__output})
    else ()
        message (FATAL "Python script error (VER): ${__result}")
    endif ()

    set (Python_Development_FOUND ON)
endmacro ()

#/// Find package with list.
#///
#/// @param ... [in] COMPONENTS arguments: Interpreter, Compiler, Development, NumPy
#/// @param ... [in] QUIET, LAST
macro (tabg_python__find_default)
    set (__python_interpreter)
    set (__python_compiler)
    set (__python_development)
    set (__python_numpy)
    set (__python_quiet)
    set (__python_last)

    foreach (__cursor ${ARGN})
        if ("${__cursor}" STREQUAL "Interpreter")
            set (__python_interpreter Interpreter)
        elseif ("${__cursor}" STREQUAL "Compiler")
            set (__python_compiler Compiler)
        elseif ("${__cursor}" STREQUAL "Development")
            set (__python_development Development)
        elseif ("${__cursor}" STREQUAL "NumPy")
            set (__python_numpy NumPy)
        elseif ("${__cursor}" STREQUAL "QUIET")
            set (__python_quiet QUIET)
        elseif ("${__cursor}" STREQUAL "LAST")
            set (__python_last LAST)
        else ()
            message (FATAL "tbag_config__find_python() argument error: ${ARGN}")
        endif ()
    endforeach ()

    if (NOT __python_quiet STREQUAL "QUIET")
        set (__python_quiet REQUIRED)
    endif ()

    if (__python_last)
        set (__original_cmake_find_framework ${CMAKE_FIND_FRAMEWORK})
        set (__original_python_find_registry ${Python_FIND_REGISTRY})
        set (Python_FIND_REGISTRY LAST)
        set (CMAKE_FIND_FRAMEWORK LAST)
    endif ()

    if (__python_interpreter AND NOT Python_Interpreter_FOUND)
        #if (CMAKE_VERSION VERSION_LESS "3.12")
            if (TbagPython_PYENV_NAME)
                set (Python_ROOT_DIR "$ENV{HOME}/.pyenv/versions/${TbagPython_PYENV_NAME}")
                set (Python_Interpreter_FOUND ON)
                set (Python_EXECUTABLE ${Python_ROOT_DIR}/bin/python)
                if (TbagPython_FIND_VERSION)
                    set (Python_VERSION ${TbagPython_FIND_VERSION})
                endif ()
                # set (Python_VERSION_MAJOR ${PYTHON_VERSION_MAJOR})
                # set (Python_VERSION_MINOR ${PYTHON_VERSION_MINOR})
                # set (Python_VERSION_PATCH ${PYTHON_VERSION_PATCH})
            else ()
                find_package (PythonInterp ${__python_quiet} ${TbagPython_FIND_VERSION})
                if (PYTHONINTERP_FOUND)
                    set (Python_Interpreter_FOUND ${PYTHONINTERP_FOUND})
                endif ()
                if (PYTHON_EXECUTABLE)
                    set (Python_EXECUTABLE ${PYTHON_EXECUTABLE})
                endif ()
                if (PYTHON_VERSION_STRING)
                    set (Python_VERSION ${PYTHON_VERSION_STRING})
                endif ()
                if (PYTHON_VERSION_MAJOR)
                    set (Python_VERSION_MAJOR ${PYTHON_VERSION_MAJOR})
                endif ()
                if (PYTHON_VERSION_MINOR)
                    set (Python_VERSION_MINOR ${PYTHON_VERSION_MINOR})
                endif ()
                if (PYTHON_VERSION_PATCH)
                    set (Python_VERSION_PATCH ${PYTHON_VERSION_PATCH})
                endif ()
            endif ()
        #else ()
        #    if (TbagPython_PYENV_NAME)
        #        set (Python_ROOT_DIR "$ENV{HOME}/.pyenv/versions/${TbagPython_PYENV_NAME}")
        #    endif ()
        #    message (STATUS "[FindTbagPython] Python_ROOT_DIR: ${Python_ROOT_DIR}")
        #    if ("${TbagPython_FIND_VERSION}" VERSION_LESS "3")
        #        message (STATUS "[FindTbagPython] Find Python2")
        #        find_package (Python2 ${__python_quiet} ${TbagPython_FIND_VERSION} COMPONENTS Interpreter)
        #    else ()
        #        message (STATUS "[FindTbagPython] Find Python3")
        #        find_package (Python3 ${__python_quiet} ${TbagPython_FIND_VERSION} COMPONENTS Interpreter)
        #    endif ()
        #endif ()
    endif ()

    if (__python_development AND NOT Python_Development_FOUND)
        if (TbagPython_PYENV_NAME)
            if (TbagPython_FIND_VERSION)
                set (Python_ROOT_DIR "$ENV{HOME}/.pyenv/versions/${TbagPython_FIND_VERSION}")
            else ()
                set (Python_ROOT_DIR "$ENV{HOME}/.pyenv/versions/${Python_VERSION}")
            endif ()
            tabg_python__find_development_in_interpreter ()
        else ()
            if (CMAKE_VERSION VERSION_LESS "3.12")
                find_package (PythonLibs ${__python_quiet} ${TbagPython_FIND_VERSION})
                if (PYTHONLIBS_FOUND)
                    set (Python_Development_FOUND ${PYTHONLIBS_FOUND})
                endif ()
                if (PYTHON_INCLUDE_DIRS)
                    set (Python_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
                endif ()
                if (PYTHON_LIBRARIES)
                    set (Python_LIBRARIES ${PYTHON_LIBRARIES})
                endif ()
                if (PYTHON_LIBRARY_DIRS)
                    set (Python_LIBRARY_DIRS ${PYTHON_INCLUDE_DIRS})
                endif ()
                if (PYTHONLIBS_VERSION_STRING)
                    set (Python_VERSION_STRING ${PYTHONLIBS_VERSION_STRING})
                endif ()
            else ()
                find_package (Python ${__python_quiet} ${TbagPython_FIND_VERSION} COMPONENTS Development)
            endif ()
        endif ()
    endif ()

    if (__python_numpy AND NOT Python_NumPy_FOUND)
        if (TbagPython_PYENV_NAME)
            set (Python_ROOT_DIR "$ENV{HOME}/.pyenv/versions/${TbagPython_FIND_VERSION}")
        endif ()

        if (CMAKE_VERSION VERSION_LESS "3.14")
            set (PYTHONINTERP_FOUND ${Python_Interpreter_FOUND})
            set (PYTHON_EXECUTABLE ${Python_EXECUTABLE})
            find_package (NumPy ${__python_quiet})
            if (NumPy_FOUND)
                set (Python_NumPy_FOUND ${NumPy_FOUND})
                set (Python_NumPy_INCLUDE_DIRS ${NumPy_INCLUDE_DIRS})
                set (Python_NumPy_VERSION ${NumPy_VERSION})
            endif ()
        else ()
            find_package (Python ${__python_quiet} COMPONENTS NumPy)
        endif ()
    endif ()

    if (__python_last)
        set (Python_FIND_REGISTRY ${__original_cmake_find_framework})
        set (CMAKE_FIND_FRAMEWORK ${__original_python_find_registry})
        unset (__original_cmake_find_framework)
        unset (__original_python_find_registry)
    endif ()

    if (NOT __python_quiet STREQUAL "QUIET")
        if (__python_interpreter AND Python_Interpreter_FOUND)
            message (STATUS "Found Python executable: ${Python_EXECUTABLE}")
            message (STATUS "Found Python executable version: ${Python_VERSION}")
        endif ()
        if (__python_development AND Python_Development_FOUND)
            message (STATUS "Found CPython include: ${Python_INCLUDE_DIRS}")
            message (STATUS "Found CPython library: ${Python_LIBRARIES}")
            message (STATUS "Found CPython version: ${Python_VERSION_STRING}")
        endif ()
        if (__python_numpy AND Python_NumPy_FOUND)
            message (STATUS "Found NumPy include: ${Python_NumPy_INCLUDE_DIRS}")
            message (STATUS "Found NumPy version: ${Python_NumPy_VERSION}")
        endif ()
    endif ()

    unset (__python_interpreter)
    unset (__python_compiler)
    unset (__python_development)
    unset (__python_numpy)
    unset (__python_quiet)
    unset (__python_last)
endmacro ()

if (TbagPython_FIND_QUIETLY)
    set (__tbag_python_quiet QUIET)
else ()
    set (__tbag_python_quiet)
endif ()

tabg_python__find_default (${TbagPython_FIND_COMPONENTS} ${__tbag_python_quiet})

