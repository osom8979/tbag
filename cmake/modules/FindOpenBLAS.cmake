## Find the OpenBLAS library.
#
# The following variables are optionally searched for defaults
#  OPENBLAS_ROOT
#
# The following are set after configuration is done:
#  OPENBLAS_FOUND
#  OPENBLAS_INCLUDE_DIRS
#  OPENBLAS_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "openblas_config.h")
set (_libs     "openblas")

tbag_simple_find_library (OPENBLAS "${_headers}" "${_libs}")

