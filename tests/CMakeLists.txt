cmake_minimum_required(VERSION 3.28)

project("waterscreen_tests" VERSION 1.0 LANGUAGES C)

include(cmake/FetchCMocka.cmake)

set(TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
cmake_path(GET TESTS_PATH PARENT_PATH PROJ_DIR)

set(EMBEDDED_DIRS
        ${PROJ_DIR}/source
        ${PROJ_DIR}/source/spi_transfer
        ${PROJ_DIR}/source/gpio
        ${PROJ_DIR}/source/waterscreen_state
        ${PROJ_DIR}/source/external_communication
        ${PROJ_DIR}/source/external
)

set(EMBEDDED_SRC
        ${PROJ_DIR}/source/spi_transfer/spi_transfer.h
        ${PROJ_DIR}/source/gpio/power_control.h
        ${PROJ_DIR}/source/gpio/sensors_control.h
        ${PROJ_DIR}/source/pictures.h
        ${PROJ_DIR}/source/waterscreen_state/validation.h
        ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.h
        ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.c
        ${PROJ_DIR}/source/waterscreen_state/waterscreen_state_context.h
        ${PROJ_DIR}/source/waterscreen_state/waterscreen_state_context.c
        ${PROJ_DIR}/source/external/cjson/cjson.c
        ${PROJ_DIR}/source/external_communication/waterscreen_config_parser.h
        ${PROJ_DIR}/source/external_communication/waterscreen_config_parser.c
)

SET(TEST_UTILS_SRC
        src/utils/common_state_assert.h
        src/utils/common_state_assert.c
        src/utils/mocked_functions.c
        src/utils/mocked_functions.h
)

function(generate_test name src)
    add_executable(${name} ${EMBEDDED_SRC} ${TEST_UTILS_SRC} ${src})

    target_link_libraries(${name} PRIVATE cmocka-static)
    target_include_directories(${name} PRIVATE ${EMBEDDED_DIRS})
    target_compile_features(${name} PRIVATE c_std_17)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

enable_testing()

generate_test("presentationStateTest" "src/presentation_state_test.c")
generate_test("demonstrationStateTest" "src/demonstration_state_test.c")
generate_test("lowWaterStateTest" "src/low_water_state_test.c")
generate_test("idleStateTest" "src/idle_state_test.c")
generate_test("sensorsTest" "src/sensors_test.c")
generate_test("valvesTest" "src/valves_test.c")
generate_test("configParserTest" "src/config_parser_test.c")
