#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>

int charToInt(char c);

// This code is for the Credit problem at: https://cs50.harvard.edu/x/2020/psets/1/credit/.
int main(void)
{
    // Get a valid num from the user
    long num = get_long("Number: ");

    // Convert to string for easy parsing
    char str[256];
    sprintf(str, "%ld", num);

    long unsigned length = strlen(str); // Use %lu to print

    //printf("%c", str[0]);

    // Run Luhns algorithm to get the number
    int product_sum = 0;
    if (length % 2 == 0)
    {
        // Even length
        // Start from index 0
        for (int i = 0; i < length; i = i + 2)
        {
            char digit = str[i];
            int digit_num = charToInt(digit);
            // printf("%i", digit_num);

            if (digit_num * 2 >= 10)
            {
                product_sum += (digit_num * 2 - 10) + 1;
            }
            else
            {
                product_sum += digit_num * 2;
            }

        }

        // Second for loop for the other non-selected digits
        for (int i = 1; i < length; i = i + 2)
        {
            product_sum += charToInt(str[i]);
        }

    }
    else
    {
        // Odd length
        // Start from index 1
        for (int i = 1; i < length; i = i + 2)
        {
            char digit = str[i];
            int digit_num = charToInt(digit);
            // printf("%i", digit_num);

            if (digit_num * 2 >= 10)
            {
                product_sum += (digit_num * 2 - 10) + 1;
            }
            else
            {
                product_sum += digit_num * 2;
            }
        }

        // Second for loop for the other non-selected digits
        for (int i = 0; i < length; i = i + 2)
        {
            product_sum += charToInt(str[i]);
        }
    }

    // Extract first 2 digits
    int first_digit = charToInt(str[0]);
    int second_digit = charToInt(str[1]);
    int first_two = first_digit * 10 + second_digit;



    if (product_sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else if ((length == 15) && (first_two == 37 || first_two == 34))
    {
        printf("AMEX\n");
    }
    else if ((length == 16) && (first_two > 50) && (first_two < 56))
    {
        printf("MASTERCARD\n");
    }
    else if ((length == 16 || length == 13) && (first_digit == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}

int charToInt(char c)
{
    int num = 0;

    //Substract '0' from entered char to get corresponding digit.

    num = c - '0';

    return num;
}