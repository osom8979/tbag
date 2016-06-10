#/// @file   TbagProjectPythonLibs.cmake
#/// @brief  CPython libraries project module.
#/// @author zer0
#/// @date   2016-06-03
#/// @date   2016-06-10 (Rename: TbagProjectPythonLibrary -> TbagProjectPythonLibs)

if (NOT PYTHONLIBS_FOUND)
    message (WARNING "Not found CPython libraries.")
endif ()

#message (STATUS "CPython version: ${PYTHONLIBS_VERSION_STRING}")
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${PYTHON_LIBRARIES})

