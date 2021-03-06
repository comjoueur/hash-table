
find_library(
  NUMA_LIBRARIES
  NAMES numa
)

find_path(
  NUMA_INCLUDE_DIRS
  NAMES numa.h
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
  NUMA DEFAULT_MSG
  NUMA_LIBRARIES
  NUMA_INCLUDE_DIRS
)

mark_as_advanced(
  NUMA_LIBRARIES
  NUMA_INCLUDE_DIRS
)

if (NOT NUMA_FOUND)
  message(STATUS "Failed finding libnuma - you may install it using the package manager of your choice!")
endif()
