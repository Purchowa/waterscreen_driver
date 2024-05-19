cmake_minimum_required(VERSION 3.29)

project("waterscreen_tests" VERSION 1.0 LANGUAGES C)

include(cmake/FetchCMocka.cmake)

set (TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
cmake_path(GET TESTS_PATH PARENT_PATH PROJ_DIR)

set(SRC
  ${PROJ_DIR}/drivers/fsl_common.h
  ${PROJ_DIR}/device/fsl_device_registers.h
  ${PROJ_DIR}/CMSIS/core_cm33.h
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.h
  ${PROJ_DIR}/source/waterscreen_state/waterscreen_states.c
  ${TESTS_PATH}/main.c
)

set(EXE_NAME tests)

add_executable(${EXE_NAME} ${SRC})

target_link_libraries(${EXE_NAME} PRIVATE cmocka-static)
target_include_directories(${EXE_NAME} PUBLIC ${PROJ_DIR}/source ${PROJ_DIR}/drivers ${PROJ_DIR}/device ${PROJ_DIR}/CMSIS)
target_compile_features(${EXE_NAME} PRIVATE c_std_11)
target_compile_definitions(${EXE_NAME} PUBLIC CPU_LPC55S69JBD100_cm33_core0)

enable_testing()
add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})