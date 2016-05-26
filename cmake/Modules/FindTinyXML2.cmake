## Find the TinyXML2 library.
#
# The following variables are optionally searched for defaults
#  TINYXML2_ROOT
#
# The following are set after configuration is done:
#  TINYXML2_FOUND
#  TINYXML2_INCLUDE_DIRS
#  TINYXML2_LIBRARIES

set (__prefix   TINYXML2)
set (__headers  "tinyxml2.h")
set (__libs     "tinyxml2")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

