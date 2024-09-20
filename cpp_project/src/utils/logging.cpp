#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <sstream>
#include <vector>

namespace dvd {

void InitializeFileLogger(const std::string& log_file) {
  SPDLOG_INFO("Log file: {}", log_file);

  bool overwrite_existing_file = true;
  auto my_logger =
      spdlog::basic_logger_mt("uvm_logger", log_file, overwrite_existing_file);

  spdlog::set_default_logger(my_logger);
};

std::string TableToString(const std::vector<std::vector<std::string>>& table) {
  if (table.empty()) return "";

  std::vector<size_t> columnWidths(table[0].size(), 0);

  for (const auto& row : table) {
    for (size_t i = 0; i < row.size(); ++i) {
      columnWidths[i] = std::max(columnWidths[i], row[i].size());
    }
  }

  std::ostringstream oss;
  for (const auto& row : table) {
    for (size_t i = 0; i < row.size(); ++i) {
      oss << fmt::format("{:<{}}", row[i], columnWidths[i] + 2);
    }
    oss << '\n';
  }

  return oss.str();
}

std::string TableToCsv(const std::vector<std::vector<std::string>>& table) {
  std::ostringstream oss;

  for (const auto& row : table) {
    for (size_t i = 0; i < row.size(); ++i) {
      oss << "\"" << row[i] << "\"";
      if (i < row.size() - 1) {
        oss << ",";
      }
    }
    oss << "\n";
  }

  return oss.str();
}

}  // namespace dvd
