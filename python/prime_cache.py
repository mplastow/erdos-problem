# Implementation of prime and semiprime caches

# Last edited by Matt Plastow on May 20, 2025

import os
import pickle
from sympy import primerange

"""
Generate and return all prime numbers <= n.
"""
def generate_primes_up_to(n_max: int) -> set:
    return set(primerange(2, n_max + 1))

"""
Generate all semiprimes (products of two distinct primes) that are <= n.
Assumption: Will only ever be called after `prime_set` has been pre-populated up to n,
since it relies on the contents of prime_set
"""
def generate_distinct_semiprimes_up_to(n_max: int, primes: set[int]) -> set:
    if primes is None:
        primes = load_or_generate_primes(n_max)
    primes_list = sorted(primes)
    semiprimes = set()
    for i, p1 in enumerate(primes_list):
        if p1 * p1 > n_max:
            break
        max_p2 = n_max // p1
        for p2 in primes_list[i+1:]:
            if p2 > max_p2:
                break
            semiprimes.add(p1 * p2)
    return semiprimes

"""
Caches an object on disk
"""
def save_to_cache(obj, filename: str):
    with open(filename, 'wb') as f:
        pickle.dump(obj, f)

"""
Loads an object from a cache on disk
"""
def load_from_cache(filename: str):
    if os.path.exists(filename):
        with open(filename, 'rb') as f:
            return pickle.load(f)
    return None

"""
Load primes up to n_max from cache on disk (if a file with that exact value exists), or else
Generate primes up to n_max and cache them on disk for faster lookup
"""
def load_or_generate_primes(n_max: int) -> set[int]:
    prime_cache_path = f"primes_up_to_{n_max}.pkl"
    primes = load_from_cache(prime_cache_path)
    if primes is None:
        primes = generate_primes_up_to(n_max)
        save_to_cache(primes, prime_cache_path)
    return primes

"""
Load semiprimes up to n_max from cache on disk (if a file with that exact value exists), or else
Generate semiprimes up to n_max and cache them on disk for faster lookup
"""
def load_or_generate_semiprimes(n: int, primes: set[int]) -> set[int]:
    semiprime_cache_path = f"semiprimes_up_to_{n}.pkl"
    semiprimes = load_from_cache(semiprime_cache_path)
    if semiprimes is None:
        semiprimes = generate_distinct_semiprimes_up_to(n, primes)
        save_to_cache(semiprimes, semiprime_cache_path)
    return semiprimes

# Do nothing! This file contains an implementation of functions used elsewhere!
sum = 1 + 1