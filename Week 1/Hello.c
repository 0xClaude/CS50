#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // asks user for input
    string name = get_string("What is your name? ");
    // greets the user
    printf("Hello, %s\n", name);
}