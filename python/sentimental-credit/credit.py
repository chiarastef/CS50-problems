# Implement a program that determines whether a provided credit card number is valid according to Luhn’s algorithm
# Problem link: https://cs50.harvard.edu/x/2022/psets/6/credit/

from cs50 import get_int
import sys
import re


# Prompt the user for credit card number
number = get_int("Number: ")

# Convert number to string
number_str = str(number)

# Length of number
length_number = len(number_str)

# Check credit card number length
if length_number != 15 and length_number != 13 and length_number != 16:
    print("INVALID\n")
    sys.exit()


# Apply Luhn’s algorithm
sum1 = 0
sum2 = 0

# Every other digit, starting with the number’s second-to-last digit, to multiply by 2 and add products’ digits together
for i in range(length_number - 2, -1, -2):
    n_multiplied = int(number_str[i]) * 2
    if n_multiplied > 9:
        for digit in str(n_multiplied):
            sum1 += int(digit)
    else:
        sum1 += n_multiplied

# Sum of digits that weren’t multiplied by 2 (starting from the end)
for i in range(length_number - 1, -1, -2):
    sum2 += int(number_str[i])

# Add both sums and check if last digit is 0
total_sum = str(sum1 + sum2)
if total_sum[len(total_sum) - 1] != "0":
    print("INVALID\n")
    sys.exit()


# Print type of card
if re.match("^4", number_str):
    print("VISA\n")
elif re.match("^34|37", number_str):
    print("AMEX\n")
elif re.match("^5[1-5]", number_str):
    print("MASTERCARD\n")
else:
    print("INVALID\n")