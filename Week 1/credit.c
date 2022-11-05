#include <cs50.h>
#include <stdio.h>

void cardtype(long number);

int main(void)
{
    long card;
    // ask user for input
    do
    {
        card = get_long("Number: ");
    }
    while (card < 0 || card >  9999999999999999);

    // check for credit card
    // First, we check whether the digit is an odd or even place
    bool odd = true;

    // Then, we add it to the sum
    int sum = 0;

    // we count the digits
    int digits = 0;

    // we save the last number we add
    int add = 0;

    // we need to keep track of the first digit
    int first = 0;

    // but we also need the second  number
    int second = 0;

    // now we loop through the digits
    while (card > 0)
    {
        second = first;
        first = card % 10;

        add = card % 10;
        if (odd)
        {
            sum += add;
        }
        else
        {
            // multiply the number by two
            // careful: if the new number is > 10, we add the digits of this new number
            if (add < 5)
            {
                sum += add * 2;
            }
            else
            {
                add = add * 2;
                sum += (add % 10) + (add / 10);
            }
        }
        card = card / 10;
        odd = !odd;
        digits++;
    }
    //printf("We end up with these values");

    //printf("add: %i\n", add);
    //printf("first: %i\n", first);
    //printf("second: %i\n", second);
    //printf("digits: %i\n", digits);
    //printf("card: %li\n", card);
    if (digits < 13 || digits > 16)
    {
        printf("INVALID");
    }
    else if (digits == 15 && first == 3 && (second == 4 || second == 7) && (sum % 10 == 0))
    {
        printf("AMEX");
    }
    else if (digits == 16 && first == 5 && (second == 1 || second == 2 || second == 3 || second == 4 || second == 5) && (sum % 10 == 0))
    {
        printf("MASTERCARD");
    }
    else if ((digits == 13 || digits == 16) && (first == 4) && (sum % 10 == 0))
    {
        printf("VISA");
    }
    else
    {
        printf("INVALID");
    }

    printf("\n");
}