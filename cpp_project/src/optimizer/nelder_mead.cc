#include "nelder_mead.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <spdlog/common.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "../utils/logging.h"
#include "../utils/math.h"

namespace dvd {

// Function to reorder a vector based on the given indices
template <typename T>
std::vector<T> take(const std::vector<T>& vec,
                    const std::vector<size_t>& indices) {
  std::vector<T> result(indices.size());
  for (size_t i = 0; i < indices.size(); ++i) {
    result[i] = vec[indices[i]];
  }
  return result;
}

std::vector<double> ScipyNelderMead::Minimize(
    const CallbackFunction& callback,
    const std::vector<double>& initial_point) {
  SPDLOG_CHECK(!initial_point.empty(), "Initial point vector cannot be empty.");

  SPDLOG_TRACE("------------ initial_point: {}", initial_point);

  // Holds simplex points.
  std::vector<std::vector<double>> simplex = make_simplex(initial_point);

  // Number of the simplex points.
  size_t n_simplex = simplex.size();

  std::vector<double> f_simplex(simplex.size(), 0);
  std::transform(simplex.begin(), simplex.end(), f_simplex.begin(), callback);

  // sort simplex so that the first point has the lowest function value.
  std::vector<size_t> idx = argsort(f_simplex);
  simplex = take(simplex, idx);
  f_simplex = take(f_simplex, idx);

  // Number of function calls.
  size_t fcalls = 0;
  auto wrapped_callback = [&fcalls, &callback](const std::vector<double>& x) {
    fcalls += 1;
    return callback(x);
  };

  while ((fcalls < maxfun) || (iterations < maxiter)) {
    if (SimplexMeetsAbsoluteTolerance(simplex, xatol) ||
        FunctionMeatsAbsoluteTolerance(f_simplex, fatol)) {
      success = true;
      status = fmt::format(
          "Optimizer converged. fcalls: {}, iterations: {}, "
          "simplex_meets_xatol: {}, function_meets_fatol: {}",
          fcalls, iterations, SimplexMeetsAbsoluteTolerance(simplex, xatol),
          FunctionMeatsAbsoluteTolerance(f_simplex, fatol));
      break;
    }

    std::vector<double> xbar = GetXbar(simplex);
    std::vector<double> xr = (1 + rho) * xbar - rho * simplex.back();
    EnforceBounds(lower_bound, upper_bound, xr);
    double f_xr = wrapped_callback(xr);

    bool do_shrink = false;

    if (f_xr < f_simplex[0]) {
      std::vector<double> xe =
          (1 + rho * chi) * xbar - rho * chi * simplex.back();
      EnforceBounds(lower_bound, upper_bound, xe);
      double f_xe = wrapped_callback(xe);
      if (f_xe < f_xr) {
        simplex.back() = xe;
        f_simplex.back() = f_xe;
      } else {
        simplex.back() = xr;
        f_simplex.back() = f_xr;
      }
    } else {
      // f_simplex[0] <= f_xr
      if (f_xr < f_simplex[n_simplex - 2]) {
        simplex.back() = xr;
        f_simplex.back() = f_xr;
      } else {
        // f_xr <= f_simplex[-2]
        // Perform contraction
        if (f_xr < f_simplex.back()) {
          std::vector<double> xc =
              (1 + psi * rho) * xbar - psi * rho * simplex.back();
          EnforceBounds(lower_bound, upper_bound, xc);
          double f_xc = wrapped_callback(xc);
          if (f_xc <= f_xr) {
            simplex.back() = xc;
            f_simplex.back() = f_xc;
          } else {
            do_shrink = true;
          }
        } else {
          // Perform an inside contraction
          std::vector<double> xcc = (1 - psi) * xbar + psi * simplex.back();
          EnforceBounds(lower_bound, upper_bound, xcc);
          double f_xcc = wrapped_callback(xcc);

          if (f_xcc < f_simplex.back()) {
            simplex.back() = xcc;
            f_simplex.back() = f_xcc;
          } else {
            do_shrink = true;
          }
        }

        if (do_shrink) {
          for (size_t j = 1; j < simplex.size(); j++) {
            simplex[j] = simplex[0] + sigma * (simplex[j] - simplex[0]);
            EnforceBounds(lower_bound, upper_bound, simplex[j]);
            f_simplex[j] = wrapped_callback(simplex[j]);
          }
        }
      }
    }

    iterations += 1;
    std::vector<size_t> idx = argsort(f_simplex);
    simplex = take(simplex, idx);
    f_simplex = take(f_simplex, idx);
  }

  std::vector<double> x = simplex[0];

  return x;
};

std::vector<std::vector<double>> ScipyNelderMead::make_simplex(
    const std::vector<double>& initial_point) {
  std::vector<std::vector<double>> simplex(initial_point.size() + 1,
                                           initial_point);

  if (stencile.size() == 1) {
    for (size_t i = 1; i < simplex.size(); i++) {
      simplex[i][i - 1] += stencile[0];
    }
  } else {
    SPDLOG_CHECK(
        stencile.size() == initial_point.size(),
        fmt::format("stencile must have either size of 1 or the same size as "
                    "initial_point: stencil.size()={}, initial_point.size()={}",
                    stencile.size(), initial_point.size()));

    for (size_t i = 1; i < simplex.size(); i++) {
      simplex[i][i - 1] += stencile[i - 1];
    }
  }

  for (size_t i = 0; i < simplex.size(); i++) {
    EnforceBounds(lower_bound, upper_bound, simplex[i]);
  }

  return simplex;
};

std::vector<double> ScipyNelderMead::GetXbar(
    const std::vector<std::vector<double>>& simplex) {
  if (simplex.empty()) {
    return {};
  }

  size_t N = simplex[0].size();
  if (simplex.size() != (N + 1)) {
    throw std::length_error("Incorrect simplex size.");
  }

  auto xbar = std::vector<double>(N, 0);
  // Loop over all simplex points except for the last point.
  for (size_t i = 0; i < (simplex.size() - 1); ++i) {
    for (size_t j = 0; j < N; ++j) {
      xbar[j] += simplex[i][j];
    }
  }

  for (size_t j = 0; j < N; ++j) {
    xbar[j] /= static_cast<double>(N);
  }

  return xbar;
}

void ScipyNelderMead::EnforceBounds(double lower_bound, double upper_bound,
                                    std::vector<double>& point) {
  for (double& p : point) {
    if (p < lower_bound) {
      p = lower_bound;
    }
    if (upper_bound < p) {
      p = upper_bound;
    }
  }
}

bool ScipyNelderMead::SimplexMeetsAbsoluteTolerance(
    const std::vector<std::vector<double>>& simplex, double xatol) {
  for (size_t i = 1; i < simplex.size(); ++i) {
    for (size_t j = 0; j < simplex[0].size(); ++j) {
      if (std::abs(simplex[i][j] - simplex[0][j]) > xatol) {
        return false;
      }
    }
  }
  return true;
};

bool ScipyNelderMead::FunctionMeatsAbsoluteTolerance(
    const std::vector<double>& f_simplex, double fatol) {
  for (size_t i = 1; i < f_simplex.size(); ++i) {
    if (std::abs(f_simplex[i] - f_simplex[0]) > fatol) {
      return false;
    }
  }

  return true;
};

}  // namespace dvd