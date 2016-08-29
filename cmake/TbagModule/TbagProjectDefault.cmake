#/// @file   TbagProjectDefault.cmake
#/// @brief  Default project module.
#/// @author zer0
#/// @date   2016-06-01

string (TOUPPER "${TBAG_PROJECT_CONST_NAME}" __tbag_project_upper_name)
list (APPEND TBAG_PROJECT_DEFINITIONS  -D${__tbag_project_upper_name})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TBAG_PROJECT_CONST_DIR_PATH})
list (APPEND TBAG_PROJECT_LDFLAGS      -L${TBAG_PROJECT_CONST_DIR_PATH})

if (UNIX AND APPLE)
    if (IS_DIRECTORY "${THIRD_PREFIX}")
        list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-rpath,${THIRD_PREFIX}/lib"
                                          "-Wl,-rpath,${THIRD_PREFIX}/Library/Frameworks")
    endif ()
endif ()

