find_program(CCACHE_PATH ccache)
if(CCACHE_PATH)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
    message(FATAL_ERROR "Unable to find ccache executable")
endif()