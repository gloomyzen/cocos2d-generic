function(GetCurrentSourcePath result)
    string(APPEND MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" MODULE_PATH ${MODULE_PATH})
    SET(${result} ${MODULE_PATH} PARENT_SCOPE)
endfunction()

function(MakeReadableListOfSources sources)
    set(result)
    foreach(source ${${sources}})
        set(item ${source})
        string(REPLACE "${CMAKE_SOURCE_DIR}/" "" item ${item})
        list(APPEND result
                ${item}
                )
    endforeach()
    SET(${sources} ${result} PARENT_SCOPE)
endfunction()

function(AddCurrentSourcesPathToList sources)
    set(imgui_sources)
    foreach(source ${${sources}})
        list(APPEND imgui_sources
            ${CMAKE_CURRENT_SOURCE_DIR}/${source}
        )
    endforeach()
    SET(${sources} ${imgui_sources} PARENT_SCOPE)
endfunction()