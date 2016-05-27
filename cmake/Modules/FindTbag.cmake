## Find the tbag library.
#
# The following variables are optionally searched for defaults
#  Tbag_ROOT
#
# The following are set after configuration is done:
#  Tbag_FOUND
#  Tbag_INCLUDE_DIRS
#  Tbag_LIBRARIES

set (__headers  "libtbag/config.h")
set (__libs     "")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

