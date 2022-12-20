// Write a program called readability that takes a text and determines its reading level.
// Problem link: https://cs50.harvard.edu/x/2022/psets/2/readability/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for text
    string text = get_string("Text: ");

    // Number of letters, words and sentences in text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Average number of letters per 100 words in the text
    float avgLetters = (letters / (float)words) * 100;
    // Average number of sentences per 100 words in the text
    float avgSentences = (sentences / (float)words) * 100;

    // Compute Coleman-Liau index (it outputs the U.S. grade level)
    int index = round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8);

    // Print out grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Calculate number of letters in text
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

// Calculate number of words in text
int count_words(string text)
{
    int spaces = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            spaces++;
        }
    }
    int words = spaces + 1;
    return words;
}

// Calculate number of sentences in text
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}