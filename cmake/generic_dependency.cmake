# General dependency target
SET(GENERIC_LIB_NAME  "generic_dependency" CACHE INTERNAL "GENERIC_LIB_NAME")
add_library(${GENERIC_LIB_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.h ${CMAKE_CURRENT_SOURCE_DIR}/cmake/empty.cpp)

set_target_properties(${GENERIC_LIB_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        )

if (POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()