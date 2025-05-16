// Paul Erdos's prime problem - erdos-primes.cpp

// C++ Standard Library headers
#include <cstdint>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

// Third-party headers
#include <primesieve.hpp>

/** * Specifies a grid as used in this program. */
struct GridSpec {
  /** * Minimum and maximum coordinates in the grid */
  uint64_t min_x{};
  uint64_t min_y{};
  uint64_t max_x{};
  uint64_t max_y{};

  /** * Dimensions of the grid */
  size_t dim_x{};
  size_t dim_y{};

  /**
   * Values for accessible and inaccessible points.
   * Used to both store the data values and as printable characters.
   */
  char accessible = '#';
  char inaccessible = '.';
};

namespace {
/**
 * The array to hold precomputed prime numbers prime numbers.
 */
std::vector<uint64_t> primes_vec{};
std::set<uint64_t> primes_set{};

} // namespace

/**
 * Precomputes prime numbers up to a given number.
 * Note: This function must be called in `main()` to
 * get the performance advantages of looking up primes rather than recomputing
 * them dynamically.
 */
void precomputePrimes(uint64_t up_to_num) {
  primesieve::generate_primes(up_to_num, &primes_vec);
  for (auto i : primes_vec) {
    primes_set.insert(i);
  }
}

/**
 * Determines whether a number is prime by searching the precomputed array of
 * primes. This approach avoids recomputing primes dynamically.
 *
 * Invariant: `num` must be no greater than the value of `up_to_num` used to
 * precompute the primes.
 */
bool isPrime(uint64_t num) { return primes_set.find(num) != primes_set.end(); }

/**
 * Determines if a point (x, y) is "visible" from the origin.
 */
bool isVisible(uint64_t coord_x, uint64_t coord_y) {
  return std::gcd(coord_x, coord_y) == 1;
}

/**
 * Generates an array representing a grid of points from (grid.min_x,
 * grid.min_y) to (grid.max_x, grid.max_y), inclusive. Each point has the value
 * `grid.accessible` if it is "visible" from the origin and `grid.inaccessible`
 * if it is not.
 */
auto generateVisGrid(GridSpec grid) {
  std::vector<char> result{};
  result.reserve((grid.max_x - grid.min_x + 1) * (grid.max_y - grid.min_y + 1));

  for (size_t coord_y = grid.min_y; coord_y < grid.max_y + 1; ++coord_y) {
    for (size_t coord_x = grid.min_x; coord_x < grid.max_x + 1; ++coord_x) {
      char vis =
          isVisible(coord_x, coord_y) ? grid.accessible : grid.inaccessible;
      result.emplace_back(vis);
    }
  }

  return result;
}

/**
 * Mark all points with coprime coordinates.
 */
void markCoprimes(GridSpec grid, std::vector<char> &data) {
  size_t coord_x{};
  size_t coord_y{};
  size_t data_index{};

  /** * NOTE: `index_x` and `index_y` refer to the indices of the view of the
   * grid defined by `grid.dim_x` and `grid.dim_y`, while `coord_x` and coord_y`
   * refer to the actual x and y coordinates that are represented. */
  for (size_t index_y = 0; index_y < grid.dim_y; ++index_y) {
    coord_y = grid.min_y + index_y;
    if (isPrime(coord_y)) {
      for (size_t index_x = 0; index_x < grid.dim_x; ++index_x) {
        coord_x = grid.min_x + index_x;
        if (isPrime(coord_x)) {
          data_index = index_x + (index_y * grid.dim_x);
          data.at(data_index) = grid.inaccessible;
        }
      }
    }
  }
}

/**
 * Mark all dead-end points: points that do not have any paths out of the grid
 * Works backward from upper-left corner in a column-major order.
 */
void markDeadends(GridSpec grid, std::vector<char> &data) {
  size_t index{};
  size_t index_one_right{};
  size_t index_one_above{};

  for (size_t grid_y = grid.dim_y - 2; grid_y > 0; --grid_y) {
    for (size_t grid_x = grid.dim_x - 2; grid_x > 0; --grid_x) {
      index = grid_x + (grid_y * grid.dim_y);
      index_one_right = index + 1;
      index_one_above = index + grid.dim_x;

      /** * Criteria for a dead-end point:
       * the point to the right is inaccessible
       * and
       * the point above is inaccessible
       */
      if (data.at(index_one_right) == grid.inaccessible &&
          data.at(index_one_above) == grid.inaccessible) {
        data.at(index) = grid.inaccessible;
      }
    }
  }
}

/** * Prints a point grid. */
void printGrid(GridSpec grid, std::vector<char> const &data, bool half_grid) {
  /** * Header with grid metadata */
  std::cout << "Note:\nIndex (1, 1) refers to (min_x, min_y): (" << grid.min_x
            << ", " << grid.min_y << ")\n";
  std::cout << "Index (" << grid.dim_x << ", " << grid.dim_y
            << ") refers to (max_x, max_y): (" << grid.max_x << ", "
            << grid.max_y << ")\n";

  for (size_t coord_y = grid.dim_y; coord_y > 0; --coord_y) {
    if (coord_y < 10) {
      std::cout << " ";
    }
    std::cout << " " << coord_y;
    for (size_t coord_x = 1; coord_x <= grid.dim_x; ++coord_x) {
      size_t index = coord_x + ((coord_y - 1) * grid.dim_x) - 1;
      char result = data.at(index);
      std::cout << "  " << result;

      // Do not print the lower half of the grid
      if (half_grid && coord_x == coord_y) {
        break;
      }
    }
    std::cout << "\n";
  }

  // Print bottom axis
  std::cout << "   ";
  for (size_t label = 1; label <= grid.dim_x; ++label) {
    if (label < 10) {
      std::cout << " ";
    }
    std::cout << " " << label;
  }
  std::cout << "\n";
}

/**
 * \todo(matt):
 * Backtracking algorithm to generate all paths out of grid
 */

/**
 * Brute force backtracking
 *
 *  Begin new path (u, v)
 *      Exhaust u + 1:
 *          Check u + 1 and move there if legal.
 *          Repeat until u + 1 is not accessible.
 *          Mark ending point as not accessible via u + 1.
 *      Exhaust v + 1:
 *          Check v + 1 and move there if legal.
 *          Repeat until v + 1 is not accessible.
 *          Mark ending point as not accessible via v + 1.
 *      If path reaches (dimension, v) or (u, dimension), add to result
 * VecVecTuple. Begin new path starting with next (u, v) according to some
 * progression.
 *  ...
 *  Repeat until all paths are found and marked.
 */

/**
 * Recursive backtracking: working backward from upper-right corner
 *  Types of points: From this point, we can move forward monotonically (FM) to
 *      * both (u+1, v) and (u, v+1)
 *      * only (u+1, v)
 *      * only (u, v+1)
 *      * neither (u+1, v) nor (u, v+1)
 *  Additionally, since we are working backwards, we must check for
 * accessibility from FM movement.
 *      * If the point does not have an accessible point at either (u-1, v) or
 * (u, v-1), mark inaccessible. We start all points as either accessible or
 * inaccessible based on the visibility criterion. For each possible path, we
 * check the points along the path based on the type above.
 *      * If a point is the last type (neither possible moves are legal), we
 * mark it inaccessible. As the algorithm runs, the inaccessibility will
 * propagate until only points along paths are accessible.
 *  ...
 * Ending: ...
 *
 * Idea: each point stores a VecVecTuple of paths that we can append to current
 * result at some point.
 *
 * Note: Finding all paths and adding them to a vector becomes very expensive,
 * very quickly, even when storing all accessible paths at each point. Dubious
 * value beyond small grids.
 * Note: Finding smallest path is not useful, since shortest path is known: {
 * (u, v+1), (u, v+2), ..., (u, v+(n-1)) }, or straight up the y-axis
 */

/**
 * Alternative approach: Tree-based approach where nodes are generated at each
 * intersection.
 *  * Building list of paths simply requires traversing the tree
 */
using VecVecTuple = std::vector<std::vector<std::tuple<size_t, size_t>>>;
auto backtrackPaths(GridSpec grid, std::vector<char> &data) {
  VecVecTuple result{};
  /** \todo (matt): write the backtracking algorithm */
  return result;
}

int main() {
  /**
   * EDIT:
   * The range of numbers to plot, inclusive.
   */
  uint64_t min_x = 10'037;
  uint64_t min_y = 10'037;
  uint64_t max_x = 10'087;
  uint64_t max_y = 10'087;

  /**
   * EDIT:
   * The characters used to represent the points when printing to the console
   */
  char accessible = '#';
  char inaccessible = '.';

  /**
   * EDIT:
   * The number of prime numbers to precompute at runtime.
   */
  uint64_t primes_up_to = 20'000;

  //

  /** * Precompute the primes right away so that they can be used */
  precomputePrimes(primes_up_to);

  /** * Print the last prime that was precomputed. */
  std::cout << primes_vec.back() << "\n";

  /** * Create GridSpec object */
  size_t dim_x = max_x - min_x + 1;
  size_t dim_y = max_y - min_y + 1;
  GridSpec grid_spec = {min_x, min_y, max_x,      max_y,
                        dim_x, dim_y, accessible, inaccessible};

  /** * Step 1: Generate a grid of visible points based on the gcd criterion */
  auto data = generateVisGrid(grid_spec);
  std::cout << "\nVisible points:\n";
  printGrid(grid_spec, data, false);

  /** * Step 2: Remove points with coprime coordinates */
  markCoprimes(grid_spec, data);
  std::cout << "\nVisible points without coprimes:\n";
  printGrid(grid_spec, data, false);

  /** * Step 3: Remove dead-end points (points that lie on paths that do no exit
   * the grid) */
  markDeadends(grid_spec, data);
  std::cout << "\nVisible points without coprimes and dead-end points:\n";
  printGrid(grid_spec, data, false);

  return 0;
}