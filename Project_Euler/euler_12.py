# Problem 12: First triangle number with over 500 divisors

def count_divisors(n):
    count = 0
    i = 1
    while i * i <= n:
        if n % i == 0:
            count += 2 if i != n // i else 1
        i += 1
    return count

def first_triangle_number_with_divisors(divisor_limit):
    triangle, i = 0, 1
    while True:
        triangle += i
        if count_divisors(triangle) > divisor_limit:
            return triangle
        i += 1

divisor_limit = 500

triangle_number_result = first_triangle_number_with_divisors(divisor_limit)
print(f"Problem 12: The first triangle number with over {divisor_limit} divisors is: {triangle_number_result}")
