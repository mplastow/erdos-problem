# erdos-problem

The programs in this repo do one thing:
* Generate a grid of points that are visible from the origin
* Remove points from the grid where both coordinates are prime
* Remove dead-end points that are not on paths that lead out of the grid in an up-and-to-the-right direction

Note that there may be bugs since I've developed the algorithms for doing this task by feel, and haven't yet written a rigorous test suite.

## C++
`erdos` is a C++ program that generates and caches prime numbers at startup to enable fast prime checking while running. Thanks to the `primesieve` library, prime generation and checking coordinates for coprimes are both very fast, even for large (1000 x 1000) grids of large numbers (~100,000,000).

On the other hand, `erdos` doesn't do any visualization right now, aside from printing to the console. 

The easiest way to interact with the program is by treating it like a script: edit the variables indicated by the `EDIT HERE` comments in `main()`, then build and run.

#### Update: Shifting focus to Python
Since generating primes in Python is so fast now, I will probably not touch the C++ code in this repo unless we need to add features or algorithms that are unavoidably slow in Python.

### Dependencies
C++17 or newer (mainly for `std::gcd`)

The `primesieve` library is used to generate primes quickly. For more information: [[primesieve on GitHub](https://github.com/kimwalisch/primesieve/)]
To get `primesieve` on Ubuntu, use `apt install primesieve`

### Building and running
`erdos` was authored on Ubuntu 24.04 LTS running on Windows Subsystem for Linux. Other platforms haven't been tested.

#### Linux
Building and running `erdos` is done by running the simple build script in the `cpp` directory:
```
cd cpp
sh build.sh
./erdos
```

### To-do list
* Put a visualization frontend on the program with a GUI (looking into `matplotlib-cpp` and `matplot++` libraries)
* Improve design of grid data format so that it is easier to use and manipulate correctly
* Write tests to support correctness in calculation
* Fix (literal) edge case of failing to remove points on the left and bottom edges of the grid that lead nowhere
* Design and write a backtracking algorithm to determine all possible paths given a grid spec

## Python

### `visible_point_grid.py`

The Python script `visible_point_grid.py` has been translated from the C++ code using ChatGPT, with some cleanup afterward. It uses the following libraries:
* `sympy` to precompute primes
* `numpy` to reshape the coordinate data for plotting
* `matplotlib` to plot the grid

**Update:** `visible_point_grid.py` now uses the prime and semiprime caches described below and features
* highlighting of semiprime "avenues" and "streets"
* on-hover coordinate labels

**Usage:**
1. Specify the min and max coordinates for the x- and y-axes in the grid. These variables are labeled with an `# EDIT HERE:` comment.
2. Specify a number to generate primes and semiprimes up to for the cache if the default of 1 million is too small.
3. Run the script to view the grid.

### Prime and semiprime generation and caching
The `sympy` package is now used to generate primes much faster than `primePy`. I'm able to generate primes up to 100 million in less than 2 minutes on my machine.

Primes are automatically cached to disk with the `pickle` package. You can start generating primes up to some huge number, walk away and make a coffee, and have your primes ready to use when you return. You can then use the cached primes for tasks involving prime checking in all future sessions. For example, generating semiprimes is much more efficient if a cache of primes is available for use.

#### Suggested workflow for incorporating prime caching into your project:

0. Install the `sympy` package with `pip install sympy` (or the equivalent command for your Python environment manager)
1. Put `prime_cache.py` in the same directory your notebook or script.
2. Copy all the code from `example_prime_cache_basic.py` into your notebook or script.
3. Specify the number `n` to generate primes up to with `prime_max = n` (I've used 1 million as a default since this only takes a second or two to run.)
4. Run the code. This will generate the primes and cache them to a file in your working directory named `primes_up_to_n.pkl`. Calling `init_cache_primes(prime_max)` in future sessions will load this file instead of generating the primes again. (If the `.pkl` file is deleted or `n` is changed, it will generate the primes again.)
5. Write or generate user code that uses the `primes` sets and the `is_prime` functions imported from `prime_cache.py`.

For example, the following prompt to ChatGPT:

```I'd like to generate all sphenic numbers up to a number n. Write some Python code that does this efficiently. Assume I have an existing function is_prime(n: int, primes: set[int]) -> bool for checking primes and a set primes: set[int] that already contains all primes needed for the task.```

generated a function for generating sphenic numbers using cached primes. It was able to generate sphenic numbers up to 10000 in a few seconds without any modification. See `example_prime_cache_sphenic_numbers.py`.

#### Semiprime generation
I've included a function to generate and cache distinct semiprimes, since they are relevant to this problem. Generating semiprimes works the same as generating primes, described above. See `example_prime_cache_basic.py`.

### Testing the code
The Python code now has a minimal suite of tests to guarantee correctness in prime and semiprime number generation. If you stumble upon a number that is missing from those sets (or a number that is erroneously included), please let me know so that I can fix the bug and add a test case.

## Lean
The Lean directory contains an attempt to have ChatGPT provide a proof of a solution to the problem. It wasn't successful, but the code has been saved as a curiosity.

All code in the `lean` directory was generated by ChatGPT and has not been modified beyond changing some of the import statements. There is no guarantee it compiles, nor that it proves anything if it does compile.