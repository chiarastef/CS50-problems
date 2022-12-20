# Implement a program that computes the approximate grade level needed to comprehend some text
# Problem link: https://cs50.harvard.edu/x/2022/psets/6/readability/

from cs50 import get_string
import re

# Get the user’s input
text = get_string("Text: ")


# Keep count of letters, words and sentences
# (add 1 to words for the last word)
n_letters = 0
n_words = 1
n_sentences = 0


for letter in text:
    # Count letters
    if re.match("[a-z]|[A-Z]", letter):
        n_letters += 1
    # Count white spaces to count words
    if letter == " ":
        n_words += 1
    # Count sentences (consider any sequence of characters that ends with a . or a ! or a ? to be a sentence)
    if letter == "." or letter == "!" or letter == "?":
        n_sentences += 1


# Apply the Coleman-Liau index
# index = 0.0588 * L - 0.296 * S - 15.8 (L = average number of letters per 100 words, S = average number of sentences per 100 words)
avg_letters = (n_letters / n_words) * 100
avg_sentences = (n_sentences / n_words) * 100

idx = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8)

# Print result
if idx < 1:
    print("Before Grade 1")
elif idx >= 16:
    print("Grade 16+")
else:
    print(f"Grade {idx}")
