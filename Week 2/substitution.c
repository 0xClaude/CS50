#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check if user entered exactly one argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }

    // store the key in a separate variable
    // we put it in upper case to calculate the difference
    string key = argv[1];

    // check if user entered enough characters
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check invalid characters in key
    // we can do this with one loop and check for non-alphabetical values

    for (int i = 0, n = strlen(key); i < n; i ++)
    {
        if (!isalpha(key[i]))
        {
            return 1;
        }
    }

    // Check duplicate characters in key
    // We can do this with two loops, checking for corresponding values

    for (int i = 0, n = strlen(key); i < n; i ++)
    {
        for (int j = 0; j < i; j ++)
        {
            if (key[i] == key[j])
            {
                return 1;
            }
        }
    }

    // get input from user
    string text = get_string("plaintext: ");

    // store alphabet in a variable
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    printf("ciphertext: ");

    // loop through the text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check if the current character is a a letter.
        // if not, we will printf the character
        if (isalpha(text[i]))
        {
            // now let's find the correspondending position in the alphabet
            // we do this via while loop since we most likely don't need to rotate through the whole alphabet
            // we upper the text here, because we also used uppercase alphabet
            int j = 0;
            while (alphabet[j] != toupper(text[i]))
            {
                j++;
            }

            // determine how much we need to rotate
            // TODO: we need to find a way to work around with modulo

            int rotate = toupper(key[j]) - alphabet[j];

            char letter = text[i];
            int sub;


            if (letter > 90)
            {
                sub = 97;
            }
            else
            {
                sub = 65;
            }
            // first we substract, so we can perform mathematical operations
            letter -= sub;

            // rotate it
            letter += rotate;
            letter = letter % 26;

            // we add the basic number again
            letter += sub;

            // print the letter
            printf("%c", letter);

        }
        else
        {
            // the character in question was no letter
            // so we printf the character
            printf("%c", text[i]);
        }


    }
    // clean up
    printf("\n");

    // code worked well
    return 0;
}