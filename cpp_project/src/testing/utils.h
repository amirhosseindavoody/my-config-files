#ifndef UTILS_H
#define UTILS_H

#include <limits.h>
#include <unistd.h>

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace testing {

inline std::string CurrentDirectory() {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    return std::string(cwd);
  }

  throw std::runtime_error("getcwd() error");
};

// Removes leading indentation from each line of the input string.
// This is useful for multi-line strings in tests to avoid indentation issues.
std::string Dedent(const std::string &input);

// Writes a test file with the given content to the specified workarea.
// The file is named in the following location:
// workarea/<test_suite_name>.<test_name>.<suffix>
std::string WriteTestFile(const std::string &workarea,
                          const std::string &content,
                          const std::string &suffix);
}  // namespace testing

#endif