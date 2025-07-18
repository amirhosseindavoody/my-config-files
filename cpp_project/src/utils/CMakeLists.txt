# Math utils library
add_library(math_utils STATIC math.cc)
target_link_libraries(math_utils PUBLIC ${THIRDPARTY_LIBS})

# Math test
add_executable(math_utils_test math_test.cc)
target_link_libraries(math_utils_test ${GTEST} math_utils logging)
gtest_discover_tests(math_utils_test)

# Logging library
add_library(logging STATIC logging.cpp)
target_link_libraries(logging PUBLIC ${THIRDPARTY_LIBS})

# Create an interface library for csv.hpp
add_library(csv_parser INTERFACE)
target_compile_options(csv_parser INTERFACE -Wno-deprecated-literal-operator) # Suppress some warnings.
