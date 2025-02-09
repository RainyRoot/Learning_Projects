// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counter for words that passed my hash function
unsigned int word_count = 0;

// Hashes word to a number based on sum of ASCII values
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N; // Ensure the index stays within bounds of the hash table
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word);

    for (node *current = table[index]; current != NULL; current = current->next)
    {
        // Compare the lowercase word in the current node with the given word
        if (strcasecmp(current->word, word) == 0)
        {
            // Word found in dictionary
            return true;
        }
    }

    // Word not found in dictionary
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file for reading
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Could not open dictionary %s\n", dictionary);
        return false;
    }

    // Read each word from the file and insert into hash table
    while (true)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            fclose(source);
            printf("Memory allocation failed.\n");
            return false;
        }

        // Read a word from the dictionary
        if (fscanf(source, "%s", n->word) == EOF)
        {
            // End of file reached
            free(n);
            break;
        }

        // Convert word to lowercase
        for (int i = 0; n->word[i] != '\0'; i++)
        {
            n->word[i] = tolower(n->word[i]);
        }

        // Hash the word to obtain its hash value
        unsigned int index = hash(n->word);

        // Insert the new node into the hash table at the calculated index
        n->next = table[index];
        table[index] = n;

        word_count++;
    }
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate over each bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        // Get the head of the linked list in the current bucket
        node *cursor = table[i];

        // Traverse the linked list and free memory for each node
        while (cursor != NULL)
        {
            // Keep track of the next node before freeing the current one
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }

        // After freeing all nodes in the linked list, set the bucket to NULL
        table[i] = NULL;
    }

    // Unloading successful
    return true;
}
