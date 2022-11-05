#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // ask for input
    string text = get_string("Text: ");

    // count the letters
    int letters = count_letters(text);

    // count the words
    int words = count_words(text);

    // count the sentences
    int sentences = count_sentences(text);

    // index = 0.0588 * L - 0.296 * S - 15.8
    // where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.

    // calculate the average letters per 100 words
    float average_letters = (float)letters / (float)words * 100.0;

    // calculate the average sentences per 100 words
    float average_sentences = (float)sentences / (float)words * 100.0;

    // calculte the whole formula
    float index = 0.0588 * average_letters - 0.296 * average_sentences - 15.8;

    // round to the next integer
    int rounded = round(index);

    // check whether the grade is below 1 or above 16
    if (rounded < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (rounded > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", rounded);
    }
}

int count_letters(string text)
{
    // initialize counter
    int count = 0;

    // loop through the text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check if it's a letter
        if (isalpha(text[i]))
        {
            // if so, increase counter
            count++;
        }
    }
    // return the counter
    return count;
}

int count_words(string text)
{
    // initialize the counter
    int count = 0;

    // loop through text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check for space
        if (text[i] == 32)
        {
            count++;
        }
    }
    // add one to count to include the last word, because there won't be a space
    return count + 1;
}

int count_sentences(string text)
{
    // initialize counter
    int count = 0;

    // loop through text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check for ".", "!", and "?"
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            count++;
        }
    }
    return count;
}