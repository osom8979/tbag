## Find the TinyXML2 library.
#
# The following variables are optionally searched for defaults
#  TinyXML2_ROOT
#
# The following are set after configuration is done:
#  TinyXML2_FOUND
#  TinyXML2_INCLUDE_DIRS
#  TinyXML2_LIBRARIES

set (__headers "tinyxml2.h")
set (__libs    "tinyxml2")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

