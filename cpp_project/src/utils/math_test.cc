#include "math.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace utils {
namespace {

const int kSeed = 0;
const double kPi = 3.14159265358979323846;

// Check for near equality of two double integers.
MATCHER_P(Near, tol, "") {
  return std::fabs(std::get<0>(arg) - std::get<1>(arg)) < tol;
}

TEST(MathTest, TestNormFunction) {
  std::vector<double> vec{2, 2};

  EXPECT_NEAR(norm(vec), 2 * std::sqrt(2), 1e-8);
};

TEST(MathTest, SphericalToCartesianNormality) {
  std::default_random_engine rng(kSeed);

  std::uniform_real_distribution<double> distribution(/*__a=*/0,
                                                      /*__b=*/2 * kPi);

  double r = 2;
  std::vector<double> thetas;
  for (size_t i = 0; i <= 10; i++) {
    std::vector<double> cartesian = SphericalToCartesian(r, thetas);
    SPDLOG_INFO("r: {}, thetas: {}, cartesian: {}", r, thetas, cartesian);

    EXPECT_EQ(cartesian.size(), thetas.size() + 1);

    EXPECT_NEAR(norm(cartesian), r, 1e-5);

    thetas.push_back(distribution(rng));
  }
}

TEST(MathTest, CartesianToSphericalDoesNotAcceptEmptyVector) {
  EXPECT_THROW(CartesianToSpherical({}), std::runtime_error);
}

TEST(MathTest, SphericalToCartesianReversability) {
  std::default_random_engine rng(kSeed);

  std::normal_distribution<double> distribution(/*__mean=*/0,
                                                /*__stddev=*/1);

  for (size_t ndim = 2; ndim <= 10; ndim++) {
    SPDLOG_INFO("ndim: {}", ndim);
    std::vector<double> cartesian(ndim);
    for (size_t i = 0; i <= 10; i++) {
      std::generate(cartesian.begin(), cartesian.end(),
                    [&distribution, &rng]() { return distribution(rng); });
      auto [r, thetas] = CartesianToSpherical(cartesian);

      std::vector<double> back_calculated_cartesian =
          SphericalToCartesian(r, thetas);

      EXPECT_THAT(back_calculated_cartesian,
                  testing::Pointwise(Near(1e-5), cartesian));
    }
  }
}

}  // namespace
}  // namespace utils