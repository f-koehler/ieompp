function(run_version_script SCRIPT CONFIG_FILE)
    add_custom_target(
        configure
        ALL
        COMMAND ${SCRIPT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E echo
        OUTPUT ${CONFIG_FILE}
    )
endfunction()
