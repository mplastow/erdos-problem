# Import and initialize prime cache
from prime_cache import (
    load_or_generate_primes,
    load_or_generate_distinct_semiprimes,
    is_prime,
    is_distinct_semiprime
)

# EDIT THIS: specify the number to generate primes and semiprimes up to
prime_semiprime_max = 10000000

primes: set[int] = load_or_generate_primes(prime_semiprime_max)
semiprimes: set[int] = load_or_generate_distinct_semiprimes(prime_semiprime_max, primes)

# User code goes here...