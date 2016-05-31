## Find the OpenBLAS library.
#
# The following variables are optionally searched for defaults
#  HDF5v2_ROOT
#
# The following are set after configuration is done:
#  HDF5v2_FOUND
#  HDF5v2_INCLUDE_DIRS
#  HDF5v2_LIBRARIES
#  HDF5v2_LIBRARY_C
#  HDF5v2_LIBRARY_HL
#
# Default HDF5 settings:
#  <code>
#    set (HDF5_FIND_COMPONENTS C HL)
#    find_package (HDF5)
#  </code>

if ((WIN32 AND MINGW) AND NOT CYGWIN AND NOT MSYS)
    # Don't use CMAKE_STATIC_LIBRARY_PREFIX variable.
    set (__hdf5_lib_c_name  "libhdf5")
    set (__hdf5_lib_hl_name "libhdf5_hl")
else ()
    set (_hdf5_lib_c_name  "hdf5")
    set (__hdf5_lib_hl_name "hdf5_hl")
endif ()

set (__headers "hdf5.h")
set (__libs    "${__hdf5_lib_hl_name}" "${__hdf5_lib_c_name}")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

if (HDF5v2_FOUND)
    foreach (__cursor ${HDF5v2_LIBRARIES})
        string (REGEX MATCH "${__hdf5_lib_c_name}\\.(a|so|lib|dll\\.a|dylib)"  __temp_c  "${__cursor}")
        string (REGEX MATCH "${__hdf5_lib_hl_name}\\.(a|so|lib|dll\\.a|dylib)" __temp_hl "${__cursor}")

        if (__temp_c)
            set (HDF5v2_LIBRARY_C  ${__cursor})
            message (STATUS "HDF5v2_LIBRARY_C: ${HDF5v2_LIBRARY_C}")
        endif ()
        if (__temp_hl)
            set (HDF5v2_LIBRARY_HL ${__cursor})
            message (STATUS "HDF5v2_LIBRARY_HL: ${HDF5v2_LIBRARY_HL}")
        endif ()
    endforeach ()
endif ()

