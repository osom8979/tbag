#/// @file   TbagProjectPythonLibrary.cmake
#/// @brief  CPython library project module.
#/// @author zer0
#/// @date   2016-06-03

if (NOT PYTHONLIBS_FOUND)
    find_package (PythonLibs)
endif ()

if (PYTHONLIBS_FOUND)
    #message (STATUS "CPython version: ${PYTHONLIBS_VERSION_STRING}")
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_PYTHON)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${PYTHON_LIBRARIES})
endif ()

