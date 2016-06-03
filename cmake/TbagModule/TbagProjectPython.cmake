#/// @file   TbagProjectPython.cmake
#/// @brief  Python interpreter project module.
#/// @author zer0
#/// @date   2016-06-03

if (NOT PYTHONINTERP_FOUND)
    find_package (PythonInterp)
endif ()

if (PYTHONINTERP_FOUND)
    #message (STATUS "Python interpreter: ${PYTHON_EXECUTABLE}")
    #message (STATUS "Python interpreter version: ${PYTHON_VERSION_STRING}")
endif ()

