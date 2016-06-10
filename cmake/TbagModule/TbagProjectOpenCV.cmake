#/// @file   TbagProjectOpenCV.cmake
#/// @brief  OpenCV project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT OpenCV_FOUND)
    message (WARNING "Not found OpenCV.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenCV_LIBS})

