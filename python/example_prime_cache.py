# Prime cache example

# Import and initialize prime cache
from prime_cache import (
    load_or_generate_primes,
    load_or_generate_semiprimes
)

# EDIT: specify the number to generate primes and semiprimes up to
prime_semiprime_max = 1000000

primes: set[int] = load_or_generate_primes(prime_semiprime_max)
semiprimes: set[int] = load_or_generate_semiprimes(prime_semiprime_max, primes)

# User code goes here...



# Demo usage

print("Largest prime generated: ", max(primes))
print("1049 is prime:", 1049 in primes)                     # prints "True"
print("1050 is prime:", 1050 in primes)                     # prints "False"
print()

print("Largest distinct semiprime generated: ", max(semiprimes))
print("1049 is a distinct semiprime:", 1049 in semiprimes)  # prints "False"
print("2098 is a distinct semiprime:", 2098 in semiprimes)  # prints "True"
print("49 is a distinct semiprime:", 49 in semiprimes)      # prints "False"