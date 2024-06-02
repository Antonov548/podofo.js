if(NPM_EXECUTABLE)
    add_custom_target(
        npm
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.js ${CMAKE_CURRENT_SOURCE_DIR}/podofo.mjs
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.wasm ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${NPM_EXECUTABLE} install
        COMMAND ${NPM_EXECUTABLE} pack
        COMMENT "Install podofo.js"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/npm
    )

    add_dependencies(npm jpodofo)
endif()
