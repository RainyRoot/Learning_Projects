# Problem 1 Sum of Multiples of 3 or 5

def sum_of_multiples(limit, multiples):
    return sum(
        number for number in range(limit)
        if any(number % m == 0 for m in multiples)
    )

limit = 1000
multiples = [3, 5]

result = sum_of_multiples(limit, multiples)
print(f"The sum of all multiples of {multiples} below {limit} is: {result}")
