# Problem 6: Difference between sum of squares and square of sum
def sum_square_difference(n):
    sum_of_squares = sum(i ** 2 for i in range(1, n + 1))
    square_of_sum = sum(range(1, n + 1)) ** 2
    return square_of_sum - sum_of_squares

n = 100

ssd_result = sum_square_difference(n)
print(f"Problem 6: The difference between the sum of squares and the square of the sum for first {n} natural numbers is: {ssd_result}")
