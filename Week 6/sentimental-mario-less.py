# Import get_int
from cs50 import get_int


def main():
    n = get_int("Height: ")

    # Make sure the user enters a valid height
    while n < 1 or n > 8:
        n = get_int("Height: ")

    # Print the rows
    for i in range(n):

        # Print the empty spaces first
        for j in range(n-i-1):
            print(" ", end="")

        # Print the blocks
        for j in range(i+1):
            print("#", end="")

        # Print a new line
        print()


# Make sure file is run directly
if __name__ == "__main__":
    main()