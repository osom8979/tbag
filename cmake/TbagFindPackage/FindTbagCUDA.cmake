#/// @file   FindTbagCUDA.cmake
#/// @brief  Find the CUDA compiler & libraries.
#/// @author zer0
#/// @date   2016-06-09
#///
#/// @see <https://github.com/BVLC/caffe/blob/master/cmake/Cuda.cmake>
#///
#/// @remarks
#/// The following variables are optionally searched for defaults:
#///  - ${CUDA_64_BIT_DEVICE_CODE}
#///  - ${CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE}
#///  - ${CUDA_BUILD_CUBIN}
#///  - ${CUDA_BUILD_EMULATION}
#///  - ${CUDA_GENERATED_OUTPUT_DIR}
#///  - ${CUDA_HOST_COMPILATION_CPP}
#///  - ${CUDA_HOST_COMPILER}
#///  - ${CUDA_NVCC_FLAGS}
#///  - ${CUDA_NVCC_FLAGS_<CONFIG>}
#///  - ${CUDA_PROPAGATE_HOST_FLAGS}    (Default ON) ${CMAKE_{C,CXX}_FLAGS} to -Xcompiler flag.
#///  - ${CUDA_SEPARABLE_COMPILATION}
#///  - ${CUDA_VERBOSE_BUILD}
#///
#/// The following are set after configuration is done:
#///  - ${CUDA_VERSION_MAJOR}
#///  - ${CUDA_VERSION_MINOR}
#///  - ${CUDA_VERSION}
#///  - ${CUDA_VERSION_STRING}
#///  - ${CUDA_TOOLKIT_ROOT_DIR}   Path to the CUDA Toolkit (defined if not set).
#///  - ${CUDA_SDK_ROOT_DIR}       Path to the CUDA SDK.
#///  - ${CUDA_INCLUDE_DIRS}
#///  - ${CUDA_LIBRARIES}          Cuda RT library.
#///  - ${CUDA_CUFFT_LIBRARIES}    Device or emulation library for the Cuda FFT implementation.
#///  - ${CUDA_CUBLAS_LIBRARIES}   Device or emulation library for the Cuda BLAS implementation.
#///  - ${CUDA_cupti_LIBRARY}      CUDA Profiling Tools Interface library. Only available for CUDA version 4.0+.
#///  - ${CUDA_curand_LIBRARY}     CUDA Random Number Generation library. Only available for CUDA version 3.2+.
#///  - ${CUDA_cusparse_LIBRARY}   CUDA Sparse Matrix library. Only available for CUDA version 3.2+.
#///  - ${CUDA_npp_LIBRARY}        NVIDIA Performance Primitives library. Only available for CUDA version 4.0+.
#///  - ${CUDA_nppc_LIBRARY}       NVIDIA Performance Primitives library (core). Only available for CUDA version 5.5+.
#///  - ${CUDA_nppi_LIBRARY}       NVIDIA Performance Primitives library (image processing). Only available for CUDA version 5.5+.
#///  - ${CUDA_npps_LIBRARY}       NVIDIA Performance Primitives library (signal processing). Only available for CUDA version 5.5+.
#///  - ${CUDA_nvcuvenc_LIBRARY}   CUDA Video Encoder library. Only available for CUDA version 3.2+. Windows only.
#///  - ${CUDA_nvcuvid_LIBRARY}    CUDA Video Decoder library. Only available for CUDA version 3.2+. Windows only.

# Known NVIDIA GPU achitectures.
# This list will be used for 'CUDA_ARCH_NAME = All' option.
set (TBAG_CUDA_KNOWN_GPU_ARCHS "20 21(20) 30 35 50 52 61")

#/// A function for automatic detection of GPUs installed  (if autodetection is enabled)
#///
#/// @param __result [out] Standard output result variable.
function (tabg_cuda__find_detect_arch __result)
    if (NOT EXISTS "${CUDA_NVCC_EXECUTABLE}")
        message (FATAL_ERROR "Not found nvcc executable: ${CUDA_NVCC_EXECUTABLE}")
    endif ()

    if (NOT __gpu_detect_output)
        set (__cuda_source_file "${PROJECT_BINARY_DIR}/tabg_cuda__find_detect_arch.cu")

        file (WRITE ${__cuda_source_file} ""
                "#include <cstdio>"
                "\n"
                "int main()"
                "{"
                "    int count = 0;"
                "    if (cudaSuccess != cudaGetDeviceCount(&count)) {"
                "        return -1;"
                "    }"
                "    if (count == 0) {"
                "        return -1;"
                "    }"
                "    for (int device = 0; device < count; ++device) {"
                "        cudaDeviceProp prop;"
                "        if (cudaSuccess == cudaGetDeviceProperties(&prop, device)) {"
                "            std::printf(\"%d.%d \", prop.major, prop.minor);"
                "        }"
                "    }"
                "    return 0;"
                "}")

        execute_process (COMMAND "${CUDA_NVCC_EXECUTABLE}" "--run" "${__cuda_source_file}" "-ccbin" "${CMAKE_CXX_COMPILER}"
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/CMakeFiles/"
                RESULT_VARIABLE __nvcc_exit_code
                OUTPUT_VARIABLE __nvcc_output
                ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

        if (__nvcc_exit_code EQUAL 0)
            string (REPLACE "2.1" "2.1(2.0)" __nvcc_output "${__nvcc_output}")
            set (__gpu_detect_output ${__nvcc_output} CACHE INTERNAL "Returned GPU architetures" FORCE)
        endif ()
    endif ()

    if (NOT __gpu_detect_output)
        message (STATUS "Automatic GPU detection failed. Building for all known architectures.")
        set (${__result} ${TBAG_CUDA_KNOWN_GPU_ARCHS})
    else ()
        set (${__result} ${__gpu_detect_output})
    endif ()

    # Update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Function for selecting GPU arch flags for nvcc based on CUDA_ARCH_NAME
#///
#/// @param __result [out] Output result variable.
function (tabg_cuda__select_nvcc_arch_flags __result)
    # List of arch names
    set (__archs_names "Fermi" "Kepler" "Maxwell" "Pascal" "All" "Manual" "Local")
    set (__archs_name_default "All")
    if (NOT CMAKE_CROSSCOMPILING)
        list (APPEND __archs_names "Auto")
        set (__archs_name_default "Auto")
    endif ()

    # set CUDA_ARCH_NAME strings (so it will be seen as dropbox in CMake-Gui)
    set (CUDA_ARCH_NAME ${__archs_name_default} CACHE STRING "Select target NVIDIA GPU achitecture.")
    set_property (CACHE CUDA_ARCH_NAME PROPERTY STRINGS "" ${__archs_names} )
    mark_as_advanced (CUDA_ARCH_NAME)

    # verify CUDA_ARCH_NAME value
    if (NOT ";${__archs_names};" MATCHES ";${CUDA_ARCH_NAME};")
        string (REPLACE ";" ", " __archs_names "${__archs_names}")
        message (FATAL_ERROR "Only ${__archs_names} architeture names are supported.")
    endif ()

    if (${CUDA_ARCH_NAME} STREQUAL "Manual")
        set (CUDA_ARCH_BIN ${TBAG_CUDA_KNOWN_GPU_ARCHS} CACHE STRING "Specify 'real' GPU architectures to build binaries for, BIN(PTX) format is supported")
        set (CUDA_ARCH_PTX "50"                         CACHE STRING "Specify 'virtual' PTX architectures to build PTX intermediate code for")
        mark_as_advanced (CUDA_ARCH_BIN CUDA_ARCH_PTX)
    else ()
        unset (CUDA_ARCH_BIN CACHE)
        unset (CUDA_ARCH_PTX CACHE)
    endif ()

    if (${CUDA_ARCH_NAME} STREQUAL "Fermi")
        set (__cuda_arch_bin "20 21(20)")
    elseif (${CUDA_ARCH_NAME} STREQUAL "Kepler")
        set (__cuda_arch_bin "30 35")
    elseif (${CUDA_ARCH_NAME} STREQUAL "Maxwell")
        set (__cuda_arch_bin "50")
    elseif (${CUDA_ARCH_NAME} STREQUAL "Pascal")
        set (__cuda_arch_bin "61")
    elseif (${CUDA_ARCH_NAME} STREQUAL "All")
        set (__cuda_arch_bin ${TBAG_CUDA_KNOWN_GPU_ARCHS})
    elseif (${CUDA_ARCH_NAME} STREQUAL "Auto")
        tabg_cuda__find_detect_arch (__cuda_arch_bin)
    elseif (${CUDA_ARCH_NAME} STREQUAL "Manual")
        set (__cuda_arch_bin ${CUDA_ARCH_BIN})
    else () # (${CUDA_ARCH_NAME} STREQUAL "Local")
        set (__cuda_arch_bin ${CUDA_ARCH_BIN})
        set (${__result}          "" PARENT_SCOPE)
        set (${__result}_readable "" PARENT_SCOPE)
        return ()
    endif ()

    # remove dots and convert to lists
    string (REGEX REPLACE "\\." "" __cuda_arch_bin "${__cuda_arch_bin}")
    string (REGEX REPLACE "\\." "" __cuda_arch_ptx "${CUDA_ARCH_PTX}")
    string (REGEX MATCHALL "[0-9()]+" __cuda_arch_bin "${__cuda_arch_bin}")
    string (REGEX MATCHALL "[0-9]+"   __cuda_arch_ptx "${__cuda_arch_ptx}")
    list (REMOVE_DUPLICATES __cuda_arch_bin)
    list (REMOVE_DUPLICATES __cuda_arch_ptx)

    set (__nvcc_flags "")
    set (__nvcc_archs_readable "")

    # Tell NVCC to add binaries for the specified GPUs
    foreach (__arch ${__cuda_arch_bin})
        if (__arch MATCHES "([0-9]+)\\(([0-9]+)\\)")
            # User explicitly specified PTX for the concrete BIN
            list (APPEND __nvcc_flags -gencode arch=compute_${CMAKE_MATCH_2},code=sm_${CMAKE_MATCH_1})
            list (APPEND __nvcc_archs_readable sm_${CMAKE_MATCH_1})
        else ()
            # User didn't explicitly specify PTX for the concrete BIN, we assume PTX=BIN
            list (APPEND __nvcc_flags -gencode arch=compute_${__arch},code=sm_${__arch})
            list (APPEND __nvcc_archs_readable sm_${__arch})
        endif ()
    endforeach ()

    # Tell NVCC to add PTX intermediate code for the specified architectures
    foreach (__arch ${__cuda_arch_ptx})
        list (APPEND __nvcc_flags -gencode arch=compute_${__arch},code=compute_${__arch})
        list (APPEND __nvcc_archs_readable compute_${__arch})
    endforeach ()

    string (REPLACE ";" " " __nvcc_archs_readable "${__nvcc_archs_readable}")
    set (${__result}          ${__nvcc_flags}          PARENT_SCOPE)
    set (${__result}_readable ${__nvcc_archs_readable} PARENT_SCOPE)
endfunction ()

############ FIND ############
find_package (CUDA 5.5 QUIET)#
##############################

if (NOT CUDA_FOUND)
    return ()
endif ()

# cmake 2.8.7 compartibility which doesn't search for curand.
find_cuda_helper_libs (curand)

# setting nvcc arch flags
tabg_cuda__select_nvcc_arch_flags (NVCC_FLAGS_EXTRA)
list (APPEND CUDA_NVCC_FLAGS ${NVCC_FLAGS_EXTRA})
message (STATUS "Added CUDA NVCC flags for: ${NVCC_FLAGS_EXTRA_readable}")

# Boost 1.55 workaround, see https://svn.boost.org/trac/boost/ticket/9392 or
# https://github.com/ComputationalRadiationPhysics/picongpu/blob/master/src/picongpu/CMakeLists.txt
#if (Boost_VERSION EQUAL 105500)
#    message (STATUS "Cuda + Boost 1.55: Applying noinline work around.")
#    # avoid warning for CMake >= 2.8.12
#    set (CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} \"-DBOOST_NOINLINE=__attribute__((noinline))\" ")
#endif ()

## See: cmake revision 48040c1 - Merge topic 'FindCUDA.cmake/C++11Flags'
if (CMAKE_VERSION VERSION_LESS "3.2.2")
    # Process the C++11 flag.
    # cc1: warning: command line option ‘-std=c++11’ is valid for C++/ObjC++ but not for C
    if ("${CMAKE_CXX_FLAGS}" MATCHES "-std=c\\+\\+11")
        # Add the c++11 flag to nvcc if it isn't already present.  Note that we only look at
        # the main flag instead of the configuration specific flags.
        if (NOT "${CUDA_NVCC_FLAGS}" MATCHES "-std;c\\+\\+11" )
            list (APPEND CUDA_NVCC_FLAGS --std c++11)
        endif ()
    endif ()
endif ()

