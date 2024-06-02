add_library(jpodofoobj OBJECT bind.cpp font/notomono-regular.ttf.cpp)

target_include_directories(jpodofoobj PRIVATE ${PODOFO_INCLUDE_DIRS})

add_executable(jpodofo $<TARGET_OBJECTS:jpodofoobj>)
target_link_libraries(jpodofo PRIVATE podofo_static podofo_private)

set(PODOFO_EMCC_FLAGS "--bind -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Podofo -O3 -s EXPORT_ES6=1 -s ENVIRONMENT=web")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(PODOFO_EMCC_FLAGS "${PODOFO_EMCC_FLAGS} -g2 -s ASSERTIONS=2")
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

add_dependencies(jpodofo jpodofoobj)

set(PODOFO_COMMON_EMCC_FLAGS "--bind -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Podofo -O3 -s ENVIRONMENT=web")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(PODOFO_COMMON_EMCC_FLAGS "${PODOFO_COMMON_EMCC_FLAGS} -g2 -s ASSERTIONS=2")
endif()

add_executable(jpodofoc $<TARGET_OBJECTS:jpodofoobj>)
target_link_libraries(jpodofoc PRIVATE podofo_static podofo_private)

set_target_properties(
    jpodofoc
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINK_FLAGS ${PODOFO_COMMON_EMCC_FLAGS} 
        OUTPUT_NAME "podofo.common"
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_dependencies(jpodofoc jpodofoobj)
