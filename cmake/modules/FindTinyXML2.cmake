## Find the TinyXML2 library.
#
# The following variables are optionally searched for defaults
#  TINYXML2_ROOT
#
# The following are set after configuration is done:
#  TINYXML2_FOUND
#  TINYXML2_INCLUDE_DIRS
#  TINYXML2_LIBRARIES

include (SimpleFindLibrary)

set (_headers  "tinyxml2.h")
set (_libs     "tinyxml2")

simple_find_library (TINYXML2 "${_headers}" "${_libs}")

