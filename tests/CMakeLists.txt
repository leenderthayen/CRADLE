project(CRADLE-utests CXX)

set(CRADLE_MAIN main.cc)

set(CRADLE_UTESTS_SOURCES
  test_external_bsg.cc
  test_config_parser.cc
  test_reaction_engine.cc
  test_spectrum_generator.cc
  test_reaction_mode.cc
  test_reaction_mode_factory.cc
  test_event.cc
  test_writer.cc
  test_cradle_decay.cc)

enable_testing()

foreach(TEST_NAME IN ITEMS ${CRADLE_UTESTS_SOURCES})
  get_filename_component(TEST_NAME_NOEXT ${TEST_NAME} NAME_WE)
  add_executable(${TEST_NAME_NOEXT} ${CRADLE_MAIN} ${TEST_NAME})
  target_include_directories(${TEST_NAME_NOEXT} PUBLIC ${GSL_INCLUDE_DIRS})
  target_link_libraries(${TEST_NAME_NOEXT} PRIVATE Cradle)
  target_link_libraries(${TEST_NAME_NOEXT} PUBLIC ${GSL_LIBRARIES})
  add_test(NAME ${TEST_NAME_NOEXT} COMMAND ${TEST_NAME_NOEXT} -r console)
endforeach()
