#/// @file   TbagProjectHDF5v2.cmake
#/// @brief  HDF5v2 project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT HDF5v2_FOUND)
    message (WARNING "Not found HDF5.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${HDF5v2_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${HDF5v2_LIBRARIES}) # HL, C

