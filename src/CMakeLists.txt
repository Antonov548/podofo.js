add_executable(jpodofo bind.cpp)

set(PODOFO_EMCC_FLAGS "--bind -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Podofo -O3")
if (DEBUG)
    set(PODOFO_EMCC_FLAGS "${IGRAPH_EMCC_FLAGS} -g2 -s ASSERTIONS=2")
endif()

set_target_properties(
    jpodofo
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINK_FLAGS ${PODOFO_EMCC_FLAGS}
        OUTPUT_NAME "podofo"
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

target_include_directories(jpodofo PRIVATE ${PODOFO_INCLUDE_DIRS})
target_link_libraries(jpodofo PRIVATE podofo_static podofo_private)