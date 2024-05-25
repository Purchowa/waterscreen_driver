cmake_minimum_required(VERSION 3.29)

project("waterscreen_tests" VERSION 1.0 LANGUAGES C)

include(cmake/FetchCMocka.cmake)

set (TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
cmake_path(GET TESTS_PATH PARENT_PATH PROJ_DIR)

set (DIRS
  ${PROJ_DIR}/source
  ${PROJ_DIR}/source/spi_transfer
  ${PROJ_DIR}/source/gpio
  ${PROJ_DIR}/source/waterscreen_state
  ${TESTS_PATH}/src
)

set(SRC
  ${PROJ_DIR}/source/spi_transfer/spi_transfer.h
  ${PROJ_DIR}/source/gpio/power_control.h
  ${PROJ_DIR}/source/gpio/sensors_control.h
  ${PROJ_DIR}/source/pictures.h
  ${PROJ_DIR}/source/waterscreen_state/validation.h
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.h
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.c
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_state_context.h
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_state_context.c
  ${TESTS_PATH}/src/common_state_assert.h
  ${TESTS_PATH}/src/common_state_assert.c
  ${TESTS_PATH}/src/mocked_functions.c
  ${TESTS_PATH}/src/mocked_functions.h
  ${TESTS_PATH}/src/test_main.c
)

set(EXE_NAME tests)

add_executable(${EXE_NAME} ${SRC})

target_link_libraries(${EXE_NAME} PRIVATE cmocka-static)
target_include_directories(${EXE_NAME} PRIVATE ${DIRS})
target_compile_features(${EXE_NAME} PRIVATE c_std_11)

enable_testing()
add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})