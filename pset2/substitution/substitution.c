#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

int alphabetIndex(char c);
char getCharFromCipher(string cipher, int index);
bool isUpper(char c);
bool isLetter(char c);
bool gotDuplicates(string mapping);

// Code for substitution problem at: https://cs50.harvard.edu/x/2020/psets/2/substitution/
int main(int argc, string argv[])
{

    // Check for correct no. of CLI arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Extract mappings
    string mapping = argv[1];

    // Check if mappings has length 26
    int mapping_length = strlen(mapping);
    if (mapping_length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check for valid chars ie all must be in the alphabet
    for (int i = 0; mapping[i] != '\0'; i++)
    {
        if (isLetter(mapping[i]) == false)
        {
            printf("Key must contain only letters.\n");
            return 1;
        }
    }

    // Check for duplicates in mapping string
    bool gotDuplicate = gotDuplicates(mapping);
    if (gotDuplicate == true)
    {
        printf("Key must not contain duplicates.\n");
        return 1;
    }


    // Get plaintext
    string plaintext = get_string("plaintext: ");

    // Convert to ciphertext
    char ciphertext[strlen(plaintext)];

    int i = 0;
    for (i = 0; plaintext[i] != '\0'; i++)
    {
        int index = alphabetIndex(plaintext[i]);

        if (index == -1)
        {
            // Invalid alphabet
            ciphertext[i] = plaintext[i];
        }
        else
        {

            if (isUpper(plaintext[i]))
            {
                // printf("is upper\n");
                ciphertext[i] = toupper(getCharFromCipher(mapping, index));
            }
            else
            {
                ciphertext[i] = tolower(getCharFromCipher(mapping, index));
            }
        }
    }
    ciphertext[i] = '\0';
    printf("ciphertext: %s\n", ciphertext);
}

// Chekcs if the string contains any duplicates.
bool gotDuplicates(string mapping)
{
    char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                        'v', 'w', 'x', 'y', 'z'
                       };
    int index_mapping[26] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int i = 1; i < 26; i++)
    {
        char *j = strchr(mapping, letters[i]);
        int index = (int)(j - mapping);
        index_mapping[i] = index;
    }

    for (int i = 0; i < 26; i++)
    {
        //printf("%i ", index_mapping[i]);
    }

    int count = 0;

    string key = mapping;

    for (int i = 0; i < strlen(key); i++)
    {
        count = 1;
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (key[i] == key[j] && key[i] != ' ')
            {
                count++;
                //Set string[j] to 0 to avoid printing visited character
                key[j] = '0';
            }
        }
        //A character is considered as duplicate if count is greater than 1
        if (count > 1 && key[i] != '0')
        {
            return true;
        }
    }

    return false;
}

// Returns true if c is an alphabet char.
bool isLetter(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return true;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns true if c is an upper case
bool isUpper(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return false;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns the char from the cipher given the index
char getCharFromCipher(string cipher, int index)
{
    return cipher[index];
}

int alphabetIndex(char c)
{
    int index = -1;

    // lowercase
    if (c >= 'a' && c <= 'z')
    {
        index = c - 'a';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        // uppercase
        index = c - 'A';
    }
    else
    {
        index = -1;
    }

    return index;
}