#/// @file   TbagProjectBoost.cmake
#/// @brief  Boost project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT Boost_FOUND)
    find_package (Boost)
endif ()

if (Boost_FOUND)
    #message (STATUS "Boost system: ${Boost_SYSTEM_FOUND}")
    #message (STATUS "Boost thread: ${Boost_THREAD_FOUND}")
    #message (STATUS "Boost python: ${Boost_PYTHON_FOUND}")
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${Boost_LIBRARIES})
endif ()

