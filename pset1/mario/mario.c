#include <stdio.h>
#include <cs50.h>
#include <string.h>

bool isValid(int n);
void repeatedChars(int n, char c);


// This code is for the harder mario problem: https://cs50.harvard.edu/x/2020/psets/1/mario/more/.
int main(void)
{
    bool valid;
    int height;

    // Keep prompting for a number that is between 1 and 8 inclusive.
    do
    {
        height = get_int("Height: ");
        valid = isValid(height);

    }
    while (valid == 0);

    // Find the length of the base string (including spaces).
    int base_length = 2 + 2 * height;

    // Counter for each level
    for (int current_height = 1; current_height <= height; current_height++)
    {
        int num_brick_left = current_height;
        int num_space_left = height - current_height;

        repeatedChars(num_space_left, ' ');
        repeatedChars(num_brick_left, '#');
        repeatedChars(2, ' ');
        repeatedChars(num_brick_left, '#');
        printf("\n");
    }
}

// Prints the string with the character argument repeated int n times.
void repeatedChars(int n, char c)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
    return;
}


// Returns true if the number is valid ie between 1 and 8 inclusive.
bool isValid(int n)
{
    if ((n <= 8) && (n >= 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}