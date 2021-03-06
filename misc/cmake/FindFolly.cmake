
message(STATUS "Looking for Folly")

find_path   (FOLLY_PATH "include/folly/AtomicHashMap.h"
             ENV PATH ENV FOLLY_ROOT)
find_library(FOLLY_LIBRARIES folly
             ${FOLLY_PATH}/lib/)

if(FOLLY_PATH)
  message(STATUS "Looking for Folly - found path: ${FOLLY_PATH}")
  set(FOLLY_FOUND ON)

  set(FOLLY_INCLUDE_DIRS "${FOLLY_PATH}/include/")
  set(FOLLY_LIBRARY_DIRS "${FOLLY_PATH}/lib/")
else()
  message(STATUS "failed finding Folly - please create the FOLLY_ROOT environment variable or append it to PATH")
  if (FOLLY_FIND_REQUIRED)
    message(FATAL_ERROR "Required package FOLLY missing!")
  endif()
endif()
