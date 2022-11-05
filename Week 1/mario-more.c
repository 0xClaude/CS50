#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ask for user input
    int n;
    do {
        n = get_int("Height: ");
    }
    while (n <= 0 || n > 8);

    // Generating rows
    for (int i = 0; i < n; i ++)
    {
        // Generating spaces
        for (int j = n - i - 1 ; j > 0; j--)
        {
            printf(" ");
        }
        // printing the left blocks
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        // printing the middle spaces
        printf("  ");
        // printing the right block
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }

}