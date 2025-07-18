#ifndef NELDER_MEAD_H
#define NELDER_MEAD_H
#include <cstddef>
#include <functional>
#include <limits>
#include <string>
#include <vector>

namespace optimizer {

class ScipyNelderMead {
 public:
  using CallbackFunction = std::function<double(const std::vector<double>&)>;
  ScipyNelderMead(const std::string& name = "Scipy Nelder Mead")
      : name_(name) {};

  std::vector<double> Minimize(const CallbackFunction& callback,
                               const std::vector<double>& initial_point);
  std::string name() { return name_; };

  // tolerance for movement in the input search space for detecting convergence.
  double xatol = 1e-4;

  // tolerance for movement in the output space for detecting convergence.
  double fatol = 1e-4;

  // Stencil value used for making the initial simplex.
  std::vector<double> stencile{0.05};

  // Maximum number of function calls.
  size_t maxfun = 100;

  // Maximum number of iterations
  size_t maxiter = 500;

  // Number of iterations.
  size_t iterations = 0;

  // Number of function calls.
  size_t fcalls;

  // Parameters for updating simplex points.
  double rho = 1;
  double chi = 2;
  double psi = 0.5;
  double sigma = 0.5;

  // lower and upper bound of the search space for all dimensions.
  double lower_bound = -std::numeric_limits<double>::infinity();
  double upper_bound = std::numeric_limits<double>::infinity();

  // Specifies if the optimization has succeeded.
  bool success = false;

  // Get information about the convergence status of the optimizer.
  std::string status;

 private:
  std::string name_;

  std::vector<std::vector<double>> make_simplex(
      const std::vector<double>& initial_point);

  std::vector<std::vector<double>> make_simplex2(
      const CallbackFunction& callback,
      const std::vector<double>& initial_point);

  // Returns true if maximum difference between simplex is smaller than xatol.
  bool SimplexMeetsAbsoluteTolerance(
      const std::vector<std::vector<double>>& simplex, double xatol);

  // Returns true if maximum difference between simplex is smaller than xatol.
  bool FunctionMeetsAbsoluteTolerance(const std::vector<double>& f_simplex,
                                      double fatol);

  // Returns the average point of the simplex point except for the last point of
  // the simplex.
  std::vector<double> GetXbar(const std::vector<std::vector<double>>& simplex);

  // Clips the input point to the bound limits.
  void EnforceBounds(double lower_bound, double upper_bound,
                     std::vector<double>& point);
};

}  // namespace optimizer

#endif  // NELDER_MEAD_H