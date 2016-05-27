## Find the libuv library.
#
# The following variables are optionally searched for defaults
#  UV_ROOT
#
# The following are set after configuration is done:
#  UV_FOUND
#  UV_INCLUDE_DIRS
#  UV_LIBRARIES

set (__headers  "uv.h")
set (__libs     "uv")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

