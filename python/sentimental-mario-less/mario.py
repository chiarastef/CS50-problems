# Problem link: https://cs50.harvard.edu/x/2022/psets/6/mario/less/

from cs50 import get_int

# Prompt the user for the pyramid’s height
height = get_int("Height: ")
# Check if it's a positive integer between 1 and 8, inclusive
# If the user fails to provide a positive integer between 1 and 8, re-prompt for the same again
while height < 1 or height > 8:
    height = get_int("Height: ")


# Generate the half-pyramid

white_spaces = height - 1
hashes = 1

# Pyramid height
for i in range(height):
    # White spaces before pyramid
    for j in range(white_spaces):
        print(" ", end="")
    # Hashes for pyramid
    for k in range(hashes):
        print("#", end="")
    white_spaces -= 1
    hashes += 1
    # Go to new line
    print("")