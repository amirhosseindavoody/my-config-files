cmake_minimum_required(VERSION 3.30)

project(MCMC LANGUAGES CXX)

include(CTest)
include(GoogleTest)
include(FetchContent)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example when
# compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(
  zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG v1.3.1) # Replace with the desired version tag
FetchContent_MakeAvailable(zlib)
# Ensure zlib is available before other dependencies that may need it
include_directories(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

FetchContent_Declare(
  cpptrace
  GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
  GIT_TAG v0.7.2)
FetchContent_MakeAvailable(cpptrace)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0) # Replace with the desired version tag
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.1.4) # Replace with the desired version tag
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0) # Replace with the desired version tag
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0) # Replace with the desired version tag
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.2) # Replace with the desired version tag
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG v3.2) # Replace with the desired version tag
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(
  abseil
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20250127.1) # Replace with the desired version tag
FetchContent_MakeAvailable(abseil)

# Define a variable that holds all third-party dependencies
set(THIRDPARTY_LIBS
  fmt::fmt
  yaml-cpp
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  argparse::argparse
  cpptrace::cpptrace
  absl::flags
  absl::flags_parse
  absl::flags_usage
  absl::flags_commandlineflag)

set(GTEST GTest::gtest_main GTest::gmock_main)

# # Enable coverage reporting if requested
# # This can be toggled with the CMake option -DCOVERAGE=ON
# option(COVERAGE "Enable coverage reporting" ON)

# if(COVERAGE)
#   message(STATUS "Coverage reporting is enabled")
#   if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
#     add_link_options(-fprofile-instr-generate -fcoverage-mapping)
#   endif()
# endif()

add_subdirectory(src)
