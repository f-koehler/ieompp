find_package(IWYU REQUIRED)
add_custom_target(iwyu)

function(add_iwyu_analysis ANALYSIS SOURCE)
    get_property(INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(INCLUDE_DIR ${INCLUDE_DIRS})
        list(APPEND INCLUDE_FLAGS "-I${INCLUDE_DIR}")
    endforeach()
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${ANALYSIS}
        COMMAND ${IWYU_EXECUTABLE} -std=c++14 ${INCLUDE_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} &> ${CMAKE_CURRENT_BINARY_DIR}/${ANALYSIS} || true
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}
    )
    add_custom_target(${ANALYSIS} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${ANALYSIS})
    add_dependencies(iwyu ${ANALYSIS})
endfunction()
