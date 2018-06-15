// Implements a dictionary's functionality
#include "node.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define HASHMAX 100000

node **hashtable;
bool loaded = false;
int wordCount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Copy word into w
    char *w = malloc(sizeof(char) * 46);
    strcpy(w, word);
    // Ensure word is lowercase
    for (int i = 0; w[i] != '\0'; i++)
    {
        w[i] = tolower(w[i]);
    }
    // Get hashcode for word
    int element = hash(w);
    // Check through the linked list at hashmax[element] for the word
    node *ptr = hashtable[element];
    while(ptr != NULL)
    {
        // If strcmp returns 0, word is a match and is in dictionary
        if (strcmp(w, ptr -> word) == 0)
        {
            free(w);
            return true;
        }
        // Else keep looking for word
        else
        {
            ptr = ptr -> next;
        }
    }
    free(w);
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Create hashtable
    hashtable = calloc(HASHMAX, sizeof(node*));
    // Ensure malloc succeeded
    if (hashtable == NULL)
    {
        fprintf(stderr, "Dictionary could not be opened for reading.");
        unload();
        return false;
    }
    FILE *inptr = fopen(dictionary, "r");
    // Make sure file could be opened
    if (inptr == NULL)
    {
        fprintf(stderr, "Dictionary could not be opened for reading.");
        unload();
        return false;
    }
    // Go through file till end of file, adding words into hashtable
    char *word = malloc(sizeof(char) * LENGTH + 1);
    while (fscanf(inptr, "%s", word) != EOF)
    {
        node *newNode = malloc(sizeof(node));
        // Ensure malloc succeeded
        if (newNode == NULL)
        {
            unload();
            fclose(inptr);
            return false;
        }
        // Copy word into node, get hashcode
        strcpy(newNode -> word, word);
        int element = hash(word);
        // Add node to linked list of element corresponding to hash code
        // If no linked list in corresponding element
        if (hashtable[element] == NULL)
        {
            hashtable[element] = newNode;
            newNode -> next = NULL;
        }
        else
        {
            newNode -> next = hashtable[element];
        }
        hashtable[element] = newNode;
        wordCount++;
    }
    free(word);
    loaded = true;
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return wordCount;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through array
    for (int i = 0; i < HASHMAX; i++)
    {
        // Iterate through each linked list
        node* cursor = hashtable[i];
        // Free each node
        while (cursor != NULL)
        {
            node* tmp = cursor;
            cursor = cursor -> next;
            free(tmp);
        }
    }
    free(hashtable);
    return true;
}

// Hash function, created by Dan Bernstein, 'djb2'
unsigned int hash(char *str)
    {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash % HASHMAX;
    }
