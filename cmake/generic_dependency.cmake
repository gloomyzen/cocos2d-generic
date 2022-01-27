# General dependency target
SET(GENERIC_LIB_NAME  "generic_dependency" CACHE INTERNAL "GENERIC_LIB_NAME")
add_library(${GENERIC_LIB_NAME} ${CMAKE_SOURCE_DIR}/lib/empty.h ${CMAKE_SOURCE_DIR}/lib/empty.cpp)

set_target_properties(${GENERIC_LIB_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )

if (POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()