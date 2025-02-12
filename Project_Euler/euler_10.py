# Problem 10: Sum of primes below two million

def sum_of_primes_below(limit):
    sieve = [True] * limit
    sieve[0:2] = [False, False]
    for i in range(2, int(limit ** 0.5) + 1):
        if sieve[i]:
            sieve[i * i:limit:i] = [False] * len(range(i * i, limit, i))
    return sum(i for i, is_prime in enumerate(sieve) if is_prime)

prime_limit = 2000000

sum_primes_result = sum_of_primes_below(prime_limit)
print(f"Problem 10: The sum of all primes below {prime_limit} is: {sum_primes_result}")
