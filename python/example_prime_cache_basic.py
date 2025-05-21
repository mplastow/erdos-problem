# Import from prime_cache.py
from prime_cache import (
    init_cache_primes,
    init_cache_distinct_semiprimes,
    is_prime,
    is_distinct_semiprime
)

# EDIT THIS: specify the number to generate primes and semiprimes up to
prime_max: int = 1000000
semiprime_max: int = 1000000

# Initialize prime cache
primes: set[int] = init_cache_primes(prime_max)
semiprimes: set[int] = init_cache_distinct_semiprimes(semiprime_max, primes)

################
# User code goes here...
################

# Example user code:
print("1000 is prime:", is_prime(1000, primes))
print("1049 is prime:", is_prime(1049, primes))

print("1049 is a distinct semiprime:", is_distinct_semiprime(1000, semiprimes))
print("2098 is a distinct semiprime:", is_distinct_semiprime(2098, semiprimes))