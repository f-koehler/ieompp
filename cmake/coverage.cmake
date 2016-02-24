add_custom_target(coverage)

add_custom_command(
    TARGET coverage
    COMMAND mkdir -p coverage
    COMMAND lcov -c -i -b ${CMAKE_SOURCE_DIR} --no-external -d . -o coverage/baseline.info
    COMMAND ctest
    COMMAND lcov -c -b ${CMAKE_SOURCE_DIR} --no-external -d . -o coverage/unittests.info
    COMMAND lcov -a coverage/baseline.info -a coverage/unittests.info -o coverage/coverage.info
    COMMAND genhtml coverage/coverage.info -o coverage/html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
