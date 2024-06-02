if (NPM_EXECUTABLE)
    message(STATUS "npm found: ${NPM_EXECUTABLE}")

    add_custom_target(
        test
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.node.js ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/podofo.node.wasm ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${NPM_EXECUTABLE} install
        COMMAND ${NPM_EXECUTABLE} test
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
        COMMENT "Running test using npm"
    )

    add_executable(jpodofo.node $<TARGET_OBJECTS:jpodofoobj>)
    target_link_libraries(jpodofo.node PRIVATE podofo_static podofo_private)

    set(PODOFO_NODE_EMCC_FLAGS "--bind -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Podofo -O3 -s ENVIRONMENT=node")
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(PODOFO_NODE_EMCC_FLAGS "${PODOFO_NODE_EMCC_FLAGS} -g2 -s ASSERTIONS=2")
    endif()

    set_target_properties(
        jpodofo.node
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            LINK_FLAGS ${PODOFO_NODE_EMCC_FLAGS} 
            OUTPUT_NAME "podofo.node"
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )

    add_dependencies(jpodofo.node jpodofoobj)
    add_dependencies(test jpodofo.node)
endif()
