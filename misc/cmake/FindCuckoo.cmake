
message(STATUS "Looking for libcuckoo")

find_path(CUCKOO_PATH_FIRST include/libcuckoo/cuckoohash_map.hh
  PATHS ENV PATH ENV LIBCUCKOO_ROOT)


if (CUCKOO_PATH_FIRST)
  set (CUCKOO_PATH "${CUCKOO_PATH_FIRST}")
else()
  find_path(CUCKOO_PATH_SECOND libcuckoo/install/include/libcuckoo/cuckoohash_map.hh
    PATHS ENV PATH ENV LIBCUCKOO_ROOT)
  if(CUCKOO_PATH_SECOND)
    set(CUCKOO_PATH "${CUCKOO_PATH_SECOND}/libcuckoo/install/")
  endif()
endif()


if(CUCKOO_PATH)
  message(STATUS "Looking for libcuckoo - found path: ${CUCKOO_PATH}")
  set(CUCKOO_FOUND ON)

  set(CUCKOO_INCLUDE_DIRS "${CUCKOO_PATH}/include/")
else()
  message(STATUS "failed finding libcuckoo - please create the LIBCUCKOO_ROOT environment variable or append it to PATH")
  if (Cuckoo_FIND_REQUIRED)
    message(FATAL_ERROR "Required package libcuckoo missing!")
  endif()
endif()
