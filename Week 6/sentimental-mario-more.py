# TODO
from cs50 import get_int


def main():

    # Ask for user input
    n = get_int("Height: ")
    while n < 1 or n > 8:
        n = get_int("Height: ")

    for i in range(n):
        # Print the left side
        # Print the empty spaces first
        for j in range(n - i - 1):
            print(" ", end="")
        # Print the blocks
        for j in range(i + 1):
            print("#", end="")

        # Print whitespace
        print("  ", end="")

        # Print the right side
        for j in range(i + 1):
            print("#", end="")
        # Print new line
        print()


if __name__ == "__main__":
    main()