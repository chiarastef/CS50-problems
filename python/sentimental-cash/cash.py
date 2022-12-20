# Implement a program that calculates the minimum number of coins required to give a user change
# Problem link: https://cs50.harvard.edu/x/2022/psets/6/cash/

from cs50 import get_float

# Ask the user how much change is owed
change = get_float("Change owed: ")
# If user fails to provide a non-negative value, re-prompt the user for a valid amount until the user complies
while change < 0:
    change = get_float("Change owed: ")

# Keep count of number of coins
n_coins = 0

# Calculate the number of quarters
while change >= 0.25:
    n_coins += 1
    change = round(change - 0.25, 2)
# Calculate the number of dimes
while change >= 0.10:
    n_coins += 1
    change = round(change - 0.10, 2)
# Calculate the number of nickels
while change >= 0.05:
    n_coins += 1
    change = round(change - 0.05, 2)
# Calculate the number of pennies
while change >= 0.01:
    n_coins += 1
    change = round(change - 0.01, 2)

# Print minimum number of coins with which change can be made
print(n_coins)
