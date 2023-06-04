include(cmake/generic_dependency.cmake) # Add library for including external targets
include(cmake/helpers.cmake)

# Source management section
set(GENERIC_HEADER)
set(GENERIC_SOURCE)
set(GENERIC_DEFINITIONS)
option(DEBUG_ENABLED "Debug module" OFF)
if ("${DEBUG_ENABLED}" STREQUAL "1")
    list(APPEND GENERIC_DEFINITIONS " -DDEBUG_ENABLED=1")
    set(DEBUG_ENABLED ON)
else ()
    list(APPEND GENERIC_DEFINITIONS " -DRELEASE=1")
endif ()

add_subdirectory(audioModule)
add_subdirectory(coreModule)
add_subdirectory(databaseModule)
add_subdirectory(profileModule)
add_subdirectory(utilityModule)
add_subdirectory(thirdparty/fairygui)
add_subdirectory(thirdparty/magic_enum)
if (DEBUG_ENABLED)
    message("Debug module enabled.")
    add_subdirectory(debugModule)
endif ()

set(GAME_HEADER ${GAME_HEADER} ${GENERIC_HEADER} PARENT_SCOPE)
set(GAME_SOURCE ${GAME_SOURCE} ${GENERIC_SOURCE} PARENT_SCOPE)
set(GAME_DEFINITIONS ${GAME_DEFINITIONS} ${GENERIC_DEFINITIONS} PARENT_SCOPE)

