

#include "math.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <numeric>
#include <vector>

namespace dvd {

std::vector<size_t> argsort(const std::vector<double>& vec) {
  // Create an index vector with the same size as the input vector
  std::vector<size_t> indices(vec.size());
  // Fill it with 0, 1, ..., vec.size() - 1
  std::iota(indices.begin(), indices.end(), 0);

  // Sort the index vector based on the values in vec
  std::sort(indices.begin(), indices.end(),
            [&vec](size_t i1, size_t i2) { return vec[i1] < vec[i2]; });

  return indices;
}

}  // namespace dvd
