## Find the tbag library.
#
# The following variables are optionally searched for defaults
#  TBAG_ROOT
#
# The following are set after configuration is done:
#  TBAG_FOUND
#  TBAG_INCLUDE_DIRS
#  TBAG_LIBRARIES

set (__prefix   TBAG)
set (__headers  "libtbag/config.h")
set (__libs     "")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

