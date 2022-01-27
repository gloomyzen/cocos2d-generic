include(cmake/generic_dependency.cmake) # Add library for including external targets
include(cmake/helpers.cmake)

if (${DEBUG})
    add_definitions(-DDEBUG=1)
else ()
    add_definitions(-DRELEASE=1)
endif ()

# Source management section
set(GENERIC_HEADER)
set(GENERIC_SOURCE)

add_subdirectory(audioModule)
add_subdirectory(coreModule)
add_subdirectory(databaseModule)
add_subdirectory(debugModule)
add_subdirectory(profileModule)
add_subdirectory(utilityModule)

set(GAME_HEADER ${GAME_HEADER} ${GENERIC_HEADER} PARENT_SCOPE)
set(GAME_SOURCE ${GAME_SOURCE} ${GENERIC_SOURCE} PARENT_SCOPE)

macro(finalizeInitGeneric)
    initDebugModule()
endmacro()
