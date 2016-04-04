## CMake configure script.

#! Add a library value.
#
# \param _name        [in] Library name.
# \param _enable      [in] Enable(ON) or disable(OFF).
macro (add_library_value _name _enable)
    set (USE_${_name} ${_enable} CACHE BOOL "Use the ${_name} library.")
endmacro ()

# --------------------
# Solution properties.
# --------------------

# Information.
set (MAIN_NAME   "libtbag"             CACHE STRING  "Solution name string.")
set (MAIN_AUTHOR "zer0"                CACHE STRING  "Solution author.")
set (MAIN_EMAIL  "osom8979@gmail.com"  CACHE STRING  "Author E-mail address.")
set (MAIN_BRIEF  "THIRD-PARTY EXTENSION UTILITY PROJECT" CACHE STRING  "Solution brief string.")

# Libraries.
add_library_value ("zlib"      OFF)
add_library_value ("gtest"     ON)
add_library_value ("protobuf"  OFF)
# Extension project:
add_library_value ("libuv"     ON)
add_library_value ("tinyxml2"  ON)

# Continuous Integration.
option (USE_DOXYGEN "API documentation." ON)

# 3rd-party library prefix.
set (THIRD_PREFIX "$ENV{TPARTY_HOME}/local" CACHE STRING "3rd-party library prefix")

# Main version.
set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_PATCH  0)

# Packet version.
set (VERSION_PACKET_MAJOR  0)
set (VERSION_PACKET_MINOR  0)

# Extension version number.
set (VERSION_RELEASE  0)

