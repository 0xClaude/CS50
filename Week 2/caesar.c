#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string text);
char rotate(char letter, int key);

int main(int argc, string argv[])
{
    // check if user entered an argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // check if user entered a number
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);

    // ask user for input
    string text = get_string("Plaintext:  ");

    printf("ciphertext: ");

    // loop through characters
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // rotate the character
        char letter = rotate(text[i], key);

        // print the character, no new line tho!
        printf("%c", letter);
    }
    printf("\n");

    return 0;
}

bool only_digits(string text)
{
    // loop through all the characters
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check if the character is a number
        if (!isdigit(text[i]))
        {
            // if not, exit the function and return false
            return false;
        }
    }
    // if all the checks passed, we can return true
    return true;
}

char rotate(char letter, int key)
{
    // check if the character is alphabetical
    if (isalpha(letter))
    {
        if (letter < 90)
        {
            // first we substract, so we can perform mathematical operations
            letter -= 65;

            // we add the key, then use modulo so we don't add too much
            letter += key;
            letter = letter % 26;

            // we add the basic number again
            letter += 65;

            // return the result
            return letter;
        }
        else
        {
            // first we substract, so we can perform mathematical operations
            letter -= 97;

            // we add the key, then use modulo so we don't add too much
            letter += key;
            letter = letter % 26;

            // we add the basic number again
            letter += 97;

            // return the result
            return letter;
        }
    }
    else
    {
        // if the letter is not alphabetical, just return the same letter
        return letter;
    }
}