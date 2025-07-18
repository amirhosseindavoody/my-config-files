

#include "math.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <vector>

#include "logging.h"

namespace utils {
namespace {

const double kEps = 1e-36;

// Approximation of the inverse error function
double erfinv(double x) {
  double a = 0.147;  // Approximation constant
  double ln1mx2 = std::log(1 - x * x);
  double part1 = 2 / (M_PI * a) + ln1mx2 / 2;
  double part2 = ln1mx2 / a;
  return std::copysign(std::sqrt(-part1 + std::sqrt(part1 * part1 - part2)), x);
}

}  // namespace

std::vector<size_t> argsort(const std::vector<double> &vec) {
  // Create an index vector with the same size as the input vector
  std::vector<size_t> indices(vec.size());
  // Fill it with 0, 1, ..., vec.size() - 1
  std::iota(indices.begin(), indices.end(), 0);

  // Sort the index vector based on the values in vec
  std::sort(indices.begin(), indices.end(),
            [&vec](size_t i1, size_t i2) { return vec[i1] < vec[i2]; });

  return indices;
}

double PercentileFromSigma(double x) {
  const double mu = 0.0;
  const double sigma = 1.0;
  return 100.0 * 0.5 * (1 + std::erf((x - mu) / (sigma * std::sqrt(2))));
};

double SigmaFromPercentile(double percentile) {
  return std::sqrt(2) * erfinv(2 * (percentile / 100.0) - 1);
}

double CalculatePercentile(std::vector<double> &data, double percentile) {
  if (data.empty()) {
    throw std::invalid_argument("The data vector is empty.");
  }
  SPDLOG_CHECK(!data.empty(), "Data vector must have at least one member.");

  SPDLOG_CHECK(0.0 <= percentile && percentile <= 100.0,
               fmt::format("Provide percentile: {}", percentile));

  std::sort(data.begin(), data.end());

  double index = (percentile / 100.0) * (data.size() - 1);
  size_t lowerIndex = static_cast<size_t>(index);
  size_t upperIndex = lowerIndex + 1;

  if (upperIndex >= data.size()) {
    return data[lowerIndex];
  }

  double fraction = index - lowerIndex;
  return data[lowerIndex] * (1.0 - fraction) + data[upperIndex] * fraction;
}

std::vector<double> SphericalToCartesian(double r,
                                         const std::vector<double> &thetas) {
  std::vector<double> x(thetas.size() + 1, std::abs(r));

  for (size_t i = 0; i < thetas.size(); i++) {
    x[i] *= std::cos(thetas[i]);
    for (size_t j = i + 1; j < x.size(); j++) {
      x[j] *= std::sin(thetas[i]);
    }
  }

  return x;
};

std::pair<double, std::vector<double>> CartesianToSpherical(
    const std::vector<double> &x) {
  SPDLOG_CHECK(
      x.size() > 1,
      fmt::format("Input vector x must be at least 2 dimensional. x.size(): {}",
                  x.size()));
  double r = 0;
  for (size_t i = 0; i < x.size(); ++i) {
    r += (x[i] * x[i]);
  }
  r = std::sqrt(r) + kEps;

  std::vector<double> theta(x.size() - 1, 0);
  std::vector<double> normed_x = x / r;

  if (!theta.empty()) {
    for (size_t i = 0; i < theta.size(); ++i) {
      double rr = 0;
      for (size_t j = i; j < x.size(); j++) {
        rr += std::pow(normed_x[j], 2);
      }
      rr = std::sqrt(rr) + kEps;
      theta[i] = std::acos(normed_x[i] / rr);
    }
    if (normed_x.back() < 0) {
      theta.back() = 2 * M_PI - theta.back();
    }
  }

  return std::make_pair(r, std::move(theta));
};

}  // namespace utils
