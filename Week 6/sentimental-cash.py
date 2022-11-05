from cs50 import get_float


def main():
    # Assume that the only coins available are quarters (25¢), dimes (10¢), nickels (5¢), and pennies (1¢).

    # Ask user input
    n = get_float("Change owed: ")
    while (n < 0):
        n = get_float("Change owed: ")

    n = n * 100

    # Check for quarters
    coins = n // 25
    rest = n % 25

    # Check for dimes
    coins += rest // 10
    rest = rest % 10

    # Check for nickels
    coins += rest // 5
    rest = rest % 5

    # Check for pennies
    coins += rest

    # Cast the type to an integer
    coins = int(coins)

    # Print the result
    print(f"{coins}")


if __name__ == "__main__":
    main()