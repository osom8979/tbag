## Find the OpenBLAS library.
#
# The following variables are optionally searched for defaults
#  OPENBLAS_ROOT
#
# The following are set after configuration is done:
#  OPENBLAS_FOUND
#  OPENBLAS_INCLUDE_DIRS
#  OPENBLAS_LIBRARIES

set (__prefix   OPENBLAS)
set (__headers  "openblas_config.h")
set (__libs     "openblas")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

