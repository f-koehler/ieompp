#.rst:
# FindIWYU
# -------
#
# Finds the include-what-you-use executable
#
# This will define the following variables::
#
#   IWYU_FOUND      - True if the system has the include-what-you-use executable
#   IWYU_VERSION    - The version of the include-what-you-use executable which was found
#   IWYU_EXECUTABLE - Path to the include-what-you-use executable

#=============================================================================
# The MIT License (MIT)
# 
# Copyright (c) 2016 Fabian Köhler
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#=============================================================================

find_program(
    IWYU_EXECUTABLE include-what-you-use
    DOC "include-what-you-use executable"
)
mark_as_advanced(IWYU_EXECUTABLE)

if(IWYU_EXECUTABLE)
    execute_process(
        COMMAND ${IWYU_EXECUTABLE} --version
        OUTPUT_VARIABLE IWYU_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    IWYU
    REQUIRED_VARS IWYU_EXECUTABLE
    VERSION_VAR IWYU_VERSION
)
