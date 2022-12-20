// Write a program called substitution that enables you to encrypt messages using a substitution cipher
// Problem link: https://cs50.harvard.edu/x/2022/psets/2/substitution/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_key_alpha(string key);
bool is_char_repeated(string key);
string encipher(string text, string key);

int main(int argc, string argv[])
{
    // Validate the key

    // Check if there's a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check if the command-line argument/key contains 26 characters
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check if the command-line argument/key contains only alphabetical characters
    if (!is_key_alpha(argv[1]))
    {
        printf("Key must only contain alphabetic characters\n");
        return 1;
    }

    // Check if the command-line argument/key contains repeated characters
    if (is_char_repeated(argv[1]))
    {
        printf("Key must not contain repeated characters\n");
        return 1;
    }

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Encipher
    string ciphertext = encipher(plaintext, argv[1]);

    // Print ciphertext
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

// Check if key contains only alphabetical characters
bool is_key_alpha(string key)
{
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }
    }
    return true;
}

// Check if key contains repeated characters
bool is_char_repeated(string key)
{
    for (int i = 0; i < 26; i++)
    {
        for (int j = i + 1; j < 26 - j; j++)
        {
            if (key[i] == key[j])
            {
                return true;
            }
        }
    }
    return false;
}

// Encipher plaintext
string encipher(string text, string key)
{
    // Iterate through plaintext
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Check if character is alphabetical
        if (isalpha(text[i]))
        {
            // Check if character is uppercase or lowercase
            if (isupper(text[i]))
            {
                int character = text[i] - 65; // Convert ASCII to alphabetical index

                // Check if the key character is uppercase or lowercase
                if (islower(key[character]))
                {
                    text[i] = toupper(key[character]); // Turn key character to uppercase and replace plaintext character
                }
                else
                {
                    text[i] = key[character]; // Replace plaintext character
                }
            }
            else if (islower(text[i]))
            {
                int character = text[i] - 97;

                if (isupper(key[character]))
                {
                    text[i] = tolower(key[character]);
                }
                else
                {
                    text[i] = key[character];
                }
            }
        }
    }
    return text;
}