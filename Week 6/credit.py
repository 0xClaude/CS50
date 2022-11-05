from cs50 import get_string
import sys


def main():
    # All American Express numbers start with 34 or 37;
    # most MasterCard numbers start with 51, 52, 53, 54, or 55
    # and all Visa numbers start with 4.
    # American Express uses 15-digit numbers, MasterCard uses 16-digit numbers, and Visa uses 13- and 16-digit

    # Get user input
    card = get_string("Number: ")

    # Check for American Express
    if ((card[0] == "3") and (card[1] in ["4", "7"]) and checkcard(card) and len(card) == 15):
        print("AMEX")

    # Check for MasterCard
    elif ((card[0] == "5") and (card[1] in ["1", "2", "3", "4", "5"]) and checkcard(card) and len(card) == 16):
        print("MASTERCARD")

    # Check for VISA
    elif ((card[0] == "4") and checkcard(card) and (len(card) == 13 or len(card) == 16)):
        print("VISA")

    # Something went wrong
    else:
        print("INVALID")


def checkcard(creditcard):
    """ Iterate through the numbers and add """
    checksum = 0
    odd = True

    # Start iterating through the card backwards
    for i in range(len(creditcard) - 1, -1, -1):
        if odd:
            checksum += int(creditcard[i])
            odd = False
        else:
            # Check if we don't add too much
            if int(creditcard[i]) >= 5:
                tmp = int(creditcard[i]) * 2
                checksum += tmp % 10
                checksum += tmp // 10
            else:
                checksum += int(creditcard[i]) * 2
            odd = True
    return checksum % 10 == 0


if __name__ == "__main__":
    main()