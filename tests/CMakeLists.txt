# the suite is static_asserts: compiling a test IS running it
add_custom_target(ctc-test ALL)

file(GLOB test-sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# tests with a main() run; the rest are static_assert suites where
# compiling is the test
set(runtime-tests "")

foreach(source ${test-sources})
  get_filename_component(test "${source}" NAME_WE)
  if(test IN_LIST runtime-tests)
    add_executable(ctc-test-${test} "${source}")
  else()
    add_library(ctc-test-${test} OBJECT EXCLUDE_FROM_ALL "${source}")
  endif()
  target_link_libraries(ctc-test-${test} PRIVATE ctc::ctc)
  if(CTC_PCH AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    target_precompile_headers(ctc-test-${test} PRIVATE
      "${PROJECT_SOURCE_DIR}/include/ctc.hpp")
  endif()
  add_dependencies(ctc-test ctc-test-${test})
  if(test IN_LIST runtime-tests)
    add_test(NAME run-${test} COMMAND ctc-test-${test})
  else()
    add_test(NAME compile-${test}
      COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target ctc-test-${test})
  endif()
endforeach()
