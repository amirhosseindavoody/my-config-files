add_library(testing_utils STATIC utils.cc)
target_include_directories(testing_utils PUBLIC ${PROJECT_SOURCE_DIR}/src gtest)
target_link_libraries(testing_utils PUBLIC ${THIRDPARTY_LIBS} gtest)
