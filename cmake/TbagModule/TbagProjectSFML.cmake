#/// @file   TbagProjectSFML.cmake
#/// @brief  SFML project module.
#/// @author zer0
#/// @date   2016-08-29

if (NOT SFML_FOUND)
    message (WARNING "Not found SFML.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
list (APPEND TBAG_PROJECT_LDFLAGS      ${SFML_DEPENDENCIES} ${SFML_LIBRARIES})

if (UNIX AND APPLE)
    list (APPEND TBAG_PROJECT_LDFLAGS  -Wl,-rpath,${SFML_DEPENDENCIES} ${SFML_LIBRARIES})
endif ()

