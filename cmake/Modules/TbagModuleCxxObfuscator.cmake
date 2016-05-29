#/// @file   TbagModuleCxxObfuscator.cmake
#/// @brief  C++ Obfuscator tbag module.
#/// @author zer0
#/// @date   2016-05-29

set (TABG_OBFUS_SUFFIX ".obf.cpp")

#/// C++ Obfuscator or Native C/C++ Source files.
#///
#/// @param __obfus_config_path [in] C++ obfuscator config path.
macro (tbag_module_cxx_obfuscator__object __obfus_config_path)
    if (EXISTS "${__obfus_config_path}")
        message (FATAL_ERROR "Not found ${__obfus_config_path}")
    endif ()

    if (NOT CxxObfuscator_FOUND)
        find_package (CxxObfuscator)
    endif ()

    if (CxxObfuscator_FOUND)
        obfus_generate_cpp (TBAG_OBJECT_OBJECTS "${TBAG_OBJECT_CONST_SOURCES}" "${__obfus_config_path}")
    endif ()
endmacro ()

