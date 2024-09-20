cmake_minimum_required(VERSION 3.30)
project(TestProject LANGUAGES CXX)

include(CTest)
include(GoogleTest)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example when
# compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# # Set custom compiler paths(adjust as needed)
set(CMAKE_C_COMPILER ../.pixi/envs/default/bin/gcc)
set(CMAKE_CXX_COMPILER ../.pixi/envs/default/bin/g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the Release build flags to include the highest optimization level
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Export the compile commands to enable clangd language server.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)

set(GTEST GTest::gtest_main GTest::gmock_main)

# Add spdlog
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

# Define a variable that holds all third-party dependencies
set(THIRDPARTY_LIBS fmt::fmt spdlog::spdlog)

# Math utils library
add_library(math_utils STATIC src/utils/math.cc)
target_link_libraries(math_utils PUBLIC ${THIRDPARTY_LIBS})

# # Math utils test add_executable(math_utils_test src/utils/math_test.cc)
# target_link_libraries(math_utils_test ${GTEST} math_utils)
# gtest_discover_tests(math_utils_test)

# Logging2 library
add_library(logging2 STATIC src/utils/logging.cpp)
target_link_libraries(logging2 PUBLIC ${THIRDPARTY_LIBS})

# Nelder Mead optimizer.
add_library(nelder_mead STATIC src/optimizer/nelder_mead.cc)
target_link_libraries(nelder_mead PUBLIC ${THIRDPARTY_LIBS} logging2 math_utils)

# Nelder Mead optimizer test.
add_executable(nelder_mead_test src/optimizer/nelder_mead_test.cc)
target_link_libraries(nelder_mead_test ${GTEST} nelder_mead logging2)
gtest_discover_tests(nelder_mead_test)
