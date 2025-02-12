# Problem 3: Largest prime factor of 600851475143

def largest_prime_factor(n):
    factor = 2
    while factor * factor <= n:
        if n % factor == 0:
            n //= factor
        else:
            factor += 1
    return n

number = 600851475143

largest_factor = largest_prime_factor(number)
print(f"Problem 3: The largest prime factor of {number} is: {largest_factor}")
