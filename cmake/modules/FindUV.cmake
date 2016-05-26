## Find the libuv library.
#
# The following variables are optionally searched for defaults
#  UV_ROOT
#
# The following are set after configuration is done:
#  UV_FOUND
#  UV_INCLUDE_DIRS
#  UV_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "uv.h")
set (_libs     "uv")

tbag_simple_find_library (UV "${_headers}" "${_libs}")

