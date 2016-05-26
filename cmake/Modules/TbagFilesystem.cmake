#/// @file   TbagFilesystem.cmake
#/// @brief  filesystem utilities.
#/// @author zer0
#/// @date   2016-05-26

if ((WIN32) AND (NOT MSYS) AND (NOT CYGWIN))
    set (PATH_SEPARATOR ";")
else ()
    set (PATH_SEPARATOR ":")
endif ()

