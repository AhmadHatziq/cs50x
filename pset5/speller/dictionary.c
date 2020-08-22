// Implements a dictionary's functionality

#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#include <ctype.h>

// Function prototypes
void printNumberOfElementsInBuckets();

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 135000;

// Hash table
node *table[N];

// Size of dictionary
int dict_size;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Initialise boolean var
    bool exists = false;

    // NTS: Should use strcasecmp, which compares 2 strings without caring for their case
    // Convert to lowercase
    int word_length = strlen(word);
    char lowercase_word[LENGTH + 1];
    int i = 0;
    char c;
    strcpy(lowercase_word, word);
    while (lowercase_word[i])
    {
        c = tolower(lowercase_word[i]);
        lowercase_word[i] = c;
        i++;
    }
    // printf("Lowercase word from text: %s\n", lowercase_word);

    // Get the hash index of the word
    int hash_index = hash(lowercase_word);

    //free(lowercase_word);

    // Pull out the node residing at the hash index
    node *head = table[hash_index];

    // Traverse and check if word exists along the linked list
    if (head == NULL)
    {
        exists = false;
    }
    else
    {

        node *current_node = table[hash_index];

        while (current_node != NULL)
        {

            // printf("Normal node | Current dict node word: %s| Current text word: %s\n", current_node -> word, lowercase_word);

            if (strcasecmp(current_node -> word, lowercase_word) == 0)
            {
                // Found a match, Return true
                exists = true;
                // free(lowercase_word);
                return exists;
            }
            else
            {
                // Do nothing as exists is initialized to false
                // Traverse to next node
                current_node = current_node -> next;
            }

        }

    }

    // free(lowercase_word);
    return exists;
}

// djb2 hash function taken from: https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f
// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // printf("Attempting to load %s\n", dictionary);

    // Check if dictionary exists
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        printf("Dictionary does not exist. \n");
        return false;
    }

    // Start reading lines from the dictionary
    char word [LENGTH + 1];
    while (fscanf(dict_file, "%s", word) != EOF)
    {

        // Create node
        node *new_node = malloc(sizeof(node));

        // Check node is successfully created
        if (new_node == NULL)
        {
            printf("Unable to create new node in bool load(...) \n");
            return false;
        }

        // Convert the word to lowercase
        int i = 0;
        char c;
        while (word[i])
        {
            c = tolower(word[i]);
            word[i] = c;
            i++;
        }
        // printf("Lower case dictionary word: %s\n", word);

        // Copy the new word into the new node
        strcpy(new_node -> word, word);

        // Set the next pointer to null
        new_node -> next = NULL;

        // Get a hash index from hash function
        // NTS: hash_index must be smaller than N.
        int hash_index = hash(word);

        // Access head node from hash table
        node *head = table[hash_index];

        // If head is null, first node. Else, append to the front of LL
        if (head == NULL)
        {

            table[hash_index] = new_node;

        }
        else
        {

            // Set the new node to point to current header
            new_node -> next = head;

            // Store the new header into the index
            table[hash_index] = new_node;

        }

        // Increment global var, total number of words in dictionary
        dict_size += 1;

    }

    // Close the file
    fclose(dict_file);

    // printf("Successfully read dictionary file\n");

    // printNumberOfElementsInBuckets();

    // printf("Total number of words in dictionary: %i\n", size());

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through all hash table indexes
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];

        if (head != NULL)
        {

            node *current_node = table[i];
            while (current_node != NULL)
            {
                node *tmp_node = current_node;
                current_node = current_node -> next;
                free(tmp_node);
            }

        }
    }

    return true;
}

// Debugger function to print number of elements in each hash index
void printNumberOfElementsInBuckets()
{
    // Counter to track total words in dictionary
    int total_dict_words = 0;

    // Iterate through each hash index
    for (int i = 0; i < N; i++)
    {

        // Extract first head node
        node *head = table[i];

        // Only print if there is more than one element
        if (head != NULL)
        {

            node *current_node = table[i];
            int counter = 0;

            while (current_node -> next != NULL)
            {
                counter += 1;
                current_node = current_node -> next;
            }
            counter += 1;

            printf("Index %i has %i nodes\n", i, counter);

            total_dict_words += counter;

        }
    }

    printf("Total dictionary words: %i \n", total_dict_words);

}