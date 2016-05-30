## Find the OpenBLAS library.
#
# The following variables are optionally searched for defaults
#  OpenBLAS_ROOT
#
# The following are set after configuration is done:
#  OpenBLAS_FOUND
#  OpenBLAS_INCLUDE_DIRS
#  OpenBLAS_LIBRARIES

set (__headers "openblas_config.h")
set (__libs    "openblas")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

