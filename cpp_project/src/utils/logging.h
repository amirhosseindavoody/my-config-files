#ifndef LOGGING_H
#define LOGGING_H

#include <spdlog/spdlog.h>

namespace dvd {

#define SPDLOG_CHECK(condition, ...)                                     \
  if (!(condition)) {                                                    \
    SPDLOG_ERROR("Check failed: " #condition ". {}", ##__VA_ARGS__);     \
    throw std::runtime_error(                                            \
        fmt::format("Check failed: " #condition ". {}", ##__VA_ARGS__)); \
  }  // namespace uvm

// Set the default logger to a file_logger.
void InitializeFileLogger(const std::string& log_file);

// Converts a 2D vector of strings into a formatted table string.
std::string TableToString(const std::vector<std::vector<std::string>>& table);

// Converts a 2D vector of strings into a formatted table string.
std::string TableToCsv(const std::vector<std::vector<std::string>>& table);

// Helper class to initialize the logger
class LoggerInitializer {
 public:
  LoggerInitializer() {
    // 2024-08-20 - 12:07:13 - fit_uvm.py:93 - INFO - message
    spdlog::set_pattern("%Y-%m-%d - %H:%M:%S - %s:%# - %l - %v ");
    spdlog::set_level(spdlog::level::trace);

    // periodically flush all *registered* loggers every 3 seconds:
    // warning: only use if all your loggers are thread-safe ("_mt" loggers)
    spdlog::flush_every(std::chrono::seconds(1));
  }
};

// Global instance of the helper class
static LoggerInitializer logger_initializer;

}  // namespace dvd

#endif  // LOGGING_H