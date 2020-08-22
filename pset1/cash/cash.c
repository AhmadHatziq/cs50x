#include <stdio.h>
#include <cs50.h>
#include <math.h>

// This code is for the Cash problem at :https://cs50.harvard.edu/x/2020/psets/1/cash/.
int main(void)
{
    float change_owed;

    // Prompt for a valid user input
    do
    {
        change_owed = get_float("Change owed: ");
    }
    while (change_owed < 0);

    // Convert to cents
    int cents = round(change_owed * 100);

    // Order of coins is 25 cents, 10 cents, 5 cents, 1 cent

    // Get num of 25 cent counts
    int num_25 = 0;
    while (cents >= 25)
    {
        num_25++;
        cents = cents - 25;
    }

    // Get num of 10 cents
    int num_10 = 0;
    while (cents >= 10)
    {
        num_10 ++;
        cents = cents - 10;
    }

    // Get num of 5 cents
    int num_5 = 0;
    while (cents >= 5)
    {
        num_5 ++;
        cents = cents - 5;
    }

    // Get num of 1 cents
    int num_1 = cents;

    int total_coins = num_1 + num_5 + num_10 + num_25;
    printf("%i\n", total_coins);
}