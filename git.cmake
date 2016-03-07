function(git_submodule DIR)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${DIR}/.git" )
        message(STATUS "--------------------------------")
        message(STATUS "Submodule \"${DIR}\" not present. Try to obtain it using git ...")
        if(NOT DEFINED GIT_FOUND)
            find_package(Git)
        endif()
        if(GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${DIR} RESULT_VARIABLE RETVAL WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
            if(${RETVAL} EQUAL 0)
                message(STATUS "Obtained submodule \"${DIR}\".")
            else()
                message(FATAL_ERROR "Failed to obtain submodule \"${DIR}\"! Run \"git submodule update --init --recursive ${DIR}\" manually.")
            endif()
        else()
            message(FATAL_ERROR "Cannot find git! Run \"git submodule update --init --recursive ${DIR}\" manually.")
        endif()
        message(STATUS "--------------------------------")
    endif()
endfunction()