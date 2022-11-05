#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ask user for input
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n <= 0 || n > 8);

    // create the rows
    for (int i = 0; i < n; i++)
    {
        // create a for loop for the spaces
        for (int j = n - 1; j > i; j--)
        {
            printf(" ");
        }

        // create a for loop for the hash signs
        for (int j = n - i; j <= n; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}