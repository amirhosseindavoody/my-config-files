#include "nelder_mead.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

#include "gmock/gmock.h"

namespace optimizer {
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
  optimizer->maxfun = 200;

  std::vector<double> final_point =
      optimizer->Minimize(Rosenbrock, initial_point);

  SPDLOG_INFO("------------ final_point: {}", final_point);
  SPDLOG_INFO("------------ status: {}", optimizer->status);

  EXPECT_TRUE(optimizer->success);
}

// Checks if using the optimizer repeatedly on multiple functions returns
// expected convergence results. This test is mainly concerned with leaking
// success criteria from one optimization task to another.
TEST_F(ScipyNelderMeadTest, UseOptimizerRepeatedly) {
  EXPECT_FALSE(optimizer->success);

  // Must be successful.
  {
    std::vector<double> initial_point = {0, 0};
    optimizer->fatol = 1e-12;
    optimizer->xatol = 1e-6;
    optimizer->maxfun = 200;

    std::vector<double> final_point =
        optimizer->Minimize(Rosenbrock, initial_point);

    EXPECT_TRUE(optimizer->success);
  }

  // Must reach fcalls > maxfun and be .
  {
    std::vector<double> initial_point = {0, 0};
    optimizer->fatol = 1e-12;
    optimizer->xatol = 1e-6;
    optimizer->maxfun = 10;

    std::vector<double> final_point =
        optimizer->Minimize(Rosenbrock, initial_point);

    EXPECT_FALSE(optimizer->success);
    EXPECT_GT(optimizer->fcalls, optimizer->maxfun);
  }

  // Must reach fcalls > maxfun and be .
  {
    std::vector<double> initial_point = {0, 0};
    optimizer->fatol = 1e-40;
    optimizer->xatol = 1e-40;
    optimizer->maxfun = 1000;
    optimizer->maxiter = 10;

    std::vector<double> final_point =
        optimizer->Minimize(Rosenbrock, initial_point);

    EXPECT_FALSE(optimizer->success);
    EXPECT_GT(optimizer->iterations, optimizer->maxiter);
  }

  // Must converge by satisfying fatol.
  {
    std::vector<double> initial_point = {0, 0};
    optimizer->fatol = 1e-3;
    optimizer->xatol = 1e-40;
    optimizer->maxfun = 1000;
    optimizer->maxiter = 1000;

    std::vector<double> final_point =
        optimizer->Minimize(Rosenbrock, initial_point);

    EXPECT_TRUE(optimizer->success);
    EXPECT_LE(optimizer->iterations, optimizer->maxiter);
    EXPECT_LE(optimizer->fcalls, optimizer->maxfun);
    EXPECT_THAT(optimizer->status,
                ::testing::HasSubstr("function_meets_fatol: true"));
  }

  // Must converge by satisfying xatol.
  {
    std::vector<double> initial_point = {0, 0};
    optimizer->fatol = 1e-40;
    optimizer->xatol = 1e-3;
    optimizer->maxfun = 1000;
    optimizer->maxiter = 1000;

    std::vector<double> final_point =
        optimizer->Minimize(Rosenbrock, initial_point);

    EXPECT_TRUE(optimizer->success);
    EXPECT_LE(optimizer->iterations, optimizer->maxiter);
    EXPECT_LE(optimizer->fcalls, optimizer->maxfun);
    EXPECT_THAT(optimizer->status,
                ::testing::HasSubstr("simplex_meets_xatol: true"));
  }
}

}  // namespace
}  // namespace optimizer