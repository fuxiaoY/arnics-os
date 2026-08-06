cmake_minimum_required(VERSION 3.20)

project(arnics-os-tests LANGUAGES C)

option(ARNICS_ENABLE_COVERAGE "Enable GCC/Clang coverage instrumentation" OFF)
option(ARNICS_BUILD_EVENT_SYSTEM_TESTS "Build event-center application system tests" OFF)

enable_testing()

set(ARNICS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../arnics-os")
set(UNITY_ROOT "${ARNICS_ROOT}/thirdParty/unity")

function(arnics_configure_test target)
  target_include_directories(${target} PRIVATE
    "${ARNICS_ROOT}"
    "${ARNICS_ROOT}/Inc"
    "${UNITY_ROOT}"
    "${ARNICS_ROOT}/thirdParty/AlgorithmLib"
    "${ARNICS_ROOT}/thirdParty/ring"
    "${ARNICS_ROOT}/rtosInterface/queue"
  )
  target_compile_definitions(${target} PRIVATE UNITY_INCLUDE_DOUBLE)
  set_property(TARGET ${target} PROPERTY C_STANDARD 11)
  set_property(TARGET ${target} PROPERTY C_STANDARD_REQUIRED ON)
  set_property(TARGET ${target} PROPERTY C_EXTENSIONS OFF)
  if (MSVC)
    target_compile_options(${target} PRIVATE /utf-8)
  endif()
  if (UNIX)
    target_link_libraries(${target} PRIVATE pthread)
  endif()
endfunction()

add_executable(arnics-unit-tests
  unit/test_unit_main.c
  unit/core/test_crc16.c
  unit/core/test_ringbuffer.c
  unit/core/test_data_platform.c
  unit/platform/test_queue_win.c
  support/test_platform_stubs.c
  "${UNITY_ROOT}/unity.c"
  "${ARNICS_ROOT}/thirdParty/AlgorithmLib/crc16.c"
  "${ARNICS_ROOT}/thirdParty/ring/ringbuffer.c"
  "${ARNICS_ROOT}/rtosInterface/win/queue_win.c"
  "${ARNICS_ROOT}/dataPlat/dataCore/dataCore.c"
  "${ARNICS_ROOT}/dataPlat/dataManager/dataEvent.c"
)
arnics_configure_test(arnics-unit-tests)

if (ARNICS_BUILD_EVENT_SYSTEM_TESTS)
  add_executable(arnics-event-system-tests
    system/event_center/test_event_center_main.c
    system/event_center/test_event_center_flow.c
    system/event_center/test_event_center_stress.c
    system/event_center/event_center_test_employees.c
    system/event_center/event_center_test_support.c
    support/event_center_rtos_adapter.c
    support/test_platform_stubs.c
    "${UNITY_ROOT}/unity.c"
    "${ARNICS_ROOT}/dePartment/centerEvent/eventProcess/eventCore.c"
    "${ARNICS_ROOT}/core/separateSM/separate_core.c"
    "${ARNICS_ROOT}/rtosInterface/queue/queue_port.c"
    "${ARNICS_ROOT}/rtosInterface/queue/compat_queue.c"
    "${ARNICS_ROOT}/rtosInterface/compat_mutex.c"
    "${ARNICS_ROOT}/rtosInterface/win/queue_win.c"
  )
  arnics_configure_test(arnics-event-system-tests)
  target_include_directories(arnics-event-system-tests PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/system/event_center"
  )
  add_test(NAME arnics-event-system-tests COMMAND arnics-event-system-tests)
  set_tests_properties(arnics-event-system-tests PROPERTIES LABELS "system;event-center;stress" TIMEOUT 90)
endif()

if (ARNICS_ENABLE_COVERAGE)
  set(coverage_targets arnics-unit-tests)
  if (ARNICS_BUILD_EVENT_SYSTEM_TESTS)
    list(APPEND coverage_targets arnics-event-system-tests)
  endif()
  foreach(target IN LISTS coverage_targets)
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      target_compile_options(${target} PRIVATE --coverage -O0 -g)
      target_link_options(${target} PRIVATE --coverage)
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
      target_compile_options(${target} PRIVATE -fprofile-instr-generate -fcoverage-mapping -O0 -g)
      target_link_options(${target} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    else()
      message(FATAL_ERROR "ARNICS_ENABLE_COVERAGE requires GCC or Clang")
    endif()
  endforeach()
endif()

add_test(NAME arnics-unit-tests COMMAND arnics-unit-tests)
set_tests_properties(arnics-unit-tests PROPERTIES LABELS "unit;core")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
  set(report_dependencies arnics-unit-tests)
  if (ARNICS_BUILD_EVENT_SYSTEM_TESTS)
    list(APPEND report_dependencies arnics-event-system-tests)
  endif()
  add_custom_target(arnics-unit-test-report
    COMMAND "${CMAKE_CTEST_COMMAND}" --test-dir "${CMAKE_BINARY_DIR}" -C "$<CONFIG>" --output-on-failure --output-junit "${CMAKE_BINARY_DIR}/test-results-$<CONFIG>.xml"
    DEPENDS ${report_dependencies}
    USES_TERMINAL
  )
endif()
