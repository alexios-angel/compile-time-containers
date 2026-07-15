file(GLOB example-sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(source ${example-sources})
  get_filename_component(example "${source}" NAME_WE)
  add_executable(ctc-example-${example} "${source}")
  target_link_libraries(ctc-example-${example} PRIVATE ctc::ctc)
  if(CTC_PCH AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    target_precompile_headers(ctc-example-${example} PRIVATE
      "${PROJECT_SOURCE_DIR}/include/ctc.hpp")
  endif()
  add_test(NAME example-${example} COMMAND ctc-example-${example})
endforeach()
