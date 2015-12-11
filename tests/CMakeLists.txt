include_directories(${CMAKE_SOURCE_DIR}/external/catch/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(unittests
	operator
    term
)

foreach(unittest ${unittests})
    add_executable(${unittest} "test_${unittest}.cpp")
    if(hubbard_ENABLE_COVERAGE)
        target_link_libraries(${unittest} gcov)
        get_target_property(TEMP ${unittest} COMPILE_FLAGS)
        if(${TEMP} STREQUAL "TEMP-NOTFOUND")
            set(TEMP "")
        endif()
        set(TEMP "${TEMP} -fprofile-arcs -ftest-coverage")
        set_target_properties(${unittest} PROPERTIES COMPILE_FLAGS ${TEMP})
    endif()
    add_test(NAME ${unittest} COMMAND ${unittest})
endforeach()
