## Find the GLES library.
#
# The following variables are optionally searched for defaults
#  GLES_ROOT
#
# The following are set after configuration is done:
#  GLES_FOUND
#  GLES_INCLUDE_DIRS
#  GLES_LIBRARIES

if (GLES_FOUND)
    return ()
endif ()

set (__headers "GLES/gl.h")
set (__libs    "GLESv1_CM")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

