#include <spdlog/spdlog.h>

#include <cpptrace/from_current.hpp>

#include "main_lib.h"

int main(int argc, char** argv) {
  CPPTRACE_TRY { return dvd::SayHello(argc, argv); }
  CPPTRACE_CATCH(const std::exception& e) {
    fmt::println(std::string(100, '='));
    fmt::println("Exception caught: {}\n", e.what());
    fmt::println(std::string(100, '='));
    fmt::println("Exception stacktrace:\n{}",
                 cpptrace::from_current_exception().to_string());

    spdlog::error(std::string(100, '='));
    spdlog::error("Exception caught: {}", e.what());
    spdlog::error(std::string(100, '='));
    spdlog::critical("Exception stacktrace:\n{}",
                     cpptrace::from_current_exception().to_string());

    return 1;
  }

  return 0;
}