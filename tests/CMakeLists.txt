find_program(NPM_EXECUTABLE npm REQUIRED)

if(NPM_EXECUTABLE)
    message(STATUS "npm found: ${NPM_EXECUTABLE}")

    add_custom_target(
        test
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.js ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.wasm ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${NPM_EXECUTABLE} install
        COMMAND ${NPM_EXECUTABLE} test
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
        COMMENT "Running test using npm"
    )

    add_dependencies(test jpodofo)
else()
    message(WARNING "npm not found. Please install Node.js and npm.")
endif()