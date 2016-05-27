#/// @file   TbagModuleUV.cmake
#/// @brief  libuv tbag cmake module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign libuv.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_uv)
    if (NOT UV_FOUND)
        find_package (UV)
    endif ()

    if (UV_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_LIBUV)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${UV_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${UV_LIBRARIES})
    endif ()
endmacro ()

