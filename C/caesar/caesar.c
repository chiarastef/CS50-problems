// Design and implement a program that encrypts messages using Caesar’s cipher.
// Problem link: https://cs50.harvard.edu/x/2022/psets/2/caesar/

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool is_digit(string text);
string rotate(string text, int key);

int main(int argc, string argv[])
{
    // Check if user has provided only 1 command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check if command-line argument is a digit
    if (!is_digit(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert key provided by user (string) to an int
    int key = atoi(argv[1]);

    // Get plaintext from user
    string plaintext = get_string("Plaintext: ");

    // Compute ciphertext
    string ciphertext = rotate(plaintext, key);

    // Print out ciphertext
    printf("Ciphertext: %s\n", ciphertext);
    return 0;
}

// Check if command-line argument is digit
bool is_digit(string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (!isdigit(text[i]))
        {
            return false;
        }
    }
    return true;
}

// Compute ciphertext
string rotate(string text, int key)
{
    // Iterate over plaintext provied
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Check if single char is alphabetical
        if (isalpha(text[i]))
        {
            // Check if char is uppercase or lowercase
            if (isupper(text[i]))
            {
                text[i] -= 65; // Convert ASCII to alphabetical index
                text[i] += key; // Add key value
                text[i] %= 26;  // Wrap around from Z to A
                text[i] += 65; // Convert result back to ASCII
            }
            else
            {
                text[i] -= 97;
                text[i] += key;
                text[i] %= 26;
                text[i] += 97;
            }
        }
    }
    return text;
}