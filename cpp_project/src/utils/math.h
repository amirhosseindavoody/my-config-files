#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace utils {
// Template function to overload the addition operator for two std::vector<T>
template <typename T>
std::vector<T> operator+(const std::vector<T> &a, const std::vector<T> &b) {
  if (a.size() != b.size()) {
    throw std::invalid_argument("Vectors must be of the same size to add.");
  }

  std::vector<T> result(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    result[i] = a[i] + b[i];
  }

  return result;
}

// Template function to overload the subtraction operator for two std::vector<T>
template <typename T>
std::vector<T> operator-(const std::vector<T> &a, const std::vector<T> &b) {
  if (a.size() != b.size()) {
    throw std::invalid_argument("Vectors must be of the same size to add.");
  }

  std::vector<T> result(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    result[i] = a[i] - b[i];
  }

  return result;
}

// Template function to overload the multiplication operator for std::vector<T>
// and a scalar
template <typename T>
std::vector<T> operator*(T scalar, const std::vector<T> &vec) {
  std::vector<T> result(vec.size());
  for (size_t i = 0; i < vec.size(); ++i) {
    result[i] = scalar * vec[i];
  }
  return result;
}

// Template function to overload the in-place multiplicaton operator for
// std::vector<T> and a scalar
template <typename T>
std::vector<T> &operator*=(std::vector<T> &vec, T scalar) {
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] *= scalar;
  }
  return vec;
}

// Template function to overload the division operator for std::vector<T> and a
// scalar
template <typename T>
std::vector<T> operator/(const std::vector<T> &vec, T scalar) {
  std::vector<T> result(vec.size());
  for (size_t i = 0; i < vec.size(); ++i) {
    result[i] = vec[i] / scalar;
  }
  return result;
}

// Template function to overload the in-place division operator for
// std::vector<T> and a scalar
template <typename T>
std::vector<T> &operator/=(std::vector<T> &vec, T scalar) {
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] /= scalar;
  }
  return vec;
}

// Template function to overload the in-place division operator for
// std::vector<T> and a scalar
template <typename T>
double norm(const std::vector<T> &vec, size_t start = 0) {
  T value = 0;
  for (size_t i = start; i < vec.size(); ++i) {
    value += (vec[i] * vec[i]);
  }
  return std::sqrt(value);
}

// Function to perform argsort on a std::vector<double>
std::vector<size_t> argsort(const std::vector<double> &vec);

// Calculates Percentile point from sigma value `x`.
// This is equivalent to 100 * CDF(x) for normal distribution N(0, 1)
// Output value is in range [0, 100].
double PercentileFromSigma(double x);

// Calculates sigma value corresponding to a particular percentile point.
// Input value must be in range [0, 100].
double SigmaFromPercentile(double percentile);

// calculates the percentile value but modifies the order of the input value.
// percentile must be between 0 and 100.
double CalculatePercentile(std::vector<double> &data, double percentile);

// Converts spherical coordinates to cartesian corrdinates.
std::vector<double> SphericalToCartesian(double r,
                                         const std::vector<double> &thetas);

// Converts cartesian coordinates to spherical corrdinates.
std::pair<double, std::vector<double>> CartesianToSpherical(
    const std::vector<double> &x);

}  // namespace utils

#endif  // MATH_H