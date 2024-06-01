# clang-format support
function(add_clang_format_target)
    if(NOT ${PROJECT_NAME}_CLANG_FORMAT_BINARY)
			find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
    endif()

    message(STATUS ${ALL_SOURCES} ${ALL_HEADERS})

    if(${PROJECT_NAME}_CLANG_FORMAT_BINARY)
      add_custom_target(clang-format
          COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY} --verbose
          -i ${ALL_SOURCES} ${ALL_HEADERS}
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

			message(STATUS "Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format).\n")
    endif()
endfunction()
