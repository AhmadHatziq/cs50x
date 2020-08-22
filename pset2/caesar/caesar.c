#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

char encrypt(int key, char plainChar);

// Main code for the caesar problem at: https://cs50.harvard.edu/x/2020/psets/2/caesar/#:~:text=Caesar.
int main(int argc, string argv[])
{
    // Error checking if there are 0 or more than 1 command line arguments
    if (argc != 2) 
    {
        printf("Usage: Need 1 command line argument\n");
        return 1;
    }

    // Check if argv[1] is an int
    string key_string = argv[1];
    for (int i = 0; i < strlen(key_string); i++)
    {
        char c = key_string[i];
        if (isdigit(c) == false)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Extract the key
    int key = atoi(key_string);

    // Get plaintext
    string plaintext = get_string("plaintext: ");

    // Convert the chars
    printf("ciphertext: ");

    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        char entryptedChar = encrypt(key, plaintext[i]);
        printf("%c", entryptedChar);
    }
    printf("\n");

    return 0;
}

// Encrypts a char with the key
char encrypt(int key_original, char plainChar)
{
    int key = key_original % 26;
    if (plainChar >= 'A' && plainChar <= 'Z')
    {

        // Handle uppercase
        int plain_ascii = plainChar;

        // 90 is ascii of Z
        int encrypt_ascii = (plainChar + key) % 90;

        // 65 is ascii of A
        if (encrypt_ascii >= 65 && encrypt_ascii <= 90)
        {
            return encrypt_ascii;
        }
        else
        {

            return encrypt_ascii + 64;

        }


    }
    else if (plainChar >= 'a' && plainChar <= 'z')
    {

        // Handle lowercase
        int plain_ascii = plainChar;
        int encrypt_ascii = (plainChar + key) % 122;

        if (encrypt_ascii >= 97 && encrypt_ascii <= 122)
        {
            return encrypt_ascii;
        }
        else
        {

            return encrypt_ascii + 96;

        }



    }
    else
    {

        // Do nothing as char is not an alphabet
        return plainChar;

    }
}
