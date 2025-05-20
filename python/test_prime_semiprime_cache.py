import unittest

from prime_cache import (
    generate_primes_up_to,
    generate_distinct_semiprimes_up_to
)

from visible_point_grid import (
    is_prime,
    is_distinct_semiprime
)

class TestPrimeSemiprimeGeneration(unittest.TestCase):
    prime_set: set[int]
    semiprime_set: set[int]

    def test_known_primes(self):
        primes = generate_primes_up_to(30)
        expected = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
        for p in expected:
            self.assertIn(p, primes)

    def test_is_prime_function(self):
        prime_set = set(generate_primes_up_to(100))
        self.assertTrue(is_prime(97, prime_set))
        self.assertFalse(is_prime(100, prime_set))

    def test_distinct_semiprime_generation(self):
        primes = set(generate_primes_up_to(210))
        semiprimes = generate_distinct_semiprimes_up_to(210, primes)
        expected = {6, 10, 14, 15, 21, 22, 26, 33, 34, 35, 38, 39, 46, 51, 55,
                    57, 58, 62, 65, 69, 74, 77, 82, 85, 86, 87, 91, 93, 94, 95,
                    106, 111, 115, 118, 119, 122, 123, 129, 133, 134, 141, 142,
                    143, 145, 146, 155, 158, 159, 161, 166, 177, 178, 183, 185,
                    187, 194, 201, 202, 203, 205}
        self.assertTrue(expected.issubset(semiprimes))
        self.assertFalse(is_distinct_semiprime(36, semiprimes)) # 6 * 6 → not a product of primes
        self.assertFalse(is_distinct_semiprime(49, semiprimes)) # 7 * 7 → not distinct
        self.assertFalse(is_distinct_semiprime(101, semiprimes))# prime

    def test_all_semiprimes_are_valid(self):
        primes = set(generate_primes_up_to(100))
        semiprimes = generate_distinct_semiprimes_up_to(1000, primes)
        for sp in semiprimes:
            factors = [p for p in primes if sp % p == 0 and (sp // p) in primes and p != (sp // p)]
            self.assertTrue(len(factors) > 0, f"Invalid semiprime: {sp}")

if __name__ == '__main__':
    unittest.main()
