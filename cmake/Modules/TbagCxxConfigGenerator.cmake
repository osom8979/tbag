#/// @file   TbagCxxConfigGenerator.cmake
#/// @brief  tbag_cxx_config_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

#/// Write C/C++ header file.
#///
#/// @param __output_path [in] Output header file path.
function (tbag_cxx_config_generate __output_path)
    if (EXISTS "${__output_path}")
        file (REMOVE "${__output_path}")
    endif ()

    file (WRITE "${__output_path}"
        "#ifndef __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__\n"
        "#define __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__\n"
        "\n"
        "#if defined(_MSC_VER) && (_MSC_VER >= 1020)\n"
        "#pragma once\n"
        "#endif\n"
        "\n"
        "#if defined(UNICODE) || defined(_UNICODE)\n"
        "#error \"Don't using UNICODE from the main project.\"\n"
        "#endif\n"
        "\n"
        "#ifndef __EMPTY_BLOCK__\n"
        "#define __EMPTY_BLOCK__\n"
        "#endif\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_TITLE_NAME        ${MAIN_NAME}\n"
        "#define ${MAIN_NAME_UPPER}_TITLE_NAME_UPPER  ${MAIN_NAME_UPPER}\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_MAJOR  ${VERSION_MAJOR}\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_MINOR  ${VERSION_MINOR}\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_PATCH  ${VERSION_PATCH}\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_TWEAK  \"${VERSION_TWEAK}\"\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_PACKET_MAJOR  ${VERSION_PACKET_MAJOR}\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_PACKET_MINOR  ${VERSION_PACKET_MINOR}\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_RELEASE       ${VERSION_RELEASE}\n"
        "\n"
        "#ifndef NAMESPACE_${MAIN_NAME_UPPER}\n"
        "#define NAMESPACE_${MAIN_NAME_UPPER} ${MAIN_NAME_UPPER}_TITLE_NAME\n"
        "# if defined(__cplusplus)\n"
        "#  define NAMESPACE_${MAIN_NAME_UPPER}_OPEN   namespace NAMESPACE_${MAIN_NAME_UPPER} {\n"
        "#  define NAMESPACE_${MAIN_NAME_UPPER}_CLOSE  }\n"
        "#  define USING_NAMESPACE_${MAIN_NAME_UPPER}  using namespace NAMESPACE_${MAIN_NAME_UPPER};\n"
        "# else /* __cplusplus */\n"
        "#  define NAMESPACE_${MAIN_NAME_UPPER}_OPEN\n"
        "#  define NAMESPACE_${MAIN_NAME_UPPER}_CLOSE\n"
        "#  define USING_NAMESPACE_${MAIN_NAME_UPPER}\n"
        "# endif /* __cplusplus */\n"
        "#endif\n"
        "\n"
        "#if defined(__cplusplus)\n"
        "NAMESPACE_${MAIN_NAME_UPPER}_OPEN\n"
        "NAMESPACE_${MAIN_NAME_UPPER}_CLOSE\n"
        "#endif /* __cplusplus */\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_VERSION_STRING \\\n"
        "       TO_STRING(${MAIN_NAME_UPPER}_VERSION_MAJOR)  VERSION_SEPARATOR \\\n"
        "       TO_STRING(${MAIN_NAME_UPPER}_VERSION_MINOR)  VERSION_SEPARATOR \\\n"
        "       TO_STRING(${MAIN_NAME_UPPER}_VERSION_PATCH)\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_TITLE_STRING  TO_STRING(${MAIN_NAME_UPPER}_TITLE_NAME)\n"
        "#define ${MAIN_NAME_UPPER}_TITLE_PREFIX  \"[\" ${MAIN_NAME_UPPER}_TITLE_STRING \"]\"\n"
        "\n"
        "#define ${MAIN_NAME_UPPER}_MAIN_TITLE \\\n"
        "       ${MAIN_NAME_UPPER}_TITLE_STRING \" v\" ${MAIN_NAME_UPPER}_VERSION_STRING\n"
        "\n"
        "#endif /* __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__ */\n")
endfunction()

