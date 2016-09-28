find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(NOT IWYU_PATH)
  message(FATAL_ERROR "Could not find include-what-you-use")
endif()

function(enable_iwyu TARGET)
    set_property(TARGET ${TARGET} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
endfunction()
