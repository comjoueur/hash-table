
message(STATUS "Looking for smhasher Implementation")

find_path(SMHASHER_PATH smhasher/src/MurmurHash3.cpp
  PATHS ENV PATH ENV SMHASHER_ROOT)

if(SMHASHER_PATH)
  message(STATUS "Looking for smhasher - found path: ${SMHASHER_PATH}")
  set(SMHASHER_FOUND ON)

  set(SMHASHER_INCLUDE_DIRS "${SMHASHER_PATH}/smhasher/src/")
else()
  message(STATUS "failed finding smhasher - please create the SMHASHER_ROOT environment variable or append it to PATH")
  if (smhasher_FIND_REQUIRED)
    message(FATAL_ERROR "Required package smhasher missing!")
  endif()
endif()
