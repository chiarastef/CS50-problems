# Implement a program that identifies a person based on their DNA
# Problem link: https://cs50.harvard.edu/x/2022/psets/6/dna/

import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit()

    # Read database file into a variable
    data = []

    with open(sys.argv[1], "r") as file_data:
        reader_data = csv.DictReader(file_data)
        for row in reader_data:
            data.append(row)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file_sequence:
        sequence = file_sequence.read()

    # Find longest match of each STR in DNA sequence
    # Get list of STR
    keys = list(data[0].keys())
    # Remove "name" key
    keys.pop(0)

    # Create dictionary with longest match for each STR and populate it
    str_count = {}

    for i in keys:
        str_count[i] = longest_match(sequence, i)

    # Check database for matching profiles
    # Create list with names of possible matches
    name_match = []

    # Itirate over STR counts from database and sequence
    for row1 in data:
        for row2 in str_count.keys():
            # If the numbers match add name to list of possible matches
            if int(row1[row2]) == str_count[row2]:
                name_match.append(row1["name"])

    # Iterate over list of matches to count how many there are for every name
    for name in name_match:
        count = name_match.count(name)
        # If the count equals the number of STR in the database we found a match
        if count == len(keys):
            print(name)
            sys.exit()

    # Print "No match" in case of nothing found
    print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


# main()
if __name__ == "__main__":
    main()
