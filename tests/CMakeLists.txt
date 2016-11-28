include_directories(${CMAKE_SOURCE_DIR}/include)

function(add_ieompp_test test_name)
    target_link_libraries(${test_name} ${Boost_LIBRARIES})
    add_test(${test_name} ${test_name})
    install(TARGETS ${test_name} RUNTIME DESTINATION bin/tests)
endfunction()

add_subdirectory(algebra)
add_subdirectory(lattices)
add_subdirectory(models)
