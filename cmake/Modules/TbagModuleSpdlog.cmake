#/// @file   TbagModuleSpdlog.cmake
#/// @brief  Speed-log tbag cmake module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign speed-log libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_spdlog)
    find_package (Spdlog)
    if (SPDLOG_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_SPDLOG)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SPDLOG_INCLUDE_DIRS})
    endif ()
endmacro ()

