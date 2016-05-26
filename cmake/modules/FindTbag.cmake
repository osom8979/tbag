## Find the tbag library.
#
# The following variables are optionally searched for defaults
#  TBAG_ROOT
#
# The following are set after configuration is done:
#  TBAG_FOUND
#  TBAG_INCLUDE_DIRS
#  TBAG_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers "libtbag/config.h")

tbag_simple_find_library (TBAG "${_headers}" "")

