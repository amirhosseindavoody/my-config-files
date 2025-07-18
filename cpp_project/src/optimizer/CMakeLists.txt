# CMakeLists.txt for the optimizer module

# Nelder Mead optimizer.
add_library(nelder_mead STATIC nelder_mead.cc)
target_include_directories(nelder_mead PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(nelder_mead PUBLIC ${THIRDPARTY_LIBS} logging math_utils)

# Nelder Mead optimizer test.
add_executable(nelder_mead_test nelder_mead_test.cc)
target_link_libraries(nelder_mead_test ${GTEST} nelder_mead logging)
gtest_discover_tests(
  nelder_mead_test DISCOVERY_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
