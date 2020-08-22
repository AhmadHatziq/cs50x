#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
bool isLetter(char c);
int count_sentences(string text);

// This code is for the readibility problem at: https://cs50.harvard.edu/x/2020/psets/2/readability/#:~:text=readability.c.
int main(void)
{
    // Get user input text
    string text = get_string("Text: ");

    // Get total letters
    int num_letters = count_letters(text);
    // printf("Letters: %i\n", num_letters);

    // Get total words
    int num_words = count_words(text);
    // printf("Words: %i\n", num_words);

    // Get number of sentences
    int num_sentences = count_sentences(text);
    // printf("Sentences: %i\n", num_sentences);

    // Calculate L, S, index
    float L = (100 * (float) num_letters / (float) num_words);
    float S = (100 * (float) num_sentences / (float) num_words);
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // Print the grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// Returns the number of sentences ie seq of chars ending with '.', '!', '?'
int count_sentences(string text)
{
    int special_char_count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        if (c == '.' || c == '!' || c == '?')
        {
            special_char_count++;
        }
    }
    return special_char_count;
}

// Returns the number of words in the text string.
int count_words(string text)
{
    if (strlen(text) == 0)
    {
        return 0;
    }

    // Assumes that the first word exists as logic searches for a ' ' and a subsequent non-space char
    int count = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] == ' ') && (text[i + 1] != ' '))
        {
            count++;
        }
    }

    return count;
}

// Returns the number of chars, excluding white spaces
int count_letters(string text)
{
    int totalChars = strlen(text);
    int originalLength = strlen(text);

    totalChars = 0;
    for (int i = 0; i < originalLength; i++)
    {
        char c = text[i];

        // Increment the count if there is a letter char in the text
        if (isLetter(c) == true)
        {
            totalChars++;
        }
    }

    return totalChars;
}

// Returns true if the char is a letter (lower or upper case)
bool isLetter(char c)
{
    int charValue = c;

    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    {
        return true;
    }
    else
    {
        return false;
    }
}