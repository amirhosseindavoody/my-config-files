
#include <filesystem>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "utils/logging.h"

ABSL_FLAG(std::string, log_file, "./example.log", "Log file path");
ABSL_FLAG(std::string, first_name, "", "First name");
ABSL_FLAG(std::string, last_name, "", "Last name");
ABSL_FLAG(size_t, number, 1, "Number of times to say hello");

namespace dvd {

namespace fs = std::filesystem;

// Main function to run the optimization.
int SayHello(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  absl::SetProgramUsageMessage("Program to say hello.");

  std::string log_file = fs::weakly_canonical(absl::GetFlag(FLAGS_log_file));

  if (!fs::exists(log_file)) {
    SPDLOG_THROW2("Log file does not exist: {}", log_file);
  }

  utils::InitializeFileLogger(log_file);

  std::string first_name = absl::GetFlag(FLAGS_first_name);
  std::string last_name = absl::GetFlag(FLAGS_last_name);
  size_t number = absl::GetFlag(FLAGS_number);

  for (size_t i = 0; i < number; ++i) {
    SPDLOG_INFO("Hello, {} {}!", first_name, last_name);
  }

  return 0;
}

}  // namespace dvd
