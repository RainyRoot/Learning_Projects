# Problem 9: Special Pythagorean triplet

def special_pythagorean_triplet(sum_total):
    for a in range(1, sum_total // 3):
        for b in range(a + 1, sum_total // 2):
            c = sum_total - a - b
            if a * a + b * b == c * c:
                return a * b * c

triplet_sum = 1000

triplet_result = special_pythagorean_triplet(triplet_sum)
print(f"Problem 9: The product of the Pythagorean triplet for which a + b + c = {triplet_sum} is: {triplet_result}")
