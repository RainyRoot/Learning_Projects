# Problem 5: Smallest multiple of numbers 1 to 20

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def lcm(a, b):
    return a * b // gcd(a, b)

def smallest_multiple(n):
    result = 1
    for i in range(2, n + 1):
        result = lcm(result, i)
    return result

multiple_limit = 20

smallest_mult_result = smallest_multiple(multiple_limit)
print(f"Problem 5: The smallest multiple of numbers from 1 to {multiple_limit} is: {smallest_mult_result}")
