# Problem 2: Sum of even Fibonacci numbers below 4 million

def sum_of_even_fibonacci(limit):
    a, b = 1, 2
    total = 0
    while b < limit:
        if b % 2 == 0:
            total += b
        a, b = b, a + b
    return total

fib_limit = 4000000

fib_result = sum_of_even_fibonacci(fib_limit)
print(f"Problem 2: The sum of even Fibonacci numbers below {fib_limit} is: {fib_result}")
