#/// @file   TbagProjectHDF5v2.cmake
#/// @brief  HDF5v2 project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT HDF5v2_FOUND)
    find_package (HDF5v2)
endif ()

if (HDF5v2_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_HDF5)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${HDF5v2_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${HDF5v2_LIBRARIES}) # HL, C
endif ()

