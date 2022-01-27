set(GENERIC_HEADER)
set(GENERIC_SOURCE)

add_subdirectory(audioModule)
add_subdirectory(databaseModule)
add_subdirectory(profileModule)
add_subdirectory(utilityModule)

set(GAME_HEADER ${GAME_HEADER} ${GENERIC_HEADER} PARENT_SCOPE)
set(GAME_SOURCE ${GAME_SOURCE} ${GENERIC_SOURCE} PARENT_SCOPE)
