// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// keep track of added values in dictionary
int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // get the hash value for the word first
    int hashvalue = hash(word);

    // start at the linked list at index
    node *cursor = table[hashvalue];

    // now loop through the list and check if cursor->word strcasecmp word is true
    while (cursor != NULL)
    {
        // check if the current word is the word we're looking for
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // if it's not, check for the next word
        cursor = cursor->next;
    }
    // if eventually the word wasn't in the list, return false
    return false;
}

// Hashes word to a number
// OK
unsigned int hash(const char *word)
{
    int tmp = 0;
    // add every letter and then %N
    for (int i = 0; i < strlen(word); i ++)
    {
        tmp += tolower(word[i]);
    }
    return tmp % N;
}

// Loads dictionary into memory, returning true if successful, else false
// OK
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");

    // check if opening didn't succeed
    if (file == NULL)
    {
        return false;
    }

    // create temporary variable word with a length of LENGTH + 1
    // +1 because of the \0
    char tempword[LENGTH + 1];

    // scan word by word, and put each word temporarily in "word"
    while (fscanf(file, "%s", tempword) != EOF)
    {
        // create a new node and allocate some memory for it
        node *n = malloc(sizeof(node));

        // failsafe that we got some memory
        if (n == NULL)
        {
            return false;
        }
        // insert the word into the node
        strcpy(n->word, tempword);

        // since we don't know what the next word will be, we will set the next value to NULL
        n->next = NULL;

        // hash word to obtain a hash number
        int hashvalue = hash(tempword);

        // insert node into hash table at that location
        // first, we need to keep track of where the hashtable currently starts
        // we save this in our next pointer
        n->next = table[hashvalue];

        // then, we add the new node at the beginning of the table
        table[hashvalue] = n;

        // update the counter
        counter++;

    }

    // close the file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// OK
unsigned int size(void)
{
    // since we kept track of how many words were added, we can just return this value
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // first loop through all the items in the hashtable
    for (int i = 0; i < N; i ++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        // now clear memory until we reach NULL
        while (cursor != NULL)
        {
            // first set the cursor to the next node
            cursor = cursor->next;

            // free the temporary variable
            free(tmp);

            // move the temporary variable to current node
            tmp = cursor;
        }
    }
    return true;
}