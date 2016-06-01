#/// @file   TbagObjectCxxObfuscator.cmake
#/// @brief  C++ Obfuscator object module.
#/// @author zer0
#/// @date   2016-06-01

if (EXISTS "${TBAG_OBFUS_CONFIG_PATH}")
    message (FATAL_ERROR "Not found ${TBAG_OBFUS_CONFIG_PATH}")
endif ()

if (NOT CxxObfuscator_FOUND)
    find_package (CxxObfuscator)
endif ()

if (CxxObfuscator_FOUND)
    obfus_generate_cpp (TBAG_OBJECT_OBJECTS "${TBAG_OBJECT_CONST_SOURCES}" "${TBAG_OBFUS_CONFIG_PATH}")
endif ()

