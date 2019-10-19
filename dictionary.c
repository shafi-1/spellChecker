// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    char letter;
    bool is_word;
    struct node *children[N];
}
node;

bool freeChild(struct node *parent);

// Represents a trie
node *root;

node *ptr;

int wordNo = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1] = {0};

    bool wordComplete;
    // int wordNo = 1;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        wordComplete = false;

        ptr = root;
        int placement = -1;

        // iterate over each index of word
        for (int index = 0; index < LENGTH + 1; index++)
        {
            if (word[index] == '\'')
            {
                placement = 26;
            }
            else if (word[index] == 0)
            {
                // finished word
                ptr->is_word = true;
                wordComplete = true;
                break;
            }
            else
            {
                placement = word[index] % 97;
            }

            // check that the letter in children of nodes is not equal to NULL
            if (ptr->children[placement])
            {
                ptr = ptr->children[placement];
            }
            else
            {
                ptr->children[placement] = malloc(sizeof(node));
                ptr = ptr->children[placement];
                ptr->is_word = false;
                for (int i = 0; i < N; i++)
                {
                    ptr->children[i] = NULL;
                }
                ptr->letter = word[index];
            }
        }
        if (wordComplete)
        {
            wordNo++;
            continue;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordNo;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    ptr = root;
    const char *index;
    int placement;
    for (index = word; *index; index++)
    {
        char letter = tolower(*index);
        // check placement of character
        if (letter == '\'')
        {
            placement = 26;
        }
        else
        {
            placement = letter % 97;
        }

        if (ptr->children[placement])
        {
            ptr = ptr->children[placement];
        }
        else
        {
            return false;
        }
    }
    if(ptr->is_word)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    ptr = root;
    return freeChild(ptr);
}

bool freeChild(struct node *parent)
{
    for (int i = 0; i < N; i++)
    {
        if (parent->children[i])
        {
            freeChild(parent->children[i]);
        }
    }
    free(parent);
    return true;
}

