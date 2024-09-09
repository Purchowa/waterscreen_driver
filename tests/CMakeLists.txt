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
        ${PROJ_DIR}/source/datetime
        ${PROJ_DIR}/source/picture_managment
)

SET(TEST_UTILS_SRC
        src/utils/common_state_assert.h
        src/utils/common_state_assert.c
        src/utils/mocked_functions.c
        src/utils/mocked_functions.h
)

function(generate_test name embedded_srcs)
    set(absolute_srcs "")
    foreach (src IN LISTS embedded_srcs)
        list(APPEND absolute_srcs "${PROJ_DIR}/source/${src}")
    endforeach ()

    add_executable(${name} ${absolute_srcs} ${TEST_UTILS_SRC} "src/${name}.c")

    target_link_libraries(${name} PRIVATE cmocka-static)
    target_include_directories(${name} PRIVATE ${EMBEDDED_DIRS})
    target_compile_features(${name} PRIVATE c_std_17)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

enable_testing()

generate_test("presentation_state_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("demonstration_state_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("low_water_state_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("idle_state_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("standard_state_test"
        "waterscreen_state/standard_mode_state.c;waterscreen_state/waterscreen_state_context.c;waterscreen_state/waterscreen_states.c")

generate_test("sensors_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("valves_test"
        "waterscreen_state/waterscreen_states.c;waterscreen_state/waterscreen_state_context.c")

generate_test("config_parser_test"
        "external_communication/waterscreen_config_parser.c;external/cjson/cjson.c")

generate_test("datetime_parser_test"
        "datetime/datetime_parser.c")

generate_test("weather_parser_test"
        "external_communication/weather_parser.c;external/cjson/cjson.c")
