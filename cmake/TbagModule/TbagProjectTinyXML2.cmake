#/// @file   TbagProjectTinyXML2.cmake
#/// @brief  libtinyxml2 project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT TinyXML2_FOUND)
    find_package (TinyXML2)
endif ()

if (TinyXML2_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_TINYXML2)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TinyXML2_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${TinyXML2_LIBRARIES})
endif ()

