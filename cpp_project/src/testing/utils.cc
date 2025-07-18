#include "utils.h"

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace testing {

namespace fs = std::filesystem;

std::string Dedent(const std::string &input) {
  std::vector<std::string> lines;
  std::istringstream iss(input);
  std::string line;
  size_t min_indent = std::numeric_limits<size_t>::max();

  // Split into lines and find minimum indentation (ignoring empty lines)
  while (std::getline(iss, line)) {
    lines.push_back(line);
    auto first_non_space = line.find_first_not_of(" \t");
    if (first_non_space != std::string::npos) {
      min_indent = std::min(min_indent, first_non_space);
    }
  }

  // Remove min_indent spaces/tabs from each line
  std::ostringstream oss;
  bool first_non_empty_line_found = false;
  for (const auto &l : lines) {
    // Skip leading empty lines
    if (!first_non_empty_line_found) {
      if (l.find_first_not_of(" \t\r\n") == std::string::npos) {
        continue;
      }
      first_non_empty_line_found = true;
    }
    if (min_indent != std::numeric_limits<size_t>::max() &&
        l.size() >= min_indent) {
      oss << l.substr(min_indent);
    } else {
      oss << l;
    }
    oss << '\n';
  }
  return oss.str();
}

std::string WriteTestFile(const std::string &workarea,
                          const std::string &content,
                          const std::string &suffix) {
  std::string filename = fmt::format(
      "{}/{}.{}.{}", workarea,
      UnitTest::GetInstance()->current_test_info()->test_suite_name(),
      UnitTest::GetInstance()->current_test_info()->name(), suffix);

  filename = fs::weakly_canonical(fs::absolute(filename)).string();

  if (!fs::create_directories(fs::path(filename).parent_path()) &&
      !fs::exists(fs::path(filename).parent_path())) {
    throw std::runtime_error("Failed to create directories for test file.");
  }
  std::ofstream ofs(filename);
  if (ofs.is_open()) {
    ofs << content;
    ofs.close();
  } else {
    throw std::runtime_error("Failed to open test file for writing.");
  }

  SPDLOG_INFO("Test file created: {}", filename);

  return filename;
}

}  // namespace testing