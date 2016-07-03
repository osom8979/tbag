## Find the ICU library.
#
# The following variables are optionally searched for defaults
#  ICU_ROOT
#
# The following are set after configuration is done:
#  ICU_FOUND
#  ICU_INCLUDE_DIRS
#  ICU_LIBRARIES

set (__headers "unicode/utf.h")
set (__libs    "icuuc" "icui18n" "icudata")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

