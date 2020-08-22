#include <stdio.h>
#include <cs50.h>

// This code is for the Hello problem set at: https://cs50.harvard.edu/x/2020/psets/1/hello/#:~:text=style50%20hello.c.
int main(void)
{
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}