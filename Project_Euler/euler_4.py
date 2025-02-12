# Problem 4: Largest palindrome product of two 3-digit numbers

def largest_palindrome_product():
    max_palindrome = 0
    for i in range(100, 1000):
        for j in range(i, 1000):
            product = i * j
            if str(product) == str(product)[::-1] and product > max_palindrome:
                max_palindrome = product
    return max_palindrome

palindrome_result = largest_palindrome_product()
print(f"Problem 4: The largest palindrome product of two 3-digit numbers is: {palindrome_result}")
