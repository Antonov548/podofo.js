find_program(NPM_EXECUTABLE npm REQUIRED)

if (NPM_EXECUTABLE)
    message(STATUS "npm found: ${NPM_EXECUTABLE}")
else()
    message(WARNING "npm not found. Please install Node.js and npm.")
endif()
