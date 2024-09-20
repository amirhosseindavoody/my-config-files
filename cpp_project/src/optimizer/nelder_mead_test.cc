#include "nelder_mead.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace dvd {
namespace {

double Rosenbrock(std::vector<double> x) {
  constexpr double a = 1.0;
  constexpr double b = 100.0;

  return std::pow(a - x[0], 2) + b * std::pow(x[1] - std::pow(x[0], 2), 2);
};

class ScipyNelderMeadTest : public testing::Test {
 protected:
  void SetUp() override { optimizer = std::make_unique<ScipyNelderMead>(); };

  std::unique_ptr<ScipyNelderMead> optimizer;
};

TEST_F(ScipyNelderMeadTest, OptimizeRosenbrock) {
  std::vector<double> initial_point = {0, 0};
  optimizer->fatol = 1e-12;
  optimizer->xatol = 1e-6;

  std::vector<double> final_point =
      optimizer->Minimize(Rosenbrock, initial_point);

  SPDLOG_INFO("------------ final_point: {}", final_point);
  SPDLOG_INFO("------------ status: {}", optimizer->status);

  EXPECT_TRUE(optimizer->success);
}

}  // namespace
}  // namespace dvd