
message(STATUS "Looking for xxHash Implementation")

find_path(XXHASH_PATH xxHash/xxhash.h
  PATHS ENV PATH ENV XXHASH_ROOT)

if(XXHASH_PATH)
  message(STATUS "Looking for xxHash - found path: ${XXHASH_PATH}")
  set(XXHASH_FOUND ON)

  set(XXHASH_INCLUDE_DIRS "${XXHASH_PATH}/xxHash/")
else()
  message(STATUS "failed finding xxHash - please create the XXHASH_ROOT environment variable or append it to PATH")
  if (XXHash_FIND_REQUIRED)
    message(FATAL_ERROR "Required package xxHash missing!")
  endif()
endif()
