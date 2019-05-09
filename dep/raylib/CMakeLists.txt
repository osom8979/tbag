## CMake build script.
cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

project (raylib C)

set (SRCS core.c
          models.c
          raudio.c
          shapes.c
          text.c
          textures.c
          utils.c)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options (/w)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-w)
endif ()

add_definitions (-DPLATFORM_DESKTOP)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
else ()
    set (CMAKE_C_STANDARD 99)
endif ()

if (APPLE)
    # Need to force OpenGL 3.3 on OS X
    # See: https://github.com/raysan5/raylib/issues/341
    set (GRAPHICS "GRAPHICS_API_OPENGL_33")
    find_library (OPENGL_LIBRARY OpenGL)
    if (NOT CMAKE_SYSTEM STRLESS "Darwin-18.0.0")
        add_definitions (-DGL_SILENCE_DEPRECATION)
        # (AUTHOR_WARNING "OpenGL is deprecated starting with macOS 10.14 (Mojave)!")
    endif ()
elseif (WIN32)
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
else()
    find_library (pthread NAMES pthread)
    find_package (OpenGL QUIET)
    if ("${OPENGL_LIBRARIES}" STREQUAL "")
        set (OPENGL_LIBRARIES "GL")
    endif()
    if ("${CMAKE_SYSTEM_NAME}" MATCHES "(Net|Open)BSD")
        find_library (OSS_LIBRARY ossaudio)
    endif()
endif()

if (${OPENGL_VERSION})
    set (${SUGGESTED_GRAPHICS} "${GRAPHICS}")
    if (${OPENGL_VERSION} MATCHES "3.3")
        set (GRAPHICS "GRAPHICS_API_OPENGL_33")
    elseif (${OPENGL_VERSION} MATCHES "2.1")
        set (GRAPHICS "GRAPHICS_API_OPENGL_21")
    elseif (${OPENGL_VERSION} MATCHES "1.1")
        set (GRAPHICS "GRAPHICS_API_OPENGL_11")
    elseif (${OPENGL_VERSION} MATCHES "ES 2.0")
        set (GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    endif ()
    if ("${SUGGESTED_GRAPHICS}" AND NOT "${SUGGESTED_GRAPHICS}" STREQUAL "${GRAPHICS}")
        message (WARNING "You are overriding the suggested GRAPHICS=${SUGGESTED_GRAPHICS} with ${GRAPHICS}! This may fail")
    endif ()
endif ()

if (NOT GRAPHICS)
    set (GRAPHICS "GRAPHICS_API_OPENGL_33")
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${glfw_EXT_INCLUDE_DIR})
link_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_library (raylib STATIC ${SRCS})
add_dependencies (raylib glfw)

