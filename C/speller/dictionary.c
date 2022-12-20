// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table (26^2)
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Hash word to obtain a hash value
    int idx = hash(word);

    // Access linked list and traverse it looking for the word
    for (node *tmp = table[idx]; tmp != NULL; tmp = tmp->next)
    {
        // If word is found (case insensitive) return true
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Use first two letters of the word to compute hash value
    // Convert ASCII to alphabetical index
    int firstLetter = tolower(word[0]) - 'a';
    int secondLetter = 0;
    // Check if word has more than one letter
    if (strlen(word) > 1)
    {
        secondLetter = tolower(word[1]) - 'a';
    }
    // Compute hash value for word
    int result = firstLetter + secondLetter + (25 * firstLetter);
    return result;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    // Check if return value is null
    if (file == NULL)
    {
        return false;
    }

    // Read strings from file
    char wordBuffer[LENGTH + 1];
    while (fscanf(file, "%s", wordBuffer) != EOF)
    {
        // Create a new node
        node *n = malloc(sizeof(node));
        // Check if return value is null
        if (n == NULL)
        {
            return false;
        }

        // Copy word into node
        strcpy(n->word, wordBuffer);

        //Hash word
        int idx = hash(wordBuffer);

        // Insert node into hash table
        // Check if there's a node already
        if (table[idx] == NULL)
        {
            n->next = NULL;
            table[idx] = n;
        }
        else
        {
            // Insert at the beginning of the linked list (the new pointer points to the old head of the linked list)
            n->next = table[idx];
            table[idx] = n;
        }
    }
    // Close dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    //Iterate over hash table
    for (int i = 0; i < N; i++)
    {
        // Traverse linked list and count nodes
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            count++;
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Free list
    //Iterate over hash table
    for (int i = 0; i < N; i++)
    {
        // Go over each linked list
        while (table[i] != NULL)
        {
            // Use a temporary pointer to remember the next node before freeing the current one
            node *tmp = table[i]->next;
            // free the memory
            free(table[i]);
            // Set table[i] to the next node
            table[i] = tmp;
        }
    }
    return true;
}
